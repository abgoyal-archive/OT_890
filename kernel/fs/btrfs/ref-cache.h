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
 * Copyright (C) 2008 Oracle.  All rights reserved.
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
#ifndef __REFCACHE__
#define __REFCACHE__

struct btrfs_extent_info {
	/* bytenr and num_bytes find the extent in the extent allocation tree */
	u64 bytenr;
	u64 num_bytes;

	/* objectid and offset find the back reference for the file */
	u64 objectid;
	u64 offset;
};

struct btrfs_leaf_ref {
	struct rb_node rb_node;
	struct btrfs_leaf_ref_tree *tree;
	int in_tree;
	atomic_t usage;

	u64 root_gen;
	u64 bytenr;
	u64 owner;
	u64 generation;
	int nritems;

	struct list_head list;
	struct btrfs_extent_info extents[];
};

static inline size_t btrfs_leaf_ref_size(int nr_extents)
{
	return sizeof(struct btrfs_leaf_ref) +
	       sizeof(struct btrfs_extent_info) * nr_extents;
}

static inline void btrfs_leaf_ref_tree_init(struct btrfs_leaf_ref_tree *tree)
{
	tree->root.rb_node = NULL;
	INIT_LIST_HEAD(&tree->list);
	spin_lock_init(&tree->lock);
}

static inline int btrfs_leaf_ref_tree_empty(struct btrfs_leaf_ref_tree *tree)
{
	return RB_EMPTY_ROOT(&tree->root);
}

void btrfs_leaf_ref_tree_init(struct btrfs_leaf_ref_tree *tree);
struct btrfs_leaf_ref *btrfs_alloc_leaf_ref(struct btrfs_root *root,
					    int nr_extents);
void btrfs_free_leaf_ref(struct btrfs_root *root, struct btrfs_leaf_ref *ref);
struct btrfs_leaf_ref *btrfs_lookup_leaf_ref(struct btrfs_root *root,
					     u64 bytenr);
int btrfs_add_leaf_ref(struct btrfs_root *root, struct btrfs_leaf_ref *ref,
		       int shared);
int btrfs_remove_leaf_refs(struct btrfs_root *root, u64 max_root_gen,
			   int shared);
int btrfs_remove_leaf_ref(struct btrfs_root *root, struct btrfs_leaf_ref *ref);
#endif
