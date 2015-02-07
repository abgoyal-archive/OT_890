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
 *	STP SAP demux
 *
 *	Copyright (c) 2008 Patrick McHardy <kaber@trash.net>
 *
 *	This program is free software; you can redistribute it and/or
 *	modify it under the terms of the GNU General Public License
 *	version 2 as published by the Free Software Foundation.
 */
#include <linux/mutex.h>
#include <linux/skbuff.h>
#include <linux/etherdevice.h>
#include <linux/llc.h>
#include <net/llc.h>
#include <net/llc_pdu.h>
#include <net/stp.h>

/* 01:80:c2:00:00:20 - 01:80:c2:00:00:2F */
#define GARP_ADDR_MIN	0x20
#define GARP_ADDR_MAX	0x2F
#define GARP_ADDR_RANGE	(GARP_ADDR_MAX - GARP_ADDR_MIN)

static const struct stp_proto *garp_protos[GARP_ADDR_RANGE + 1] __read_mostly;
static const struct stp_proto *stp_proto __read_mostly;

static struct llc_sap *sap __read_mostly;
static unsigned int sap_registered;
static DEFINE_MUTEX(stp_proto_mutex);

/* Called under rcu_read_lock from LLC */
static int stp_pdu_rcv(struct sk_buff *skb, struct net_device *dev,
		       struct packet_type *pt, struct net_device *orig_dev)
{
	const struct ethhdr *eh = eth_hdr(skb);
	const struct llc_pdu_un *pdu = llc_pdu_un_hdr(skb);
	const struct stp_proto *proto;

	if (pdu->ssap != LLC_SAP_BSPAN ||
	    pdu->dsap != LLC_SAP_BSPAN ||
	    pdu->ctrl_1 != LLC_PDU_TYPE_U)
		goto err;

	if (eh->h_dest[5] >= GARP_ADDR_MIN && eh->h_dest[5] <= GARP_ADDR_MAX) {
		proto = rcu_dereference(garp_protos[eh->h_dest[5] -
						    GARP_ADDR_MIN]);
		if (proto &&
		    compare_ether_addr(eh->h_dest, proto->group_address))
			goto err;
	} else
		proto = rcu_dereference(stp_proto);

	if (!proto)
		goto err;

	proto->rcv(proto, skb, dev);
	return 0;

err:
	kfree_skb(skb);
	return 0;
}

int stp_proto_register(const struct stp_proto *proto)
{
	int err = 0;

	mutex_lock(&stp_proto_mutex);
	if (sap_registered++ == 0) {
		sap = llc_sap_open(LLC_SAP_BSPAN, stp_pdu_rcv);
		if (!sap) {
			err = -ENOMEM;
			goto out;
		}
	}
	if (is_zero_ether_addr(proto->group_address))
		rcu_assign_pointer(stp_proto, proto);
	else
		rcu_assign_pointer(garp_protos[proto->group_address[5] -
					       GARP_ADDR_MIN], proto);
out:
	mutex_unlock(&stp_proto_mutex);
	return err;
}
EXPORT_SYMBOL_GPL(stp_proto_register);

void stp_proto_unregister(const struct stp_proto *proto)
{
	mutex_lock(&stp_proto_mutex);
	if (is_zero_ether_addr(proto->group_address))
		rcu_assign_pointer(stp_proto, NULL);
	else
		rcu_assign_pointer(garp_protos[proto->group_address[5] -
					       GARP_ADDR_MIN], NULL);
	synchronize_rcu();

	if (--sap_registered == 0)
		llc_sap_put(sap);
	mutex_unlock(&stp_proto_mutex);
}
EXPORT_SYMBOL_GPL(stp_proto_unregister);

MODULE_LICENSE("GPL");
