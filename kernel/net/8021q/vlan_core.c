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

#include <linux/skbuff.h>
#include <linux/netdevice.h>
#include <linux/if_vlan.h>
#include <linux/netpoll.h>
#include "vlan.h"

/* VLAN rx hw acceleration helper.  This acts like netif_{rx,receive_skb}(). */
int __vlan_hwaccel_rx(struct sk_buff *skb, struct vlan_group *grp,
		      u16 vlan_tci, int polling)
{
	if (netpoll_rx(skb))
		return NET_RX_DROP;

	if (skb_bond_should_drop(skb))
		goto drop;

	skb->vlan_tci = vlan_tci;
	skb->dev = vlan_group_get_device(grp, vlan_tci & VLAN_VID_MASK);

	if (!skb->dev)
		goto drop;

	return (polling ? netif_receive_skb(skb) : netif_rx(skb));

drop:
	dev_kfree_skb_any(skb);
	return NET_RX_DROP;
}
EXPORT_SYMBOL(__vlan_hwaccel_rx);

int vlan_hwaccel_do_receive(struct sk_buff *skb)
{
	struct net_device *dev = skb->dev;
	struct net_device_stats *stats;

	skb->dev = vlan_dev_info(dev)->real_dev;
	netif_nit_deliver(skb);

	skb->dev = dev;
	skb->priority = vlan_get_ingress_priority(dev, skb->vlan_tci);
	skb->vlan_tci = 0;

	stats = &dev->stats;
	stats->rx_packets++;
	stats->rx_bytes += skb->len;

	switch (skb->pkt_type) {
	case PACKET_BROADCAST:
		break;
	case PACKET_MULTICAST:
		stats->multicast++;
		break;
	case PACKET_OTHERHOST:
		/* Our lower layer thinks this is not local, let's make sure.
		 * This allows the VLAN to have a different MAC than the
		 * underlying device, and still route correctly. */
		if (!compare_ether_addr(eth_hdr(skb)->h_dest,
					dev->dev_addr))
			skb->pkt_type = PACKET_HOST;
		break;
	};
	return 0;
}

struct net_device *vlan_dev_real_dev(const struct net_device *dev)
{
	return vlan_dev_info(dev)->real_dev;
}
EXPORT_SYMBOL(vlan_dev_real_dev);

u16 vlan_dev_vlan_id(const struct net_device *dev)
{
	return vlan_dev_info(dev)->vlan_id;
}
EXPORT_SYMBOL(vlan_dev_vlan_id);

static int vlan_gro_common(struct napi_struct *napi, struct vlan_group *grp,
			   unsigned int vlan_tci, struct sk_buff *skb)
{
	struct sk_buff *p;

	if (skb_bond_should_drop(skb))
		goto drop;

	skb->vlan_tci = vlan_tci;
	skb->dev = vlan_group_get_device(grp, vlan_tci & VLAN_VID_MASK);

	if (!skb->dev)
		goto drop;

	for (p = napi->gro_list; p; p = p->next) {
		NAPI_GRO_CB(p)->same_flow = p->dev == skb->dev;
		NAPI_GRO_CB(p)->flush = 0;
	}

	return dev_gro_receive(napi, skb);

drop:
	return 2;
}

int vlan_gro_receive(struct napi_struct *napi, struct vlan_group *grp,
		     unsigned int vlan_tci, struct sk_buff *skb)
{
	int err = NET_RX_SUCCESS;

	if (netpoll_receive_skb(skb))
		return NET_RX_DROP;

	switch (vlan_gro_common(napi, grp, vlan_tci, skb)) {
	case -1:
		return netif_receive_skb(skb);

	case 2:
		err = NET_RX_DROP;
		/* fall through */

	case 1:
		kfree_skb(skb);
		break;
	}

	return err;
}
EXPORT_SYMBOL(vlan_gro_receive);

int vlan_gro_frags(struct napi_struct *napi, struct vlan_group *grp,
		   unsigned int vlan_tci, struct napi_gro_fraginfo *info)
{
	struct sk_buff *skb = napi_fraginfo_skb(napi, info);
	int err = NET_RX_DROP;

	if (!skb)
		goto out;

	if (netpoll_receive_skb(skb))
		goto out;

	err = NET_RX_SUCCESS;

	switch (vlan_gro_common(napi, grp, vlan_tci, skb)) {
	case -1:
		return netif_receive_skb(skb);

	case 2:
		err = NET_RX_DROP;
		/* fall through */

	case 1:
		napi_reuse_skb(napi, skb);
		break;
	}

out:
	return err;
}
EXPORT_SYMBOL(vlan_gro_frags);
