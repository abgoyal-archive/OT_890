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
 *	NET3:	Support for 802.2 demultiplexing off Ethernet (Token ring
 *		is kept separate see p8022tr.c)
 *		This program is free software; you can redistribute it and/or
 *		modify it under the terms of the GNU General Public License
 *		as published by the Free Software Foundation; either version
 *		2 of the License, or (at your option) any later version.
 *
 *		Demultiplex 802.2 encoded protocols. We match the entry by the
 *		SSAP/DSAP pair and then deliver to the registered datalink that
 *		matches. The control byte is ignored and handling of such items
 *		is up to the routine passed the frame.
 *
 *		Unlike the 802.3 datalink we have a list of 802.2 entries as
 *		there are multiple protocols to demux. The list is currently
 *		short (3 or 4 entries at most). The current demux assumes this.
 */
#include <linux/module.h>
#include <linux/netdevice.h>
#include <linux/skbuff.h>
#include <net/datalink.h>
#include <linux/mm.h>
#include <linux/in.h>
#include <linux/init.h>
#include <net/llc.h>
#include <net/p8022.h>

static int p8022_request(struct datalink_proto *dl, struct sk_buff *skb,
			 unsigned char *dest)
{
	llc_build_and_send_ui_pkt(dl->sap, skb, dest, dl->sap->laddr.lsap);
	return 0;
}

struct datalink_proto *register_8022_client(unsigned char type,
					    int (*func)(struct sk_buff *skb,
							struct net_device *dev,
							struct packet_type *pt,
							struct net_device *orig_dev))
{
	struct datalink_proto *proto;

	proto = kmalloc(sizeof(*proto), GFP_ATOMIC);
	if (proto) {
		proto->type[0]		= type;
		proto->header_length	= 3;
		proto->request		= p8022_request;
		proto->sap = llc_sap_open(type, func);
		if (!proto->sap) {
			kfree(proto);
			proto = NULL;
		}
	}
	return proto;
}

void unregister_8022_client(struct datalink_proto *proto)
{
	llc_sap_put(proto->sap);
	kfree(proto);
}

EXPORT_SYMBOL(register_8022_client);
EXPORT_SYMBOL(unregister_8022_client);

MODULE_LICENSE("GPL");
