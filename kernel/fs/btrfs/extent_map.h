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

#ifndef __EXTENTMAP__
#define __EXTENTMAP__

#include <linux/rbtree.h>

#define EXTENT_MAP_LAST_BYTE (u64)-4
#define EXTENT_MAP_HOLE (u64)-3
#define EXTENT_MAP_INLINE (u64)-2
#define EXTENT_MAP_DELALLOC (u64)-1

/* bits for the flags field */
#define EXTENT_FLAG_PINNED 0 /* this entry not yet on disk, don't free it */
#define EXTENT_FLAG_COMPRESSED 1
#define EXTENT_FLAG_VACANCY 2 /* no file extent item found */
#define EXTENT_FLAG_PREALLOC 3 /* pre-allocated extent */

struct extent_map {
	struct rb_node rb_node;

	/* all of these are in bytes */
	u64 start;
	u64 len;
	u64 orig_start;
	u64 block_start;
	u64 block_len;
	unsigned long flags;
	struct block_device *bdev;
	atomic_t refs;
	int in_tree;
};

struct extent_map_tree {
	struct rb_root map;
	spinlock_t lock;
};

static inline u64 extent_map_end(struct extent_map *em)
{
	if (em->start + em->len < em->start)
		return (u64)-1;
	return em->start + em->len;
}

static inline u64 extent_map_block_end(struct extent_map *em)
{
	if (em->block_start + em->block_len < em->block_start)
		return (u64)-1;
	return em->block_start + em->block_len;
}

void extent_map_tree_init(struct extent_map_tree *tree, gfp_t mask);
struct extent_map *lookup_extent_mapping(struct extent_map_tree *tree,
					 u64 start, u64 len);
int add_extent_mapping(struct extent_map_tree *tree,
		       struct extent_map *em);
int remove_extent_mapping(struct extent_map_tree *tree, struct extent_map *em);

struct extent_map *alloc_extent_map(gfp_t mask);
void free_extent_map(struct extent_map *em);
int __init extent_map_init(void);
void extent_map_exit(void);
#endif
