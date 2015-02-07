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
 *  ebt_arp
 *
 *	Authors:
 *	Bart De Schuymer <bdschuym@pandora.be>
 *	Tim Gardner <timg@tpi.com>
 *
 *  April, 2002
 *
 */
#include <linux/if_arp.h>
#include <linux/if_ether.h>
#include <linux/module.h>
#include <linux/netfilter/x_tables.h>
#include <linux/netfilter_bridge/ebtables.h>
#include <linux/netfilter_bridge/ebt_arp.h>

static bool
ebt_arp_mt(const struct sk_buff *skb, const struct xt_match_param *par)
{
	const struct ebt_arp_info *info = par->matchinfo;
	const struct arphdr *ah;
	struct arphdr _arph;

	ah = skb_header_pointer(skb, 0, sizeof(_arph), &_arph);
	if (ah == NULL)
		return false;
	if (info->bitmask & EBT_ARP_OPCODE && FWINV(info->opcode !=
	   ah->ar_op, EBT_ARP_OPCODE))
		return false;
	if (info->bitmask & EBT_ARP_HTYPE && FWINV(info->htype !=
	   ah->ar_hrd, EBT_ARP_HTYPE))
		return false;
	if (info->bitmask & EBT_ARP_PTYPE && FWINV(info->ptype !=
	   ah->ar_pro, EBT_ARP_PTYPE))
		return false;

	if (info->bitmask & (EBT_ARP_SRC_IP | EBT_ARP_DST_IP | EBT_ARP_GRAT)) {
		const __be32 *sap, *dap;
		__be32 saddr, daddr;

		if (ah->ar_pln != sizeof(__be32) || ah->ar_pro != htons(ETH_P_IP))
			return false;
		sap = skb_header_pointer(skb, sizeof(struct arphdr) +
					ah->ar_hln, sizeof(saddr),
					&saddr);
		if (sap == NULL)
			return false;
		dap = skb_header_pointer(skb, sizeof(struct arphdr) +
					2*ah->ar_hln+sizeof(saddr),
					sizeof(daddr), &daddr);
		if (dap == NULL)
			return false;
		if (info->bitmask & EBT_ARP_SRC_IP &&
		    FWINV(info->saddr != (*sap & info->smsk), EBT_ARP_SRC_IP))
			return false;
		if (info->bitmask & EBT_ARP_DST_IP &&
		    FWINV(info->daddr != (*dap & info->dmsk), EBT_ARP_DST_IP))
			return false;
		if (info->bitmask & EBT_ARP_GRAT &&
		    FWINV(*dap != *sap, EBT_ARP_GRAT))
			return false;
	}

	if (info->bitmask & (EBT_ARP_SRC_MAC | EBT_ARP_DST_MAC)) {
		const unsigned char *mp;
		unsigned char _mac[ETH_ALEN];
		uint8_t verdict, i;

		if (ah->ar_hln != ETH_ALEN || ah->ar_hrd != htons(ARPHRD_ETHER))
			return false;
		if (info->bitmask & EBT_ARP_SRC_MAC) {
			mp = skb_header_pointer(skb, sizeof(struct arphdr),
						sizeof(_mac), &_mac);
			if (mp == NULL)
				return false;
			verdict = 0;
			for (i = 0; i < 6; i++)
				verdict |= (mp[i] ^ info->smaddr[i]) &
				       info->smmsk[i];
			if (FWINV(verdict != 0, EBT_ARP_SRC_MAC))
				return false;
		}

		if (info->bitmask & EBT_ARP_DST_MAC) {
			mp = skb_header_pointer(skb, sizeof(struct arphdr) +
						ah->ar_hln + ah->ar_pln,
						sizeof(_mac), &_mac);
			if (mp == NULL)
				return false;
			verdict = 0;
			for (i = 0; i < 6; i++)
				verdict |= (mp[i] ^ info->dmaddr[i]) &
					info->dmmsk[i];
			if (FWINV(verdict != 0, EBT_ARP_DST_MAC))
				return false;
		}
	}

	return true;
}

static bool ebt_arp_mt_check(const struct xt_mtchk_param *par)
{
	const struct ebt_arp_info *info = par->matchinfo;
	const struct ebt_entry *e = par->entryinfo;

	if ((e->ethproto != htons(ETH_P_ARP) &&
	   e->ethproto != htons(ETH_P_RARP)) ||
	   e->invflags & EBT_IPROTO)
		return false;
	if (info->bitmask & ~EBT_ARP_MASK || info->invflags & ~EBT_ARP_MASK)
		return false;
	return true;
}

static struct xt_match ebt_arp_mt_reg __read_mostly = {
	.name		= "arp",
	.revision	= 0,
	.family		= NFPROTO_BRIDGE,
	.match		= ebt_arp_mt,
	.checkentry	= ebt_arp_mt_check,
	.matchsize	= XT_ALIGN(sizeof(struct ebt_arp_info)),
	.me		= THIS_MODULE,
};

static int __init ebt_arp_init(void)
{
	return xt_register_match(&ebt_arp_mt_reg);
}

static void __exit ebt_arp_fini(void)
{
	xt_unregister_match(&ebt_arp_mt_reg);
}

module_init(ebt_arp_init);
module_exit(ebt_arp_fini);
MODULE_DESCRIPTION("Ebtables: ARP protocol packet match");
MODULE_LICENSE("GPL");
