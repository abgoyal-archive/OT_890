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
 *	klist.h - Some generic list helpers, extending struct list_head a bit.
 *
 *	Implementations are found in lib/klist.c
 *
 *
 *	Copyright (C) 2005 Patrick Mochel
 *
 *	This file is rleased under the GPL v2.
 */

#ifndef _LINUX_KLIST_H
#define _LINUX_KLIST_H

#include <linux/spinlock.h>
#include <linux/kref.h>
#include <linux/list.h>

struct klist_node;
struct klist {
	spinlock_t		k_lock;
	struct list_head	k_list;
	void			(*get)(struct klist_node *);
	void			(*put)(struct klist_node *);
} __attribute__ ((aligned (4)));

#define KLIST_INIT(_name, _get, _put)					\
	{ .k_lock	= __SPIN_LOCK_UNLOCKED(_name.k_lock),		\
	  .k_list	= LIST_HEAD_INIT(_name.k_list),			\
	  .get		= _get,						\
	  .put		= _put, }

#define DEFINE_KLIST(_name, _get, _put)					\
	struct klist _name = KLIST_INIT(_name, _get, _put)

extern void klist_init(struct klist *k, void (*get)(struct klist_node *),
		       void (*put)(struct klist_node *));

struct klist_node {
	void			*n_klist;	/* never access directly */
	struct list_head	n_node;
	struct kref		n_ref;
};

extern void klist_add_tail(struct klist_node *n, struct klist *k);
extern void klist_add_head(struct klist_node *n, struct klist *k);
extern void klist_add_after(struct klist_node *n, struct klist_node *pos);
extern void klist_add_before(struct klist_node *n, struct klist_node *pos);

extern void klist_del(struct klist_node *n);
extern void klist_remove(struct klist_node *n);

extern int klist_node_attached(struct klist_node *n);


struct klist_iter {
	struct klist		*i_klist;
	struct klist_node	*i_cur;
};


extern void klist_iter_init(struct klist *k, struct klist_iter *i);
extern void klist_iter_init_node(struct klist *k, struct klist_iter *i,
				 struct klist_node *n);
extern void klist_iter_exit(struct klist_iter *i);
extern struct klist_node *klist_next(struct klist_iter *i);

#endif
