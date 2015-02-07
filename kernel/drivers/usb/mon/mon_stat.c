/* Copyright Statement:
 *
 * This software/firmware and related documentation ("MediaTek Software") are
 * protected under relevant copyright laws. The information contained herein
 * is confidential and proprietary to MediaTek Inc. and/or its licensors.
 * Without the prior written permission of MediaTek inc. and/or its licensors,
 * any reproduction, modification, use or disclosure of MediaTek Software,
 * and information contained herein, in whole or in part, shall be strictly prohibited.
 */
/* MediaTek Inc. (C) 2010. All rights reserved.
 *
 * BY OPENING THIS FILE, RECEIVER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
 * THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("MEDIATEK SOFTWARE")
 * RECEIVED FROM MEDIATEK AND/OR ITS REPRESENTATIVES ARE PROVIDED TO RECEIVER ON
 * AN "AS-IS" BASIS ONLY. MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR NONINFRINGEMENT.
 * NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY WHATSOEVER WITH RESPECT TO THE
 * SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY, INCORPORATED IN, OR
 * SUPPLIED WITH THE MEDIATEK SOFTWARE, AND RECEIVER AGREES TO LOOK ONLY TO SUCH
 * THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO. RECEIVER EXPRESSLY ACKNOWLEDGES
 * THAT IT IS RECEIVER'S SOLE RESPONSIBILITY TO OBTAIN FROM ANY THIRD PARTY ALL PROPER LICENSES
 * CONTAINED IN MEDIATEK SOFTWARE. MEDIATEK SHALL ALSO NOT BE RESPONSIBLE FOR ANY MEDIATEK
 * SOFTWARE RELEASES MADE TO RECEIVER'S SPECIFICATION OR TO CONFORM TO A PARTICULAR
 * STANDARD OR OPEN FORUM. RECEIVER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S ENTIRE AND
 * CUMULATIVE LIABILITY WITH RESPECT TO THE MEDIATEK SOFTWARE RELEASED HEREUNDER WILL BE,
 * AT MEDIATEK'S OPTION, TO REVISE OR REPLACE THE MEDIATEK SOFTWARE AT ISSUE,
 * OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE CHARGE PAID BY RECEIVER TO
 * MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE.
 */

/*
 * The USB Monitor, inspired by Dave Harding's USBMon.
 *
 * This is the 's' or 'stat' reader which debugs usbmon itself.
 * Note that this code blows through locks, so make sure that
 * /dbg/usbmon/0s is well protected from non-root users.
 *
 */

#include <linux/kernel.h>
#include <linux/usb.h>
#include <linux/fs.h>
#include <asm/uaccess.h>

#include "usb_mon.h"

#define STAT_BUF_SIZE  80

struct snap {
	int slen;
	char str[STAT_BUF_SIZE];
};

static int mon_stat_open(struct inode *inode, struct file *file)
{
	struct mon_bus *mbus;
	struct snap *sp;

	if ((sp = kmalloc(sizeof(struct snap), GFP_KERNEL)) == NULL)
		return -ENOMEM;

	mbus = inode->i_private;

	sp->slen = snprintf(sp->str, STAT_BUF_SIZE,
	    "nreaders %d events %u text_lost %u\n",
	    mbus->nreaders, mbus->cnt_events, mbus->cnt_text_lost);

	file->private_data = sp;
	return 0;
}

static ssize_t mon_stat_read(struct file *file, char __user *buf,
				size_t nbytes, loff_t *ppos)
{
	struct snap *sp = file->private_data;

	return simple_read_from_buffer(buf, nbytes, ppos, sp->str, sp->slen);
}

static int mon_stat_release(struct inode *inode, struct file *file)
{
	struct snap *sp = file->private_data;
	file->private_data = NULL;
	kfree(sp);
	return 0;
}

const struct file_operations mon_fops_stat = {
	.owner =	THIS_MODULE,
	.open =		mon_stat_open,
	.llseek =	no_llseek,
	.read =		mon_stat_read,
	/* .write =	mon_stat_write, */
	/* .poll =		mon_stat_poll, */
	/* .ioctl =	mon_stat_ioctl, */
	.release =	mon_stat_release,
};
