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
 *  linux/fs/ext2/xip.c
 *
 * Copyright (C) 2005 IBM Corporation
 * Author: Carsten Otte (cotte@de.ibm.com)
 */

#include <linux/mm.h>
#include <linux/fs.h>
#include <linux/genhd.h>
#include <linux/buffer_head.h>
#include <linux/ext2_fs_sb.h>
#include <linux/ext2_fs.h>
#include <linux/blkdev.h>
#include "ext2.h"
#include "xip.h"

static inline int
__inode_direct_access(struct inode *inode, sector_t block,
		      void **kaddr, unsigned long *pfn)
{
	struct block_device *bdev = inode->i_sb->s_bdev;
	struct block_device_operations *ops = bdev->bd_disk->fops;
	sector_t sector;

	sector = block * (PAGE_SIZE / 512); /* ext2 block to bdev sector */

	BUG_ON(!ops->direct_access);
	return ops->direct_access(bdev, sector, kaddr, pfn);
}

static inline int
__ext2_get_block(struct inode *inode, pgoff_t pgoff, int create,
		   sector_t *result)
{
	struct buffer_head tmp;
	int rc;

	memset(&tmp, 0, sizeof(struct buffer_head));
	rc = ext2_get_block(inode, pgoff, &tmp, create);
	*result = tmp.b_blocknr;

	/* did we get a sparse block (hole in the file)? */
	if (!tmp.b_blocknr && !rc) {
		BUG_ON(create);
		rc = -ENODATA;
	}

	return rc;
}

int
ext2_clear_xip_target(struct inode *inode, sector_t block)
{
	void *kaddr;
	unsigned long pfn;
	int rc;

	rc = __inode_direct_access(inode, block, &kaddr, &pfn);
	if (!rc)
		clear_page(kaddr);
	return rc;
}

void ext2_xip_verify_sb(struct super_block *sb)
{
	struct ext2_sb_info *sbi = EXT2_SB(sb);

	if ((sbi->s_mount_opt & EXT2_MOUNT_XIP) &&
	    !sb->s_bdev->bd_disk->fops->direct_access) {
		sbi->s_mount_opt &= (~EXT2_MOUNT_XIP);
		ext2_warning(sb, __func__,
			     "ignoring xip option - not supported by bdev");
	}
}

int ext2_get_xip_mem(struct address_space *mapping, pgoff_t pgoff, int create,
				void **kmem, unsigned long *pfn)
{
	int rc;
	sector_t block;

	/* first, retrieve the sector number */
	rc = __ext2_get_block(mapping->host, pgoff, create, &block);
	if (rc)
		return rc;

	/* retrieve address of the target data */
	rc = __inode_direct_access(mapping->host, block, kmem, pfn);
	return rc;
}
