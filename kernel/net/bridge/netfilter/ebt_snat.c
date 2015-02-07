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
 *  ebt_snat
 *
 *	Authors:
 *	Bart De Schuymer <bdschuym@pandora.be>
 *
 *  June, 2002
 *
 */
#include <linux/module.h>
#include <net/sock.h>
#include <linux/if_arp.h>
#include <net/arp.h>
#include <linux/netfilter.h>
#include <linux/netfilter/x_tables.h>
#include <linux/netfilter_bridge/ebtables.h>
#include <linux/netfilter_bridge/ebt_nat.h>

static unsigned int
ebt_snat_tg(struct sk_buff *skb, const struct xt_target_param *par)
{
	const struct ebt_nat_info *info = par->targinfo;

	if (!skb_make_writable(skb, 0))
		return EBT_DROP;

	memcpy(eth_hdr(skb)->h_source, info->mac, ETH_ALEN);
	if (!(info->target & NAT_ARP_BIT) &&
	    eth_hdr(skb)->h_proto == htons(ETH_P_ARP)) {
		const struct arphdr *ap;
		struct arphdr _ah;

		ap = skb_header_pointer(skb, 0, sizeof(_ah), &_ah);
		if (ap == NULL)
			return EBT_DROP;
		if (ap->ar_hln != ETH_ALEN)
			goto out;
		if (skb_store_bits(skb, sizeof(_ah), info->mac,ETH_ALEN))
			return EBT_DROP;
	}
out:
	return info->target | ~EBT_VERDICT_BITS;
}

static bool ebt_snat_tg_check(const struct xt_tgchk_param *par)
{
	const struct ebt_nat_info *info = par->targinfo;
	int tmp;

	tmp = info->target | ~EBT_VERDICT_BITS;
	if (BASE_CHAIN && tmp == EBT_RETURN)
		return false;

	if (tmp < -NUM_STANDARD_TARGETS || tmp >= 0)
		return false;
	tmp = info->target | EBT_VERDICT_BITS;
	if ((tmp & ~NAT_ARP_BIT) != ~NAT_ARP_BIT)
		return false;
	return true;
}

static struct xt_target ebt_snat_tg_reg __read_mostly = {
	.name		= "snat",
	.revision	= 0,
	.family		= NFPROTO_BRIDGE,
	.table		= "nat",
	.hooks		= (1 << NF_BR_NUMHOOKS) | (1 << NF_BR_POST_ROUTING),
	.target		= ebt_snat_tg,
	.checkentry	= ebt_snat_tg_check,
	.targetsize	= XT_ALIGN(sizeof(struct ebt_nat_info)),
	.me		= THIS_MODULE,
};

static int __init ebt_snat_init(void)
{
	return xt_register_target(&ebt_snat_tg_reg);
}

static void __exit ebt_snat_fini(void)
{
	xt_unregister_target(&ebt_snat_tg_reg);
}

module_init(ebt_snat_init);
module_exit(ebt_snat_fini);
MODULE_DESCRIPTION("Ebtables: Source MAC address translation");
MODULE_LICENSE("GPL");
