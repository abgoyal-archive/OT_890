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

#include <linux/vmalloc.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/mm.h>
#include <linux/slab.h>
#include <linux/sched.h>
#include <asm/uaccess.h>
#include "oss/sound_firmware.h"

static int do_mod_firmware_load(const char *fn, char **fp)
{
	struct file* filp;
	long l;
	char *dp;
	loff_t pos;

	filp = filp_open(fn, 0, 0);
	if (IS_ERR(filp))
	{
		printk(KERN_INFO "Unable to load '%s'.\n", fn);
		return 0;
	}
	l = filp->f_path.dentry->d_inode->i_size;
	if (l <= 0 || l > 131072)
	{
		printk(KERN_INFO "Invalid firmware '%s'\n", fn);
		filp_close(filp, current->files);
		return 0;
	}
	dp = vmalloc(l);
	if (dp == NULL)
	{
		printk(KERN_INFO "Out of memory loading '%s'.\n", fn);
		filp_close(filp, current->files);
		return 0;
	}
	pos = 0;
	if (vfs_read(filp, dp, l, &pos) != l)
	{
		printk(KERN_INFO "Failed to read '%s'.\n", fn);
		vfree(dp);
		filp_close(filp, current->files);
		return 0;
	}
	filp_close(filp, current->files);
	*fp = dp;
	return (int) l;
}

/**
 *	mod_firmware_load - load sound driver firmware
 *	@fn: filename
 *	@fp: return for the buffer.
 *
 *	Load the firmware for a sound module (up to 128K) into a buffer.
 *	The buffer is returned in *fp. It is allocated with vmalloc so is
 *	virtually linear and not DMAable. The caller should free it with
 *	vfree when finished.
 *
 *	The length of the buffer is returned on a successful load, the
 *	value zero on a failure.
 *
 *	Caution: This API is not recommended. Firmware should be loaded via
 *	request_firmware.
 */
 
int mod_firmware_load(const char *fn, char **fp)
{
	int r;
	mm_segment_t fs = get_fs();

	set_fs(get_ds());
	r = do_mod_firmware_load(fn, fp);
	set_fs(fs);
	return r;
}
EXPORT_SYMBOL(mod_firmware_load);

MODULE_LICENSE("GPL");
