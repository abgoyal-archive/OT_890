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
 *  linux/fs/hfs/btree.h
 *
 * Copyright (C) 2001
 * Brad Boyer (flar@allandria.com)
 * (C) 2003 Ardis Technologies <roman@ardistech.com>
 */

#include "hfs_fs.h"

typedef int (*btree_keycmp)(const btree_key *, const btree_key *);

#define NODE_HASH_SIZE  256

/* A HFS BTree held in memory */
struct hfs_btree {
	struct super_block *sb;
	struct inode *inode;
	btree_keycmp keycmp;

	u32 cnid;
	u32 root;
	u32 leaf_count;
	u32 leaf_head;
	u32 leaf_tail;
	u32 node_count;
	u32 free_nodes;
	u32 attributes;

	unsigned int node_size;
	unsigned int node_size_shift;
	unsigned int max_key_len;
	unsigned int depth;

	//unsigned int map1_size, map_size;
	struct semaphore tree_lock;

	unsigned int pages_per_bnode;
	spinlock_t hash_lock;
	struct hfs_bnode *node_hash[NODE_HASH_SIZE];
	int node_hash_cnt;
};

/* A HFS BTree node in memory */
struct hfs_bnode {
	struct hfs_btree *tree;

	u32 prev;
	u32 this;
	u32 next;
	u32 parent;

	u16 num_recs;
	u8 type;
	u8 height;

	struct hfs_bnode *next_hash;
	unsigned long flags;
	wait_queue_head_t lock_wq;
	atomic_t refcnt;
	unsigned int page_offset;
	struct page *page[0];
};

#define HFS_BNODE_ERROR		0
#define HFS_BNODE_NEW		1
#define HFS_BNODE_DELETED	2

struct hfs_find_data {
	btree_key *key;
	btree_key *search_key;
	struct hfs_btree *tree;
	struct hfs_bnode *bnode;
	int record;
	int keyoffset, keylength;
	int entryoffset, entrylength;
};


/* btree.c */
extern struct hfs_btree *hfs_btree_open(struct super_block *, u32, btree_keycmp);
extern void hfs_btree_close(struct hfs_btree *);
extern void hfs_btree_write(struct hfs_btree *);
extern struct hfs_bnode * hfs_bmap_alloc(struct hfs_btree *);
extern void hfs_bmap_free(struct hfs_bnode *node);

/* bnode.c */
extern void hfs_bnode_read(struct hfs_bnode *, void *, int, int);
extern u16 hfs_bnode_read_u16(struct hfs_bnode *, int);
extern u8 hfs_bnode_read_u8(struct hfs_bnode *, int);
extern void hfs_bnode_read_key(struct hfs_bnode *, void *, int);
extern void hfs_bnode_write(struct hfs_bnode *, void *, int, int);
extern void hfs_bnode_write_u16(struct hfs_bnode *, int, u16);
extern void hfs_bnode_write_u8(struct hfs_bnode *, int, u8);
extern void hfs_bnode_clear(struct hfs_bnode *, int, int);
extern void hfs_bnode_copy(struct hfs_bnode *, int,
			   struct hfs_bnode *, int, int);
extern void hfs_bnode_move(struct hfs_bnode *, int, int, int);
extern void hfs_bnode_dump(struct hfs_bnode *);
extern void hfs_bnode_unlink(struct hfs_bnode *);
extern struct hfs_bnode *hfs_bnode_findhash(struct hfs_btree *, u32);
extern struct hfs_bnode *hfs_bnode_find(struct hfs_btree *, u32);
extern void hfs_bnode_unhash(struct hfs_bnode *);
extern void hfs_bnode_free(struct hfs_bnode *);
extern struct hfs_bnode *hfs_bnode_create(struct hfs_btree *, u32);
extern void hfs_bnode_get(struct hfs_bnode *);
extern void hfs_bnode_put(struct hfs_bnode *);

/* brec.c */
extern u16 hfs_brec_lenoff(struct hfs_bnode *, u16, u16 *);
extern u16 hfs_brec_keylen(struct hfs_bnode *, u16);
extern int hfs_brec_insert(struct hfs_find_data *, void *, int);
extern int hfs_brec_remove(struct hfs_find_data *);

/* bfind.c */
extern int hfs_find_init(struct hfs_btree *, struct hfs_find_data *);
extern void hfs_find_exit(struct hfs_find_data *);
extern int __hfs_brec_find(struct hfs_bnode *, struct hfs_find_data *);
extern int hfs_brec_find(struct hfs_find_data *);
extern int hfs_brec_read(struct hfs_find_data *, void *, int);
extern int hfs_brec_goto(struct hfs_find_data *, int);


struct hfs_bnode_desc {
	__be32 next;		/* (V) Number of the next node at this level */
	__be32 prev;		/* (V) Number of the prev node at this level */
	u8 type;		/* (F) The type of node */
	u8 height;		/* (F) The level of this node (leaves=1) */
	__be16 num_recs;	/* (V) The number of records in this node */
	u16 reserved;
} __packed;

#define HFS_NODE_INDEX	0x00	/* An internal (index) node */
#define HFS_NODE_HEADER	0x01	/* The tree header node (node 0) */
#define HFS_NODE_MAP	0x02	/* Holds part of the bitmap of used nodes */
#define HFS_NODE_LEAF	0xFF	/* A leaf (ndNHeight==1) node */

struct hfs_btree_header_rec {
	__be16 depth;		/* (V) The number of levels in this B-tree */
	__be32 root;		/* (V) The node number of the root node */
	__be32 leaf_count;	/* (V) The number of leaf records */
	__be32 leaf_head;	/* (V) The number of the first leaf node */
	__be32 leaf_tail;	/* (V) The number of the last leaf node */
	__be16 node_size;	/* (F) The number of bytes in a node (=512) */
	__be16 max_key_len;	/* (F) The length of a key in an index node */
	__be32 node_count;	/* (V) The total number of nodes */
	__be32 free_nodes;	/* (V) The number of unused nodes */
	u16 reserved1;
	__be32 clump_size;	/* (F) clump size. not usually used. */
	u8 btree_type;		/* (F) BTree type */
	u8 reserved2;
	__be32 attributes;	/* (F) attributes */
	u32 reserved3[16];
} __packed;

#define HFS_NODE_INDEX	0x00	/* An internal (index) node */
#define HFS_NODE_HEADER	0x01	/* The tree header node (node 0) */
#define HFS_NODE_MAP		0x02	/* Holds part of the bitmap of used nodes */
#define HFS_NODE_LEAF		0xFF	/* A leaf (ndNHeight==1) node */

#define BTREE_ATTR_BADCLOSE	0x00000001	/* b-tree not closed properly. not
						   used by hfsplus. */
#define HFS_TREE_BIGKEYS	0x00000002	/* key length is u16 instead of u8.
						   used by hfsplus. */
#define HFS_TREE_VARIDXKEYS	0x00000004	/* variable key length instead of
						   max key length. use din catalog
						   b-tree but not in extents
						   b-tree (hfsplus). */
