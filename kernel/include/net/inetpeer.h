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
 *		INETPEER - A storage for permanent information about peers
 *
 *  Authors:	Andrey V. Savochkin <saw@msu.ru>
 */

#ifndef _NET_INETPEER_H
#define _NET_INETPEER_H

#include <linux/types.h>
#include <linux/init.h>
#include <linux/jiffies.h>
#include <linux/spinlock.h>
#include <asm/atomic.h>

struct inet_peer
{
	/* group together avl_left,avl_right,v4daddr to speedup lookups */
	struct inet_peer	*avl_left, *avl_right;
	__be32			v4daddr;	/* peer's address */
	__u16			avl_height;
	__u16			ip_id_count;	/* IP ID for the next packet */
	struct list_head	unused;
	__u32			dtime;		/* the time of last use of not
						 * referenced entries */
	atomic_t		refcnt;
	atomic_t		rid;		/* Frag reception counter */
	__u32			tcp_ts;
	unsigned long		tcp_ts_stamp;
};

void			inet_initpeers(void) __init;

/* can be called with or without local BH being disabled */
struct inet_peer	*inet_getpeer(__be32 daddr, int create);

/* can be called from BH context or outside */
extern void inet_putpeer(struct inet_peer *p);

extern spinlock_t inet_peer_idlock;
/* can be called with or without local BH being disabled */
static inline __u16	inet_getid(struct inet_peer *p, int more)
{
	__u16 id;

	spin_lock_bh(&inet_peer_idlock);
	id = p->ip_id_count;
	p->ip_id_count += 1 + more;
	spin_unlock_bh(&inet_peer_idlock);
	return id;
}

#endif /* _NET_INETPEER_H */
