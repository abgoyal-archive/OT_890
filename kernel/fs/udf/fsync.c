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
 * fsync.c
 *
 * PURPOSE
 *  Fsync handling routines for the OSTA-UDF(tm) filesystem.
 *
 * COPYRIGHT
 *  This file is distributed under the terms of the GNU General Public
 *  License (GPL). Copies of the GPL can be obtained from:
 *      ftp://prep.ai.mit.edu/pub/gnu/GPL
 *  Each contributing author retains all rights to their own work.
 *
 *  (C) 1999-2001 Ben Fennema
 *  (C) 1999-2000 Stelias Computing Inc
 *
 * HISTORY
 *
 *  05/22/99 blf  Created.
 */

#include "udfdecl.h"

#include <linux/fs.h>

static int udf_fsync_inode(struct inode *, int);

/*
 *	File may be NULL when we are called. Perhaps we shouldn't
 *	even pass file to fsync ?
 */

int udf_fsync_file(struct file *file, struct dentry *dentry, int datasync)
{
	struct inode *inode = dentry->d_inode;

	return udf_fsync_inode(inode, datasync);
}

static int udf_fsync_inode(struct inode *inode, int datasync)
{
	int err;

	err = sync_mapping_buffers(inode->i_mapping);
	if (!(inode->i_state & I_DIRTY))
		return err;
	if (datasync && !(inode->i_state & I_DIRTY_DATASYNC))
		return err;

	err |= udf_sync_inode(inode);

	return err ? -EIO : 0;
}
