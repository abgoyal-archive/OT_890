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

/**
 * eCryptfs: Linux filesystem encryption layer
 *
 * Copyright (C) 1997-2003 Erez Zadok
 * Copyright (C) 2001-2003 Stony Brook University
 * Copyright (C) 2004-2006 International Business Machines Corp.
 *   Author(s): Michael A. Halcrow <mahalcro@us.ibm.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA
 * 02111-1307, USA.
 */

#include <linux/dcache.h>
#include <linux/namei.h>
#include <linux/mount.h>
#include <linux/fs_stack.h>
#include "ecryptfs_kernel.h"

/**
 * ecryptfs_d_revalidate - revalidate an ecryptfs dentry
 * @dentry: The ecryptfs dentry
 * @nd: The associated nameidata
 *
 * Called when the VFS needs to revalidate a dentry. This
 * is called whenever a name lookup finds a dentry in the
 * dcache. Most filesystems leave this as NULL, because all their
 * dentries in the dcache are valid.
 *
 * Returns 1 if valid, 0 otherwise.
 *
 */
static int ecryptfs_d_revalidate(struct dentry *dentry, struct nameidata *nd)
{
	struct dentry *lower_dentry = ecryptfs_dentry_to_lower(dentry);
	struct vfsmount *lower_mnt = ecryptfs_dentry_to_lower_mnt(dentry);
	struct dentry *dentry_save;
	struct vfsmount *vfsmount_save;
	int rc = 1;

	if (!lower_dentry->d_op || !lower_dentry->d_op->d_revalidate)
		goto out;
	dentry_save = nd->path.dentry;
	vfsmount_save = nd->path.mnt;
	nd->path.dentry = lower_dentry;
	nd->path.mnt = lower_mnt;
	rc = lower_dentry->d_op->d_revalidate(lower_dentry, nd);
	nd->path.dentry = dentry_save;
	nd->path.mnt = vfsmount_save;
	if (dentry->d_inode) {
		struct inode *lower_inode =
			ecryptfs_inode_to_lower(dentry->d_inode);

		fsstack_copy_attr_all(dentry->d_inode, lower_inode, NULL);
	}
out:
	return rc;
}

struct kmem_cache *ecryptfs_dentry_info_cache;

/**
 * ecryptfs_d_release
 * @dentry: The ecryptfs dentry
 *
 * Called when a dentry is really deallocated.
 */
static void ecryptfs_d_release(struct dentry *dentry)
{
	if (ecryptfs_dentry_to_private(dentry)) {
		if (ecryptfs_dentry_to_lower(dentry)) {
			dput(ecryptfs_dentry_to_lower(dentry));
			mntput(ecryptfs_dentry_to_lower_mnt(dentry));
		}
		kmem_cache_free(ecryptfs_dentry_info_cache,
				ecryptfs_dentry_to_private(dentry));
	}
	return;
}

struct dentry_operations ecryptfs_dops = {
	.d_revalidate = ecryptfs_d_revalidate,
	.d_release = ecryptfs_d_release,
};
