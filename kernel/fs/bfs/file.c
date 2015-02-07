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
 *	fs/bfs/file.c
 *	BFS file operations.
 *	Copyright (C) 1999,2000 Tigran Aivazian <tigran@veritas.com>
 *
 *	Make the file block allocation algorithm understand the size
 *	of the underlying block device.
 *	Copyright (C) 2007 Dmitri Vorobiev <dmitri.vorobiev@gmail.com>
 *
 */

#include <linux/fs.h>
#include <linux/buffer_head.h>
#include <linux/smp_lock.h>
#include "bfs.h"

#undef DEBUG

#ifdef DEBUG
#define dprintf(x...)	printf(x)
#else
#define dprintf(x...)
#endif

const struct file_operations bfs_file_operations = {
	.llseek 	= generic_file_llseek,
	.read		= do_sync_read,
	.aio_read	= generic_file_aio_read,
	.write		= do_sync_write,
	.aio_write	= generic_file_aio_write,
	.mmap		= generic_file_mmap,
	.splice_read	= generic_file_splice_read,
};

static int bfs_move_block(unsigned long from, unsigned long to,
					struct super_block *sb)
{
	struct buffer_head *bh, *new;

	bh = sb_bread(sb, from);
	if (!bh)
		return -EIO;
	new = sb_getblk(sb, to);
	memcpy(new->b_data, bh->b_data, bh->b_size);
	mark_buffer_dirty(new);
	bforget(bh);
	brelse(new);
	return 0;
}

static int bfs_move_blocks(struct super_block *sb, unsigned long start,
				unsigned long end, unsigned long where)
{
	unsigned long i;

	dprintf("%08lx-%08lx->%08lx\n", start, end, where);
	for (i = start; i <= end; i++)
		if(bfs_move_block(i, where + i, sb)) {
			dprintf("failed to move block %08lx -> %08lx\n", i,
								where + i);
			return -EIO;
		}
	return 0;
}

static int bfs_get_block(struct inode *inode, sector_t block,
			struct buffer_head *bh_result, int create)
{
	unsigned long phys;
	int err;
	struct super_block *sb = inode->i_sb;
	struct bfs_sb_info *info = BFS_SB(sb);
	struct bfs_inode_info *bi = BFS_I(inode);
	struct buffer_head *sbh = info->si_sbh;

	phys = bi->i_sblock + block;
	if (!create) {
		if (phys <= bi->i_eblock) {
			dprintf("c=%d, b=%08lx, phys=%09lx (granted)\n",
                                create, (unsigned long)block, phys);
			map_bh(bh_result, sb, phys);
		}
		return 0;
	}

	/*
	 * If the file is not empty and the requested block is within the
	 * range of blocks allocated for this file, we can grant it.
	 */
	if (bi->i_sblock && (phys <= bi->i_eblock)) {
		dprintf("c=%d, b=%08lx, phys=%08lx (interim block granted)\n", 
				create, (unsigned long)block, phys);
		map_bh(bh_result, sb, phys);
		return 0;
	}

	/* The file will be extended, so let's see if there is enough space. */
	if (phys >= info->si_blocks)
		return -ENOSPC;

	/* The rest has to be protected against itself. */
	mutex_lock(&info->bfs_lock);

	/*
	 * If the last data block for this file is the last allocated
	 * block, we can extend the file trivially, without moving it
	 * anywhere.
	 */
	if (bi->i_eblock == info->si_lf_eblk) {
		dprintf("c=%d, b=%08lx, phys=%08lx (simple extension)\n", 
				create, (unsigned long)block, phys);
		map_bh(bh_result, sb, phys);
		info->si_freeb -= phys - bi->i_eblock;
		info->si_lf_eblk = bi->i_eblock = phys;
		mark_inode_dirty(inode);
		mark_buffer_dirty(sbh);
		err = 0;
		goto out;
	}

	/* Ok, we have to move this entire file to the next free block. */
	phys = info->si_lf_eblk + 1;
	if (phys + block >= info->si_blocks) {
		err = -ENOSPC;
		goto out;
	}

	if (bi->i_sblock) {
		err = bfs_move_blocks(inode->i_sb, bi->i_sblock, 
						bi->i_eblock, phys);
		if (err) {
			dprintf("failed to move ino=%08lx -> fs corruption\n",
								inode->i_ino);
			goto out;
		}
	} else
		err = 0;

	dprintf("c=%d, b=%08lx, phys=%08lx (moved)\n",
                create, (unsigned long)block, phys);
	bi->i_sblock = phys;
	phys += block;
	info->si_lf_eblk = bi->i_eblock = phys;

	/*
	 * This assumes nothing can write the inode back while we are here
	 * and thus update inode->i_blocks! (XXX)
	 */
	info->si_freeb -= bi->i_eblock - bi->i_sblock + 1 - inode->i_blocks;
	mark_inode_dirty(inode);
	mark_buffer_dirty(sbh);
	map_bh(bh_result, sb, phys);
out:
	mutex_unlock(&info->bfs_lock);
	return err;
}

static int bfs_writepage(struct page *page, struct writeback_control *wbc)
{
	return block_write_full_page(page, bfs_get_block, wbc);
}

static int bfs_readpage(struct file *file, struct page *page)
{
	return block_read_full_page(page, bfs_get_block);
}

static int bfs_write_begin(struct file *file, struct address_space *mapping,
			loff_t pos, unsigned len, unsigned flags,
			struct page **pagep, void **fsdata)
{
	*pagep = NULL;
	return block_write_begin(file, mapping, pos, len, flags,
					pagep, fsdata, bfs_get_block);
}

static sector_t bfs_bmap(struct address_space *mapping, sector_t block)
{
	return generic_block_bmap(mapping, block, bfs_get_block);
}

const struct address_space_operations bfs_aops = {
	.readpage	= bfs_readpage,
	.writepage	= bfs_writepage,
	.sync_page	= block_sync_page,
	.write_begin	= bfs_write_begin,
	.write_end	= generic_write_end,
	.bmap		= bfs_bmap,
};

const struct inode_operations bfs_file_inops;
