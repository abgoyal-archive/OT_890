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
 *  linux/fs/9p/vfs_dentry.c
 *
 * This file contians vfs dentry ops for the 9P2000 protocol.
 *
 *  Copyright (C) 2004 by Eric Van Hensbergen <ericvh@gmail.com>
 *  Copyright (C) 2002 by Ron Minnich <rminnich@lanl.gov>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License version 2
 *  as published by the Free Software Foundation.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to:
 *  Free Software Foundation
 *  51 Franklin Street, Fifth Floor
 *  Boston, MA  02111-1301  USA
 *
 */

#include <linux/module.h>
#include <linux/errno.h>
#include <linux/fs.h>
#include <linux/file.h>
#include <linux/pagemap.h>
#include <linux/stat.h>
#include <linux/string.h>
#include <linux/inet.h>
#include <linux/namei.h>
#include <linux/idr.h>
#include <linux/sched.h>
#include <net/9p/9p.h>
#include <net/9p/client.h>

#include "v9fs.h"
#include "v9fs_vfs.h"
#include "fid.h"

/**
 * v9fs_dentry_delete - called when dentry refcount equals 0
 * @dentry:  dentry in question
 *
 * By returning 1 here we should remove cacheing of unused
 * dentry components.
 *
 */

static int v9fs_dentry_delete(struct dentry *dentry)
{
	P9_DPRINTK(P9_DEBUG_VFS, " dentry: %s (%p)\n", dentry->d_name.name,
									dentry);

	return 1;
}

/**
 * v9fs_cached_dentry_delete - called when dentry refcount equals 0
 * @dentry:  dentry in question
 *
 * Only return 1 if our inode is invalid.  Only non-synthetic files
 * (ones without mtime == 0) should be calling this function.
 *
 */

static int v9fs_cached_dentry_delete(struct dentry *dentry)
{
	struct inode *inode = dentry->d_inode;
	P9_DPRINTK(P9_DEBUG_VFS, " dentry: %s (%p)\n", dentry->d_name.name,
									dentry);

	if(!inode)
		return 1;

	return 0;
}

/**
 * v9fs_dentry_release - called when dentry is going to be freed
 * @dentry:  dentry that is being release
 *
 */

void v9fs_dentry_release(struct dentry *dentry)
{
	struct v9fs_dentry *dent;
	struct p9_fid *temp, *current_fid;

	P9_DPRINTK(P9_DEBUG_VFS, " dentry: %s (%p)\n", dentry->d_name.name,
									dentry);
	dent = dentry->d_fsdata;
	if (dent) {
		list_for_each_entry_safe(current_fid, temp, &dent->fidlist,
									dlist) {
			p9_client_clunk(current_fid);
		}

		kfree(dent);
		dentry->d_fsdata = NULL;
	}
}

struct dentry_operations v9fs_cached_dentry_operations = {
	.d_delete = v9fs_cached_dentry_delete,
	.d_release = v9fs_dentry_release,
};

struct dentry_operations v9fs_dentry_operations = {
	.d_delete = v9fs_dentry_delete,
	.d_release = v9fs_dentry_release,
};
