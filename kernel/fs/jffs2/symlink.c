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
 * JFFS2 -- Journalling Flash File System, Version 2.
 *
 * Copyright © 2001-2007 Red Hat, Inc.
 *
 * Created by David Woodhouse <dwmw2@infradead.org>
 *
 * For licensing information, see the file 'LICENCE' in this directory.
 *
 */

#include <linux/kernel.h>
#include <linux/slab.h>
#include <linux/fs.h>
#include <linux/namei.h>
#include "nodelist.h"

static void *jffs2_follow_link(struct dentry *dentry, struct nameidata *nd);

const struct inode_operations jffs2_symlink_inode_operations =
{
	.readlink =	generic_readlink,
	.follow_link =	jffs2_follow_link,
	.permission =	jffs2_permission,
	.setattr =	jffs2_setattr,
	.setxattr =	jffs2_setxattr,
	.getxattr =	jffs2_getxattr,
	.listxattr =	jffs2_listxattr,
	.removexattr =	jffs2_removexattr
};

static void *jffs2_follow_link(struct dentry *dentry, struct nameidata *nd)
{
	struct jffs2_inode_info *f = JFFS2_INODE_INFO(dentry->d_inode);
	char *p = (char *)f->target;

	/*
	 * We don't acquire the f->sem mutex here since the only data we
	 * use is f->target.
	 *
	 * 1. If we are here the inode has already built and f->target has
	 * to point to the target path.
	 * 2. Nobody uses f->target (if the inode is symlink's inode). The
	 * exception is inode freeing function which frees f->target. But
	 * it can't be called while we are here and before VFS has
	 * stopped using our f->target string which we provide by means of
	 * nd_set_link() call.
	 */

	if (!p) {
		printk(KERN_ERR "jffs2_follow_link(): can't find symlink target\n");
		p = ERR_PTR(-EIO);
	}
	D1(printk(KERN_DEBUG "jffs2_follow_link(): target path is '%s'\n", (char *) f->target));

	nd_set_link(nd, p);

	/*
	 * We will unlock the f->sem mutex but VFS will use the f->target string. This is safe
	 * since the only way that may cause f->target to be changed is iput() operation.
	 * But VFS will not use f->target after iput() has been called.
	 */
	return NULL;
}

