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
 * sysfile.c
 *
 * Initialize, read, write, etc. system files.
 *
 * Copyright (C) 2002, 2004 Oracle.  All rights reserved.
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
#include <linux/slab.h>
#include <linux/highmem.h>

#define MLOG_MASK_PREFIX ML_INODE
#include <cluster/masklog.h>

#include "ocfs2.h"

#include "alloc.h"
#include "dir.h"
#include "inode.h"
#include "journal.h"
#include "sysfile.h"

#include "buffer_head_io.h"

static struct inode * _ocfs2_get_system_file_inode(struct ocfs2_super *osb,
						   int type,
						   u32 slot);

static inline int is_global_system_inode(int type);
static inline int is_in_system_inode_array(struct ocfs2_super *osb,
					   int type,
					   u32 slot);

static inline int is_global_system_inode(int type)
{
	return type >= OCFS2_FIRST_ONLINE_SYSTEM_INODE &&
		type <= OCFS2_LAST_GLOBAL_SYSTEM_INODE;
}

static inline int is_in_system_inode_array(struct ocfs2_super *osb,
					   int type,
					   u32 slot)
{
	return slot == osb->slot_num || is_global_system_inode(type);
}

struct inode *ocfs2_get_system_file_inode(struct ocfs2_super *osb,
					  int type,
					  u32 slot)
{
	struct inode *inode = NULL;
	struct inode **arr = NULL;

	/* avoid the lookup if cached in local system file array */
	if (is_in_system_inode_array(osb, type, slot))
		arr = &(osb->system_inodes[type]);

	if (arr && ((inode = *arr) != NULL)) {
		/* get a ref in addition to the array ref */
		inode = igrab(inode);
		BUG_ON(!inode);

		return inode;
	}

	/* this gets one ref thru iget */
	inode = _ocfs2_get_system_file_inode(osb, type, slot);

	/* add one more if putting into array for first time */
	if (arr && inode) {
		*arr = igrab(inode);
		BUG_ON(!*arr);
	}
	return inode;
}

static struct inode * _ocfs2_get_system_file_inode(struct ocfs2_super *osb,
						   int type,
						   u32 slot)
{
	char namebuf[40];
	struct inode *inode = NULL;
	u64 blkno;
	int status = 0;

	ocfs2_sprintf_system_inode_name(namebuf,
					sizeof(namebuf),
					type, slot);

	status = ocfs2_lookup_ino_from_name(osb->sys_root_inode, namebuf,
					    strlen(namebuf), &blkno);
	if (status < 0) {
		goto bail;
	}

	inode = ocfs2_iget(osb, blkno, OCFS2_FI_FLAG_SYSFILE, type);
	if (IS_ERR(inode)) {
		mlog_errno(PTR_ERR(inode));
		inode = NULL;
		goto bail;
	}
bail:

	return inode;
}

