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

/* net/atm/atmarp.h - RFC1577 ATM ARP */
 
/* Written 1995-2000 by Werner Almesberger, EPFL LRC/ICA */
 
 
#ifndef _ATMCLIP_H
#define _ATMCLIP_H

#include <linux/netdevice.h>
#include <linux/atm.h>
#include <linux/atmdev.h>
#include <linux/atmarp.h>
#include <linux/spinlock.h>
#include <net/neighbour.h>


#define CLIP_VCC(vcc) ((struct clip_vcc *) ((vcc)->user_back))
#define NEIGH2ENTRY(neigh) ((struct atmarp_entry *) (neigh)->primary_key)

struct sk_buff;

struct clip_vcc {
	struct atm_vcc	*vcc;		/* VCC descriptor */
	struct atmarp_entry *entry;	/* ATMARP table entry, NULL if IP addr.
					   isn't known yet */
	int		xoff;		/* 1 if send buffer is full */
	unsigned char	encap;		/* 0: NULL, 1: LLC/SNAP */
	unsigned long	last_use;	/* last send or receive operation */
	unsigned long	idle_timeout;	/* keep open idle for so many jiffies*/
	void (*old_push)(struct atm_vcc *vcc,struct sk_buff *skb);
					/* keep old push fn for chaining */
	void (*old_pop)(struct atm_vcc *vcc,struct sk_buff *skb);
					/* keep old pop fn for chaining */
	struct clip_vcc	*next;		/* next VCC */
};


struct atmarp_entry {
	__be32		ip;		/* IP address */
	struct clip_vcc	*vccs;		/* active VCCs; NULL if resolution is
					   pending */
	unsigned long	expires;	/* entry expiration time */
	struct neighbour *neigh;	/* neighbour back-pointer */
};


#define PRIV(dev) ((struct clip_priv *) netdev_priv(dev))


struct clip_priv {
	int number;			/* for convenience ... */
	spinlock_t xoff_lock;		/* ensures that pop is atomic (SMP) */
	struct net_device_stats stats;
	struct net_device *next;	/* next CLIP interface */
};


#ifdef __KERNEL__
extern struct neigh_table *clip_tbl_hook;
#endif

#endif
