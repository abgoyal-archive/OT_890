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
 *	NET3:	802.3 data link hooks used for IPX 802.3
 *
 *	This program is free software; you can redistribute it and/or
 *	modify it under the terms of the GNU General Public License
 *	as published by the Free Software Foundation; either version
 *	2 of the License, or (at your option) any later version.
 *
 *	802.3 isn't really a protocol data link layer. Some old IPX stuff
 *	uses it however. Note that there is only one 802.3 protocol layer
 *	in the system. We don't currently support different protocols
 *	running raw 802.3 on different devices. Thankfully nobody else
 *	has done anything like the old IPX.
 */

#include <linux/in.h>
#include <linux/mm.h>
#include <linux/module.h>
#include <linux/netdevice.h>
#include <linux/skbuff.h>

#include <net/datalink.h>
#include <net/p8022.h>

/*
 *	Place an 802.3 header on a packet. The driver will do the mac
 *	addresses, we just need to give it the buffer length.
 */
static int p8023_request(struct datalink_proto *dl,
			 struct sk_buff *skb, unsigned char *dest_node)
{
	struct net_device *dev = skb->dev;

	dev_hard_header(skb, dev, ETH_P_802_3, dest_node, NULL, skb->len);
	return dev_queue_xmit(skb);
}

/*
 *	Create an 802.3 client. Note there can be only one 802.3 client
 */
struct datalink_proto *make_8023_client(void)
{
	struct datalink_proto *proto = kmalloc(sizeof(*proto), GFP_ATOMIC);

	if (proto) {
		proto->header_length = 0;
		proto->request	     = p8023_request;
	}
	return proto;
}

/*
 *	Destroy the 802.3 client.
 */
void destroy_8023_client(struct datalink_proto *dl)
{
	kfree(dl);
}

EXPORT_SYMBOL(destroy_8023_client);
EXPORT_SYMBOL(make_8023_client);

MODULE_LICENSE("GPL");
