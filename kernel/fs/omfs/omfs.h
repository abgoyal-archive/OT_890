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

#ifndef _OMFS_H
#define _OMFS_H

#include <linux/module.h>
#include <linux/fs.h>

#include "omfs_fs.h"

/* In-memory structures */
struct omfs_sb_info {
	u64 s_num_blocks;
	u64 s_bitmap_ino;
	u64 s_root_ino;
	u32 s_blocksize;
	u32 s_mirrors;
	u32 s_sys_blocksize;
	u32 s_clustersize;
	int s_block_shift;
	unsigned long **s_imap;
	int s_imap_size;
	struct mutex s_bitmap_lock;
	int s_uid;
	int s_gid;
	int s_dmask;
	int s_fmask;
};

/* convert a cluster number to a scaled block number */
static inline sector_t clus_to_blk(struct omfs_sb_info *sbi, sector_t block)
{
	return block << sbi->s_block_shift;
}

static inline struct omfs_sb_info *OMFS_SB(struct super_block *sb)
{
	return sb->s_fs_info;
}

/* bitmap.c */
extern unsigned long omfs_count_free(struct super_block *sb);
extern int omfs_allocate_block(struct super_block *sb, u64 block);
extern int omfs_allocate_range(struct super_block *sb, int min_request,
			int max_request, u64 *return_block, int *return_size);
extern int omfs_clear_range(struct super_block *sb, u64 block, int count);

/* dir.c */
extern struct file_operations omfs_dir_operations;
extern struct inode_operations omfs_dir_inops;
extern int omfs_make_empty(struct inode *inode, struct super_block *sb);
extern int omfs_is_bad(struct omfs_sb_info *sbi, struct omfs_header *header,
			u64 fsblock);

/* file.c */
extern struct file_operations omfs_file_operations;
extern struct inode_operations omfs_file_inops;
extern struct address_space_operations omfs_aops;
extern void omfs_make_empty_table(struct buffer_head *bh, int offset);
extern int omfs_shrink_inode(struct inode *inode);

/* inode.c */
extern struct inode *omfs_iget(struct super_block *sb, ino_t inode);
extern struct inode *omfs_new_inode(struct inode *dir, int mode);
extern int omfs_reserve_block(struct super_block *sb, sector_t block);
extern int omfs_find_empty_block(struct super_block *sb, int mode, ino_t *ino);
extern int omfs_sync_inode(struct inode *inode);

#endif
