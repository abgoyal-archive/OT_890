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
 *  xenfs.c - a filesystem for passing info between the a domain and
 *  the hypervisor.
 *
 * 2008-10-07  Alex Zeffertt    Replaced /proc/xen/xenbus with xenfs filesystem
 *                              and /proc/xen compatibility mount point.
 *                              Turned xenfs into a loadable module.
 */

#include <linux/kernel.h>
#include <linux/errno.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/magic.h>

#include "xenfs.h"

#include <asm/xen/hypervisor.h>

MODULE_DESCRIPTION("Xen filesystem");
MODULE_LICENSE("GPL");

static int xenfs_fill_super(struct super_block *sb, void *data, int silent)
{
	static struct tree_descr xenfs_files[] = {
		[2] = {"xenbus", &xenbus_file_ops, S_IRUSR|S_IWUSR},
		{""},
	};

	return simple_fill_super(sb, XENFS_SUPER_MAGIC, xenfs_files);
}

static int xenfs_get_sb(struct file_system_type *fs_type,
			int flags, const char *dev_name,
			void *data, struct vfsmount *mnt)
{
	return get_sb_single(fs_type, flags, data, xenfs_fill_super, mnt);
}

static struct file_system_type xenfs_type = {
	.owner =	THIS_MODULE,
	.name =		"xenfs",
	.get_sb =	xenfs_get_sb,
	.kill_sb =	kill_litter_super,
};

static int __init xenfs_init(void)
{
	if (xen_pv_domain())
		return register_filesystem(&xenfs_type);

	printk(KERN_INFO "XENFS: not registering filesystem on non-xen platform\n");
	return 0;
}

static void __exit xenfs_exit(void)
{
	if (xen_pv_domain())
		unregister_filesystem(&xenfs_type);
}

module_init(xenfs_init);
module_exit(xenfs_exit);

