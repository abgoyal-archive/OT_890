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

/* -*- mode: c; c-basic-offset: 8; -*-
 * vim: noexpandtab sw=8 ts=8 sts=0:
 *
 * export.c
 *
 * Functions to facilitate NFS exporting
 *
 * Copyright (C) 2002, 2005 Oracle.  All rights reserved.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public
 * License along with this program; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 021110-1307, USA.
 */

#include <linux/fs.h>
#include <linux/types.h>

#define MLOG_MASK_PREFIX ML_EXPORT
#include <cluster/masklog.h>

#include "ocfs2.h"

#include "dir.h"
#include "dlmglue.h"
#include "dcache.h"
#include "export.h"
#include "inode.h"

#include "buffer_head_io.h"

struct ocfs2_inode_handle
{
	u64 ih_blkno;
	u32 ih_generation;
};

static struct dentry *ocfs2_get_dentry(struct super_block *sb,
		struct ocfs2_inode_handle *handle)
{
	struct inode *inode;
	struct dentry *result;

	mlog_entry("(0x%p, 0x%p)\n", sb, handle);

	if (handle->ih_blkno == 0) {
		mlog_errno(-ESTALE);
		return ERR_PTR(-ESTALE);
	}

	inode = ocfs2_iget(OCFS2_SB(sb), handle->ih_blkno, 0, 0);

	if (IS_ERR(inode))
		return (void *)inode;

	if (handle->ih_generation != inode->i_generation) {
		iput(inode);
		return ERR_PTR(-ESTALE);
	}

	result = d_obtain_alias(inode);
	if (!IS_ERR(result))
		result->d_op = &ocfs2_dentry_ops;

	mlog_exit_ptr(result);
	return result;
}

static struct dentry *ocfs2_get_parent(struct dentry *child)
{
	int status;
	u64 blkno;
	struct dentry *parent;
	struct inode *dir = child->d_inode;

	mlog_entry("(0x%p, '%.*s')\n", child,
		   child->d_name.len, child->d_name.name);

	mlog(0, "find parent of directory %llu\n",
	     (unsigned long long)OCFS2_I(dir)->ip_blkno);

	status = ocfs2_inode_lock(dir, NULL, 0);
	if (status < 0) {
		if (status != -ENOENT)
			mlog_errno(status);
		parent = ERR_PTR(status);
		goto bail;
	}

	status = ocfs2_lookup_ino_from_name(dir, "..", 2, &blkno);
	if (status < 0) {
		parent = ERR_PTR(-ENOENT);
		goto bail_unlock;
	}

	parent = d_obtain_alias(ocfs2_iget(OCFS2_SB(dir->i_sb), blkno, 0, 0));
	if (!IS_ERR(parent))
		parent->d_op = &ocfs2_dentry_ops;

bail_unlock:
	ocfs2_inode_unlock(dir, 0);

bail:
	mlog_exit_ptr(parent);

	return parent;
}

static int ocfs2_encode_fh(struct dentry *dentry, u32 *fh_in, int *max_len,
			   int connectable)
{
	struct inode *inode = dentry->d_inode;
	int len = *max_len;
	int type = 1;
	u64 blkno;
	u32 generation;
	__le32 *fh = (__force __le32 *) fh_in;

	mlog_entry("(0x%p, '%.*s', 0x%p, %d, %d)\n", dentry,
		   dentry->d_name.len, dentry->d_name.name,
		   fh, len, connectable);

	if (len < 3 || (connectable && len < 6)) {
		mlog(ML_ERROR, "fh buffer is too small for encoding\n");
		type = 255;
		goto bail;
	}

	blkno = OCFS2_I(inode)->ip_blkno;
	generation = inode->i_generation;

	mlog(0, "Encoding fh: blkno: %llu, generation: %u\n",
	     (unsigned long long)blkno, generation);

	len = 3;
	fh[0] = cpu_to_le32((u32)(blkno >> 32));
	fh[1] = cpu_to_le32((u32)(blkno & 0xffffffff));
	fh[2] = cpu_to_le32(generation);

	if (connectable && !S_ISDIR(inode->i_mode)) {
		struct inode *parent;

		spin_lock(&dentry->d_lock);

		parent = dentry->d_parent->d_inode;
		blkno = OCFS2_I(parent)->ip_blkno;
		generation = parent->i_generation;

		fh[3] = cpu_to_le32((u32)(blkno >> 32));
		fh[4] = cpu_to_le32((u32)(blkno & 0xffffffff));
		fh[5] = cpu_to_le32(generation);

		spin_unlock(&dentry->d_lock);

		len = 6;
		type = 2;

		mlog(0, "Encoding parent: blkno: %llu, generation: %u\n",
		     (unsigned long long)blkno, generation);
	}
	
	*max_len = len;

bail:
	mlog_exit(type);
	return type;
}

static struct dentry *ocfs2_fh_to_dentry(struct super_block *sb,
		struct fid *fid, int fh_len, int fh_type)
{
	struct ocfs2_inode_handle handle;

	if (fh_len < 3 || fh_type > 2)
		return NULL;

	handle.ih_blkno = (u64)le32_to_cpu(fid->raw[0]) << 32;
	handle.ih_blkno |= (u64)le32_to_cpu(fid->raw[1]);
	handle.ih_generation = le32_to_cpu(fid->raw[2]);
	return ocfs2_get_dentry(sb, &handle);
}

static struct dentry *ocfs2_fh_to_parent(struct super_block *sb,
		struct fid *fid, int fh_len, int fh_type)
{
	struct ocfs2_inode_handle parent;

	if (fh_type != 2 || fh_len < 6)
		return NULL;

	parent.ih_blkno = (u64)le32_to_cpu(fid->raw[3]) << 32;
	parent.ih_blkno |= (u64)le32_to_cpu(fid->raw[4]);
	parent.ih_generation = le32_to_cpu(fid->raw[5]);
	return ocfs2_get_dentry(sb, &parent);
}

const struct export_operations ocfs2_export_ops = {
	.encode_fh	= ocfs2_encode_fh,
	.fh_to_dentry	= ocfs2_fh_to_dentry,
	.fh_to_parent	= ocfs2_fh_to_parent,
	.get_parent	= ocfs2_get_parent,
};
