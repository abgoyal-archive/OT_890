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
 *  linux/fs/proc/kmsg.c
 *
 *  Copyright (C) 1992  by Linus Torvalds
 *
 */

#include <linux/types.h>
#include <linux/errno.h>
#include <linux/time.h>
#include <linux/kernel.h>
#include <linux/poll.h>
#include <linux/proc_fs.h>
#include <linux/fs.h>

#include <asm/uaccess.h>
#include <asm/io.h>

extern wait_queue_head_t log_wait;

extern int do_syslog(int type, char __user *bug, int count);

static int kmsg_open(struct inode * inode, struct file * file)
{
	return do_syslog(1,NULL,0);
}

static int kmsg_release(struct inode * inode, struct file * file)
{
	(void) do_syslog(0,NULL,0);
	return 0;
}

static ssize_t kmsg_read(struct file *file, char __user *buf,
			 size_t count, loff_t *ppos)
{
	if ((file->f_flags & O_NONBLOCK) && !do_syslog(9, NULL, 0))
		return -EAGAIN;
	return do_syslog(2, buf, count);
}

static unsigned int kmsg_poll(struct file *file, poll_table *wait)
{
	poll_wait(file, &log_wait, wait);
	if (do_syslog(9, NULL, 0))
		return POLLIN | POLLRDNORM;
	return 0;
}


static const struct file_operations proc_kmsg_operations = {
	.read		= kmsg_read,
	.poll		= kmsg_poll,
	.open		= kmsg_open,
	.release	= kmsg_release,
};

static int __init proc_kmsg_init(void)
{
	proc_create("kmsg", S_IRUSR, NULL, &proc_kmsg_operations);
	return 0;
}
module_init(proc_kmsg_init);
