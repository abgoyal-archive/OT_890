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
 * Copyright (C) 2007 Oracle.  All rights reserved.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public
 * License v2 as published by the Free Software Foundation.
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

#ifndef __DISKIO__
#define __DISKIO__

#define BTRFS_SUPER_INFO_OFFSET (64 * 1024)
#define BTRFS_SUPER_INFO_SIZE 4096

#define BTRFS_SUPER_MIRROR_MAX	 3
#define BTRFS_SUPER_MIRROR_SHIFT 12

static inline u64 btrfs_sb_offset(int mirror)
{
	u64 start = 16 * 1024;
	if (mirror)
		return start << (BTRFS_SUPER_MIRROR_SHIFT * mirror);
	return BTRFS_SUPER_INFO_OFFSET;
}

struct btrfs_device;
struct btrfs_fs_devices;

struct extent_buffer *read_tree_block(struct btrfs_root *root, u64 bytenr,
				      u32 blocksize, u64 parent_transid);
int readahead_tree_block(struct btrfs_root *root, u64 bytenr, u32 blocksize,
			 u64 parent_transid);
struct extent_buffer *btrfs_find_create_tree_block(struct btrfs_root *root,
						   u64 bytenr, u32 blocksize);
int clean_tree_block(struct btrfs_trans_handle *trans,
		     struct btrfs_root *root, struct extent_buffer *buf);
struct btrfs_root *open_ctree(struct super_block *sb,
			      struct btrfs_fs_devices *fs_devices,
			      char *options);
int close_ctree(struct btrfs_root *root);
int write_ctree_super(struct btrfs_trans_handle *trans,
		      struct btrfs_root *root, int max_mirrors);
struct buffer_head *btrfs_read_dev_super(struct block_device *bdev);
int btrfs_commit_super(struct btrfs_root *root);
struct extent_buffer *btrfs_find_tree_block(struct btrfs_root *root,
					    u64 bytenr, u32 blocksize);
struct btrfs_root *btrfs_lookup_fs_root(struct btrfs_fs_info *fs_info,
					u64 root_objectid);
struct btrfs_root *btrfs_read_fs_root(struct btrfs_fs_info *fs_info,
				      struct btrfs_key *location,
				      const char *name, int namelen);
struct btrfs_root *btrfs_read_fs_root_no_radix(struct btrfs_root *tree_root,
					       struct btrfs_key *location);
struct btrfs_root *btrfs_read_fs_root_no_name(struct btrfs_fs_info *fs_info,
					      struct btrfs_key *location);
int btrfs_cleanup_fs_roots(struct btrfs_fs_info *fs_info);
int btrfs_insert_dev_radix(struct btrfs_root *root,
			   struct block_device *bdev,
			   u64 device_id,
			   u64 block_start,
			   u64 num_blocks);
void btrfs_btree_balance_dirty(struct btrfs_root *root, unsigned long nr);
int btrfs_free_fs_root(struct btrfs_fs_info *fs_info, struct btrfs_root *root);
void btrfs_mark_buffer_dirty(struct extent_buffer *buf);
int btrfs_buffer_uptodate(struct extent_buffer *buf, u64 parent_transid);
int btrfs_set_buffer_uptodate(struct extent_buffer *buf);
int wait_on_tree_block_writeback(struct btrfs_root *root,
				 struct extent_buffer *buf);
int btrfs_read_buffer(struct extent_buffer *buf, u64 parent_transid);
u32 btrfs_csum_data(struct btrfs_root *root, char *data, u32 seed, size_t len);
void btrfs_csum_final(u32 crc, char *result);
int btrfs_open_device(struct btrfs_device *dev);
int btrfs_verify_block_csum(struct btrfs_root *root,
			    struct extent_buffer *buf);
int btrfs_bio_wq_end_io(struct btrfs_fs_info *info, struct bio *bio,
			int metadata);
int btrfs_wq_submit_bio(struct btrfs_fs_info *fs_info, struct inode *inode,
			int rw, struct bio *bio, int mirror_num,
			unsigned long bio_flags,
			extent_submit_bio_hook_t *submit_bio_start,
			extent_submit_bio_hook_t *submit_bio_done);

int btrfs_congested_async(struct btrfs_fs_info *info, int iodone);
unsigned long btrfs_async_submit_limit(struct btrfs_fs_info *info);
int btrfs_write_tree_block(struct extent_buffer *buf);
int btrfs_wait_tree_block_writeback(struct extent_buffer *buf);
int btrfs_free_log_root_tree(struct btrfs_trans_handle *trans,
			     struct btrfs_fs_info *fs_info);
int btrfs_init_log_root_tree(struct btrfs_trans_handle *trans,
			     struct btrfs_fs_info *fs_info);
int btrfs_add_log_tree(struct btrfs_trans_handle *trans,
		       struct btrfs_root *root);
int btree_lock_page_hook(struct page *page);


#ifdef CONFIG_DEBUG_LOCK_ALLOC
void btrfs_set_buffer_lockdep_class(struct extent_buffer *eb, int level);
#else
static inline void btrfs_set_buffer_lockdep_class(struct extent_buffer *eb,
						 int level)
{
}
#endif
#endif
