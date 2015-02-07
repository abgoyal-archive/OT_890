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
 * Copyright (c) 2006-2008 Chelsio, Inc. All rights reserved.
 *
 * This software is available to you under a choice of one of two
 * licenses.  You may choose to be licensed under the terms of the GNU
 * General Public License (GPL) Version 2, available from the file
 * COPYING in the main directory of this source tree, or the
 * OpenIB.org BSD license below:
 *
 *     Redistribution and use in source and binary forms, with or
 *     without modification, are permitted provided that the following
 *     conditions are met:
 *
 *      - Redistributions of source code must retain the above
 *        copyright notice, this list of conditions and the following
 *        disclaimer.
 *
 *      - Redistributions in binary form must reproduce the above
 *        copyright notice, this list of conditions and the following
 *        disclaimer in the documentation and/or other materials
 *        provided with the distribution.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
#ifndef _CHELSIO_DEFS_H
#define _CHELSIO_DEFS_H

#include <linux/skbuff.h>
#include <net/tcp.h>

#include "t3cdev.h"

#include "cxgb3_offload.h"

#define VALIDATE_TID 1

void *cxgb_alloc_mem(unsigned long size);
void cxgb_free_mem(void *addr);
void cxgb_neigh_update(struct neighbour *neigh);
void cxgb_redirect(struct dst_entry *old, struct dst_entry *new);

/*
 * Map an ATID or STID to their entries in the corresponding TID tables.
 */
static inline union active_open_entry *atid2entry(const struct tid_info *t,
						  unsigned int atid)
{
	return &t->atid_tab[atid - t->atid_base];
}

static inline union listen_entry *stid2entry(const struct tid_info *t,
					     unsigned int stid)
{
	return &t->stid_tab[stid - t->stid_base];
}

/*
 * Find the connection corresponding to a TID.
 */
static inline struct t3c_tid_entry *lookup_tid(const struct tid_info *t,
					       unsigned int tid)
{
	struct t3c_tid_entry *t3c_tid = tid < t->ntids ?
	    &(t->tid_tab[tid]) : NULL;

	return (t3c_tid && t3c_tid->client) ? t3c_tid : NULL;
}

/*
 * Find the connection corresponding to a server TID.
 */
static inline struct t3c_tid_entry *lookup_stid(const struct tid_info *t,
						unsigned int tid)
{
	union listen_entry *e;

	if (tid < t->stid_base || tid >= t->stid_base + t->nstids)
		return NULL;

	e = stid2entry(t, tid);
	if ((void *)e->next >= (void *)t->tid_tab &&
	    (void *)e->next < (void *)&t->atid_tab[t->natids])
		return NULL;

	return &e->t3c_tid;
}

/*
 * Find the connection corresponding to an active-open TID.
 */
static inline struct t3c_tid_entry *lookup_atid(const struct tid_info *t,
						unsigned int tid)
{
	union active_open_entry *e;

	if (tid < t->atid_base || tid >= t->atid_base + t->natids)
		return NULL;

	e = atid2entry(t, tid);
	if ((void *)e->next >= (void *)t->tid_tab &&
	    (void *)e->next < (void *)&t->atid_tab[t->natids])
		return NULL;

	return &e->t3c_tid;
}

int process_rx(struct t3cdev *dev, struct sk_buff **skbs, int n);
int attach_t3cdev(struct t3cdev *dev);
void detach_t3cdev(struct t3cdev *dev);
#endif
