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
 * An extensible bitmap is a bitmap that supports an
 * arbitrary number of bits.  Extensible bitmaps are
 * used to represent sets of values, such as types,
 * roles, categories, and classes.
 *
 * Each extensible bitmap is implemented as a linked
 * list of bitmap nodes, where each bitmap node has
 * an explicitly specified starting bit position within
 * the total bitmap.
 *
 * Author : Stephen Smalley, <sds@epoch.ncsc.mil>
 */
#ifndef _SS_EBITMAP_H_
#define _SS_EBITMAP_H_

#include <net/netlabel.h>

#define EBITMAP_UNIT_NUMS	((32 - sizeof(void *) - sizeof(u32))	\
					/ sizeof(unsigned long))
#define EBITMAP_UNIT_SIZE	BITS_PER_LONG
#define EBITMAP_SIZE		(EBITMAP_UNIT_NUMS * EBITMAP_UNIT_SIZE)
#define EBITMAP_BIT		1ULL
#define EBITMAP_SHIFT_UNIT_SIZE(x)					\
	(((x) >> EBITMAP_UNIT_SIZE / 2) >> EBITMAP_UNIT_SIZE / 2)

struct ebitmap_node {
	struct ebitmap_node *next;
	unsigned long maps[EBITMAP_UNIT_NUMS];
	u32 startbit;
};

struct ebitmap {
	struct ebitmap_node *node;	/* first node in the bitmap */
	u32 highbit;	/* highest position in the total bitmap */
};

#define ebitmap_length(e) ((e)->highbit)
#define ebitmap_startbit(e) ((e)->node ? (e)->node->startbit : 0)

static inline unsigned int ebitmap_start_positive(struct ebitmap *e,
						  struct ebitmap_node **n)
{
	unsigned int ofs;

	for (*n = e->node; *n; *n = (*n)->next) {
		ofs = find_first_bit((*n)->maps, EBITMAP_SIZE);
		if (ofs < EBITMAP_SIZE)
			return (*n)->startbit + ofs;
	}
	return ebitmap_length(e);
}

static inline void ebitmap_init(struct ebitmap *e)
{
	memset(e, 0, sizeof(*e));
}

static inline unsigned int ebitmap_next_positive(struct ebitmap *e,
						 struct ebitmap_node **n,
						 unsigned int bit)
{
	unsigned int ofs;

	ofs = find_next_bit((*n)->maps, EBITMAP_SIZE, bit - (*n)->startbit + 1);
	if (ofs < EBITMAP_SIZE)
		return ofs + (*n)->startbit;

	for (*n = (*n)->next; *n; *n = (*n)->next) {
		ofs = find_first_bit((*n)->maps, EBITMAP_SIZE);
		if (ofs < EBITMAP_SIZE)
			return ofs + (*n)->startbit;
	}
	return ebitmap_length(e);
}

#define EBITMAP_NODE_INDEX(node, bit)	\
	(((bit) - (node)->startbit) / EBITMAP_UNIT_SIZE)
#define EBITMAP_NODE_OFFSET(node, bit)	\
	(((bit) - (node)->startbit) % EBITMAP_UNIT_SIZE)

static inline int ebitmap_node_get_bit(struct ebitmap_node *n,
				       unsigned int bit)
{
	unsigned int index = EBITMAP_NODE_INDEX(n, bit);
	unsigned int ofs = EBITMAP_NODE_OFFSET(n, bit);

	BUG_ON(index >= EBITMAP_UNIT_NUMS);
	if ((n->maps[index] & (EBITMAP_BIT << ofs)))
		return 1;
	return 0;
}

static inline void ebitmap_node_set_bit(struct ebitmap_node *n,
					unsigned int bit)
{
	unsigned int index = EBITMAP_NODE_INDEX(n, bit);
	unsigned int ofs = EBITMAP_NODE_OFFSET(n, bit);

	BUG_ON(index >= EBITMAP_UNIT_NUMS);
	n->maps[index] |= (EBITMAP_BIT << ofs);
}

static inline void ebitmap_node_clr_bit(struct ebitmap_node *n,
					unsigned int bit)
{
	unsigned int index = EBITMAP_NODE_INDEX(n, bit);
	unsigned int ofs = EBITMAP_NODE_OFFSET(n, bit);

	BUG_ON(index >= EBITMAP_UNIT_NUMS);
	n->maps[index] &= ~(EBITMAP_BIT << ofs);
}

#define ebitmap_for_each_positive_bit(e, n, bit)	\
	for (bit = ebitmap_start_positive(e, &n);	\
	     bit < ebitmap_length(e);			\
	     bit = ebitmap_next_positive(e, &n, bit))	\

int ebitmap_cmp(struct ebitmap *e1, struct ebitmap *e2);
int ebitmap_cpy(struct ebitmap *dst, struct ebitmap *src);
int ebitmap_contains(struct ebitmap *e1, struct ebitmap *e2);
int ebitmap_get_bit(struct ebitmap *e, unsigned long bit);
int ebitmap_set_bit(struct ebitmap *e, unsigned long bit, int value);
void ebitmap_destroy(struct ebitmap *e);
int ebitmap_read(struct ebitmap *e, void *fp);

#ifdef CONFIG_NETLABEL
int ebitmap_netlbl_export(struct ebitmap *ebmap,
			  struct netlbl_lsm_secattr_catmap **catmap);
int ebitmap_netlbl_import(struct ebitmap *ebmap,
			  struct netlbl_lsm_secattr_catmap *catmap);
#else
static inline int ebitmap_netlbl_export(struct ebitmap *ebmap,
				struct netlbl_lsm_secattr_catmap **catmap)
{
	return -ENOMEM;
}
static inline int ebitmap_netlbl_import(struct ebitmap *ebmap,
				struct netlbl_lsm_secattr_catmap *catmap)
{
	return -ENOMEM;
}
#endif

#endif	/* _SS_EBITMAP_H_ */
