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

/* (C) 1999-2001 Michal Ludvig <michal@logix.cz>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#include <linux/module.h>
#include <linux/skbuff.h>
#include <linux/if_ether.h>
#include <linux/if_packet.h>
#include <linux/in.h>
#include <linux/ip.h>
#include <linux/ipv6.h>

#include <linux/netfilter/xt_pkttype.h>
#include <linux/netfilter/x_tables.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Michal Ludvig <michal@logix.cz>");
MODULE_DESCRIPTION("Xtables: link layer packet type match");
MODULE_ALIAS("ipt_pkttype");
MODULE_ALIAS("ip6t_pkttype");

static bool
pkttype_mt(const struct sk_buff *skb, const struct xt_match_param *par)
{
	const struct xt_pkttype_info *info = par->matchinfo;
	u_int8_t type;

	if (skb->pkt_type != PACKET_LOOPBACK)
		type = skb->pkt_type;
	else if (par->family == NFPROTO_IPV4 &&
	    ipv4_is_multicast(ip_hdr(skb)->daddr))
		type = PACKET_MULTICAST;
	else if (par->family == NFPROTO_IPV6 &&
	    ipv6_hdr(skb)->daddr.s6_addr[0] == 0xFF)
		type = PACKET_MULTICAST;
	else
		type = PACKET_BROADCAST;

	return (type == info->pkttype) ^ info->invert;
}

static struct xt_match pkttype_mt_reg __read_mostly = {
	.name      = "pkttype",
	.revision  = 0,
	.family    = NFPROTO_UNSPEC,
	.match     = pkttype_mt,
	.matchsize = sizeof(struct xt_pkttype_info),
	.me        = THIS_MODULE,
};

static int __init pkttype_mt_init(void)
{
	return xt_register_match(&pkttype_mt_reg);
}

static void __exit pkttype_mt_exit(void)
{
	xt_unregister_match(&pkttype_mt_reg);
}

module_init(pkttype_mt_init);
module_exit(pkttype_mt_exit);
