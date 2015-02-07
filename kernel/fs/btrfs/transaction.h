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

#ifndef __BTRFS_TRANSACTION__
#define __BTRFS_TRANSACTION__
#include "btrfs_inode.h"

struct btrfs_transaction {
	u64 transid;
	unsigned long num_writers;
	unsigned long num_joined;
	int in_commit;
	int use_count;
	int commit_done;
	int blocked;
	struct list_head list;
	struct extent_io_tree dirty_pages;
	unsigned long start_time;
	wait_queue_head_t writer_wait;
	wait_queue_head_t commit_wait;
	struct list_head pending_snapshots;
};

struct btrfs_trans_handle {
	u64 transid;
	unsigned long blocks_reserved;
	unsigned long blocks_used;
	struct btrfs_transaction *transaction;
	u64 block_group;
	u64 alloc_exclude_start;
	u64 alloc_exclude_nr;
};

struct btrfs_pending_snapshot {
	struct dentry *dentry;
	struct btrfs_root *root;
	char *name;
	struct btrfs_key root_key;
	struct list_head list;
};

struct btrfs_dirty_root {
	struct list_head list;
	struct btrfs_root *root;
	struct btrfs_root *latest_root;
};

static inline void btrfs_set_trans_block_group(struct btrfs_trans_handle *trans,
					       struct inode *inode)
{
	trans->block_group = BTRFS_I(inode)->block_group;
}

static inline void btrfs_update_inode_block_group(
					  struct btrfs_trans_handle *trans,
					  struct inode *inode)
{
	BTRFS_I(inode)->block_group = trans->block_group;
}

static inline void btrfs_set_inode_last_trans(struct btrfs_trans_handle *trans,
					      struct inode *inode)
{
	BTRFS_I(inode)->last_trans = trans->transaction->transid;
}

int btrfs_end_transaction(struct btrfs_trans_handle *trans,
			  struct btrfs_root *root);
struct btrfs_trans_handle *btrfs_start_transaction(struct btrfs_root *root,
						   int num_blocks);
struct btrfs_trans_handle *btrfs_join_transaction(struct btrfs_root *root,
						   int num_blocks);
struct btrfs_trans_handle *btrfs_start_ioctl_transaction(struct btrfs_root *r,
						   int num_blocks);
int btrfs_write_and_wait_transaction(struct btrfs_trans_handle *trans,
				     struct btrfs_root *root);
int btrfs_commit_tree_roots(struct btrfs_trans_handle *trans,
			    struct btrfs_root *root);

int btrfs_add_dead_root(struct btrfs_root *root, struct btrfs_root *latest);
int btrfs_defrag_root(struct btrfs_root *root, int cacheonly);
int btrfs_clean_old_snapshots(struct btrfs_root *root);
int btrfs_commit_transaction(struct btrfs_trans_handle *trans,
			     struct btrfs_root *root);
int btrfs_end_transaction_throttle(struct btrfs_trans_handle *trans,
				   struct btrfs_root *root);
void btrfs_throttle(struct btrfs_root *root);
int btrfs_record_root_in_trans(struct btrfs_root *root);
int btrfs_write_and_wait_marked_extents(struct btrfs_root *root,
					struct extent_io_tree *dirty_pages);
#endif
