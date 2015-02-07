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

#ifndef _LINUX_PRIO_TREE_H
#define _LINUX_PRIO_TREE_H

/*
 * K&R 2nd ed. A8.3 somewhat obliquely hints that initial sequences of struct
 * fields with identical types should end up at the same location. We'll use
 * this until we can scrap struct raw_prio_tree_node.
 *
 * Note: all this could be done more elegantly by using unnamed union/struct
 * fields. However, gcc 2.95.3 and apparently also gcc 3.0.4 don't support this
 * language extension.
 */

struct raw_prio_tree_node {
	struct prio_tree_node	*left;
	struct prio_tree_node	*right;
	struct prio_tree_node	*parent;
};

struct prio_tree_node {
	struct prio_tree_node	*left;
	struct prio_tree_node	*right;
	struct prio_tree_node	*parent;
	unsigned long		start;
	unsigned long		last;	/* last location _in_ interval */
};

struct prio_tree_root {
	struct prio_tree_node	*prio_tree_node;
	unsigned short 		index_bits;
	unsigned short		raw;
		/*
		 * 0: nodes are of type struct prio_tree_node
		 * 1: nodes are of type raw_prio_tree_node
		 */
};

struct prio_tree_iter {
	struct prio_tree_node	*cur;
	unsigned long		mask;
	unsigned long		value;
	int			size_level;

	struct prio_tree_root	*root;
	pgoff_t			r_index;
	pgoff_t			h_index;
};

static inline void prio_tree_iter_init(struct prio_tree_iter *iter,
		struct prio_tree_root *root, pgoff_t r_index, pgoff_t h_index)
{
	iter->root = root;
	iter->r_index = r_index;
	iter->h_index = h_index;
	iter->cur = NULL;
}

#define __INIT_PRIO_TREE_ROOT(ptr, _raw)	\
do {					\
	(ptr)->prio_tree_node = NULL;	\
	(ptr)->index_bits = 1;		\
	(ptr)->raw = (_raw);		\
} while (0)

#define INIT_PRIO_TREE_ROOT(ptr)	__INIT_PRIO_TREE_ROOT(ptr, 0)
#define INIT_RAW_PRIO_TREE_ROOT(ptr)	__INIT_PRIO_TREE_ROOT(ptr, 1)

#define INIT_PRIO_TREE_NODE(ptr)				\
do {								\
	(ptr)->left = (ptr)->right = (ptr)->parent = (ptr);	\
} while (0)

#define INIT_PRIO_TREE_ITER(ptr)	\
do {					\
	(ptr)->cur = NULL;		\
	(ptr)->mask = 0UL;		\
	(ptr)->value = 0UL;		\
	(ptr)->size_level = 0;		\
} while (0)

#define prio_tree_entry(ptr, type, member) \
       ((type *)((char *)(ptr)-(unsigned long)(&((type *)0)->member)))

static inline int prio_tree_empty(const struct prio_tree_root *root)
{
	return root->prio_tree_node == NULL;
}

static inline int prio_tree_root(const struct prio_tree_node *node)
{
	return node->parent == node;
}

static inline int prio_tree_left_empty(const struct prio_tree_node *node)
{
	return node->left == node;
}

static inline int prio_tree_right_empty(const struct prio_tree_node *node)
{
	return node->right == node;
}


struct prio_tree_node *prio_tree_replace(struct prio_tree_root *root,
                struct prio_tree_node *old, struct prio_tree_node *node);
struct prio_tree_node *prio_tree_insert(struct prio_tree_root *root,
                struct prio_tree_node *node);
void prio_tree_remove(struct prio_tree_root *root, struct prio_tree_node *node);
struct prio_tree_node *prio_tree_next(struct prio_tree_iter *iter);

#define raw_prio_tree_replace(root, old, node) \
	prio_tree_replace(root, (struct prio_tree_node *) (old), \
	    (struct prio_tree_node *) (node))
#define raw_prio_tree_insert(root, node) \
	prio_tree_insert(root, (struct prio_tree_node *) (node))
#define raw_prio_tree_remove(root, node) \
	prio_tree_remove(root, (struct prio_tree_node *) (node))

#endif /* _LINUX_PRIO_TREE_H */
