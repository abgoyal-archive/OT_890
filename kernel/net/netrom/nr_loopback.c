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
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * Copyright Tomi Manninen OH2BNS (oh2bns@sral.fi)
 */
#include <linux/types.h>
#include <linux/socket.h>
#include <linux/timer.h>
#include <net/ax25.h>
#include <linux/skbuff.h>
#include <net/netrom.h>
#include <linux/init.h>

static void nr_loopback_timer(unsigned long);

static struct sk_buff_head loopback_queue;
static DEFINE_TIMER(loopback_timer, nr_loopback_timer, 0, 0);

void __init nr_loopback_init(void)
{
	skb_queue_head_init(&loopback_queue);
}

static inline int nr_loopback_running(void)
{
	return timer_pending(&loopback_timer);
}

int nr_loopback_queue(struct sk_buff *skb)
{
	struct sk_buff *skbn;

	if ((skbn = alloc_skb(skb->len, GFP_ATOMIC)) != NULL) {
		skb_copy_from_linear_data(skb, skb_put(skbn, skb->len), skb->len);
		skb_reset_transport_header(skbn);

		skb_queue_tail(&loopback_queue, skbn);

		if (!nr_loopback_running())
			mod_timer(&loopback_timer, jiffies + 10);
	}

	kfree_skb(skb);
	return 1;
}

static void nr_loopback_timer(unsigned long param)
{
	struct sk_buff *skb;
	ax25_address *nr_dest;
	struct net_device *dev;

	if ((skb = skb_dequeue(&loopback_queue)) != NULL) {
		nr_dest = (ax25_address *)(skb->data + 7);

		dev = nr_dev_get(nr_dest);

		if (dev == NULL || nr_rx_frame(skb, dev) == 0)
			kfree_skb(skb);

		if (dev != NULL)
			dev_put(dev);

		if (!skb_queue_empty(&loopback_queue) && !nr_loopback_running())
			mod_timer(&loopback_timer, jiffies + 10);
	}
}

void __exit nr_loopback_clear(void)
{
	del_timer_sync(&loopback_timer);
	skb_queue_purge(&loopback_queue);
}
