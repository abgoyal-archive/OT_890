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
 * Copyright (c) 2000-2002,2005-2006 Silicon Graphics, Inc.
 * All Rights Reserved.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it would be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write the Free Software Foundation,
 * Inc.,  51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */
#include "xfs.h"
#include "xfs_vnodeops.h"
#include "xfs_bmap_btree.h"
#include "xfs_inode.h"

int  fs_noerr(void) { return 0; }
int  fs_nosys(void) { return ENOSYS; }
void fs_noval(void) { return; }

/*
 * note: all filemap functions return negative error codes. These
 * need to be inverted before returning to the xfs core functions.
 */
void
xfs_tosspages(
	xfs_inode_t	*ip,
	xfs_off_t	first,
	xfs_off_t	last,
	int		fiopt)
{
	struct address_space *mapping = VFS_I(ip)->i_mapping;

	if (mapping->nrpages)
		truncate_inode_pages(mapping, first);
}

int
xfs_flushinval_pages(
	xfs_inode_t	*ip,
	xfs_off_t	first,
	xfs_off_t	last,
	int		fiopt)
{
	struct address_space *mapping = VFS_I(ip)->i_mapping;
	int		ret = 0;

	if (mapping->nrpages) {
		xfs_iflags_clear(ip, XFS_ITRUNCATED);
		ret = filemap_write_and_wait(mapping);
		if (!ret)
			truncate_inode_pages(mapping, first);
	}
	return -ret;
}

int
xfs_flush_pages(
	xfs_inode_t	*ip,
	xfs_off_t	first,
	xfs_off_t	last,
	uint64_t	flags,
	int		fiopt)
{
	struct address_space *mapping = VFS_I(ip)->i_mapping;
	int		ret = 0;
	int		ret2;

	if (mapping_tagged(mapping, PAGECACHE_TAG_DIRTY)) {
		xfs_iflags_clear(ip, XFS_ITRUNCATED);
		ret = filemap_fdatawrite(mapping);
		if (flags & XFS_B_ASYNC)
			return -ret;
		ret2 = filemap_fdatawait(mapping);
		if (!ret)
			ret = ret2;
	}
	return -ret;
}

int
xfs_wait_on_pages(
	xfs_inode_t	*ip,
	xfs_off_t	first,
	xfs_off_t	last)
{
	struct address_space *mapping = VFS_I(ip)->i_mapping;

	if (mapping_tagged(mapping, PAGECACHE_TAG_WRITEBACK))
		return -filemap_fdatawait(mapping);
	return 0;
}
