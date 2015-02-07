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

/* NETMAP - static NAT mapping of IP network addresses (1:1).
 * The mapping can be applied to source (POSTROUTING),
 * destination (PREROUTING), or both (with separate rules).
 */

/* (C) 2000-2001 Svenning Soerensen <svenning@post5.tele.dk>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#include <linux/ip.h>
#include <linux/module.h>
#include <linux/netdevice.h>
#include <linux/netfilter.h>
#include <linux/netfilter_ipv4.h>
#include <linux/netfilter/x_tables.h>
#include <net/netfilter/nf_nat_rule.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Svenning Soerensen <svenning@post5.tele.dk>");
MODULE_DESCRIPTION("Xtables: 1:1 NAT mapping of IPv4 subnets");

static bool netmap_tg_check(const struct xt_tgchk_param *par)
{
	const struct nf_nat_multi_range_compat *mr = par->targinfo;

	if (!(mr->range[0].flags & IP_NAT_RANGE_MAP_IPS)) {
		pr_debug("NETMAP:check: bad MAP_IPS.\n");
		return false;
	}
	if (mr->rangesize != 1) {
		pr_debug("NETMAP:check: bad rangesize %u.\n", mr->rangesize);
		return false;
	}
	return true;
}

static unsigned int
netmap_tg(struct sk_buff *skb, const struct xt_target_param *par)
{
	struct nf_conn *ct;
	enum ip_conntrack_info ctinfo;
	__be32 new_ip, netmask;
	const struct nf_nat_multi_range_compat *mr = par->targinfo;
	struct nf_nat_range newrange;

	NF_CT_ASSERT(par->hooknum == NF_INET_PRE_ROUTING ||
		     par->hooknum == NF_INET_POST_ROUTING ||
		     par->hooknum == NF_INET_LOCAL_OUT);
	ct = nf_ct_get(skb, &ctinfo);

	netmask = ~(mr->range[0].min_ip ^ mr->range[0].max_ip);

	if (par->hooknum == NF_INET_PRE_ROUTING ||
	    par->hooknum == NF_INET_LOCAL_OUT)
		new_ip = ip_hdr(skb)->daddr & ~netmask;
	else
		new_ip = ip_hdr(skb)->saddr & ~netmask;
	new_ip |= mr->range[0].min_ip & netmask;

	newrange = ((struct nf_nat_range)
		{ mr->range[0].flags | IP_NAT_RANGE_MAP_IPS,
		  new_ip, new_ip,
		  mr->range[0].min, mr->range[0].max });

	/* Hand modified range to generic setup. */
	return nf_nat_setup_info(ct, &newrange, HOOK2MANIP(par->hooknum));
}

static struct xt_target netmap_tg_reg __read_mostly = {
	.name 		= "NETMAP",
	.family		= NFPROTO_IPV4,
	.target 	= netmap_tg,
	.targetsize	= sizeof(struct nf_nat_multi_range_compat),
	.table		= "nat",
	.hooks		= (1 << NF_INET_PRE_ROUTING) |
			  (1 << NF_INET_POST_ROUTING) |
			  (1 << NF_INET_LOCAL_OUT),
	.checkentry 	= netmap_tg_check,
	.me 		= THIS_MODULE
};

static int __init netmap_tg_init(void)
{
	return xt_register_target(&netmap_tg_reg);
}

static void __exit netmap_tg_exit(void)
{
	xt_unregister_target(&netmap_tg_reg);
}

module_init(netmap_tg_init);
module_exit(netmap_tg_exit);
