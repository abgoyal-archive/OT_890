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
 * A security identifier table (sidtab) is a hash table
 * of security context structures indexed by SID value.
 *
 * Author : Stephen Smalley, <sds@epoch.ncsc.mil>
 */
#ifndef _SS_SIDTAB_H_
#define _SS_SIDTAB_H_

#include "context.h"

struct sidtab_node {
	u32 sid;		/* security identifier */
	struct context context;	/* security context structure */
	struct sidtab_node *next;
};

#define SIDTAB_HASH_BITS 7
#define SIDTAB_HASH_BUCKETS (1 << SIDTAB_HASH_BITS)
#define SIDTAB_HASH_MASK (SIDTAB_HASH_BUCKETS-1)

#define SIDTAB_SIZE SIDTAB_HASH_BUCKETS

struct sidtab {
	struct sidtab_node **htable;
	unsigned int nel;	/* number of elements */
	unsigned int next_sid;	/* next SID to allocate */
	unsigned char shutdown;
	spinlock_t lock;
};

int sidtab_init(struct sidtab *s);
int sidtab_insert(struct sidtab *s, u32 sid, struct context *context);
struct context *sidtab_search(struct sidtab *s, u32 sid);
struct context *sidtab_search_force(struct sidtab *s, u32 sid);

int sidtab_map(struct sidtab *s,
	       int (*apply) (u32 sid,
			     struct context *context,
			     void *args),
	       void *args);

int sidtab_context_to_sid(struct sidtab *s,
			  struct context *context,
			  u32 *sid);

void sidtab_hash_eval(struct sidtab *h, char *tag);
void sidtab_destroy(struct sidtab *s);
void sidtab_set(struct sidtab *dst, struct sidtab *src);
void sidtab_shutdown(struct sidtab *s);

#endif	/* _SS_SIDTAB_H_ */


