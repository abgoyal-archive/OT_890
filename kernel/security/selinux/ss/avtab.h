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
 * An access vector table (avtab) is a hash table
 * of access vectors and transition types indexed
 * by a type pair and a class.  An access vector
 * table is used to represent the type enforcement
 * tables.
 *
 *  Author : Stephen Smalley, <sds@epoch.ncsc.mil>
 */

/* Updated: Frank Mayer <mayerf@tresys.com> and Karl MacMillan <kmacmillan@tresys.com>
 *
 * 	Added conditional policy language extensions
 *
 * Copyright (C) 2003 Tresys Technology, LLC
 *	This program is free software; you can redistribute it and/or modify
 *  	it under the terms of the GNU General Public License as published by
 *	the Free Software Foundation, version 2.
 *
 * Updated: Yuichi Nakamura <ynakam@hitachisoft.jp>
 * 	Tuned number of hash slots for avtab to reduce memory usage
 */
#ifndef _SS_AVTAB_H_
#define _SS_AVTAB_H_

struct avtab_key {
	u16 source_type;	/* source type */
	u16 target_type;	/* target type */
	u16 target_class;	/* target object class */
#define AVTAB_ALLOWED     1
#define AVTAB_AUDITALLOW  2
#define AVTAB_AUDITDENY   4
#define AVTAB_AV         (AVTAB_ALLOWED | AVTAB_AUDITALLOW | AVTAB_AUDITDENY)
#define AVTAB_TRANSITION 16
#define AVTAB_MEMBER     32
#define AVTAB_CHANGE     64
#define AVTAB_TYPE       (AVTAB_TRANSITION | AVTAB_MEMBER | AVTAB_CHANGE)
#define AVTAB_ENABLED_OLD    0x80000000 /* reserved for used in cond_avtab */
#define AVTAB_ENABLED    0x8000 /* reserved for used in cond_avtab */
	u16 specified;	/* what field is specified */
};

struct avtab_datum {
	u32 data; /* access vector or type value */
};

struct avtab_node {
	struct avtab_key key;
	struct avtab_datum datum;
	struct avtab_node *next;
};

struct avtab {
	struct avtab_node **htable;
	u32 nel;	/* number of elements */
	u32 nslot;      /* number of hash slots */
	u16 mask;       /* mask to compute hash func */

};

int avtab_init(struct avtab *);
int avtab_alloc(struct avtab *, u32);
struct avtab_datum *avtab_search(struct avtab *h, struct avtab_key *k);
void avtab_destroy(struct avtab *h);
void avtab_hash_eval(struct avtab *h, char *tag);

struct policydb;
int avtab_read_item(struct avtab *a, void *fp, struct policydb *pol,
		    int (*insert)(struct avtab *a, struct avtab_key *k,
				  struct avtab_datum *d, void *p),
		    void *p);

int avtab_read(struct avtab *a, void *fp, struct policydb *pol);

struct avtab_node *avtab_insert_nonunique(struct avtab *h, struct avtab_key *key,
					  struct avtab_datum *datum);

struct avtab_node *avtab_search_node(struct avtab *h, struct avtab_key *key);

struct avtab_node *avtab_search_node_next(struct avtab_node *node, int specified);

void avtab_cache_init(void);
void avtab_cache_destroy(void);

#define MAX_AVTAB_HASH_BITS 13
#define MAX_AVTAB_HASH_BUCKETS (1 << MAX_AVTAB_HASH_BITS)
#define MAX_AVTAB_HASH_MASK (MAX_AVTAB_HASH_BUCKETS-1)
#define MAX_AVTAB_SIZE MAX_AVTAB_HASH_BUCKETS

#endif	/* _SS_AVTAB_H_ */

