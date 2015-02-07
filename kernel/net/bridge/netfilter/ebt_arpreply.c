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
 *  ebt_arpreply
 *
 *	Authors:
 *	Grzegorz Borowiak <grzes@gnu.univ.gda.pl>
 *	Bart De Schuymer <bdschuym@pandora.be>
 *
 *  August, 2003
 *
 */
#include <linux/if_arp.h>
#include <net/arp.h>
#include <linux/module.h>
#include <linux/netfilter/x_tables.h>
#include <linux/netfilter_bridge/ebtables.h>
#include <linux/netfilter_bridge/ebt_arpreply.h>

static unsigned int
ebt_arpreply_tg(struct sk_buff *skb, const struct xt_target_param *par)
{
	const struct ebt_arpreply_info *info = par->targinfo;
	const __be32 *siptr, *diptr;
	__be32 _sip, _dip;
	const struct arphdr *ap;
	struct arphdr _ah;
	const unsigned char *shp;
	unsigned char _sha[ETH_ALEN];

	ap = skb_header_pointer(skb, 0, sizeof(_ah), &_ah);
	if (ap == NULL)
		return EBT_DROP;

	if (ap->ar_op != htons(ARPOP_REQUEST) ||
	    ap->ar_hln != ETH_ALEN ||
	    ap->ar_pro != htons(ETH_P_IP) ||
	    ap->ar_pln != 4)
		return EBT_CONTINUE;

	shp = skb_header_pointer(skb, sizeof(_ah), ETH_ALEN, &_sha);
	if (shp == NULL)
		return EBT_DROP;

	siptr = skb_header_pointer(skb, sizeof(_ah) + ETH_ALEN,
				   sizeof(_sip), &_sip);
	if (siptr == NULL)
		return EBT_DROP;

	diptr = skb_header_pointer(skb,
				   sizeof(_ah) + 2 * ETH_ALEN + sizeof(_sip),
				   sizeof(_dip), &_dip);
	if (diptr == NULL)
		return EBT_DROP;

	arp_send(ARPOP_REPLY, ETH_P_ARP, *siptr, (struct net_device *)par->in,
		 *diptr, shp, info->mac, shp);

	return info->target;
}

static bool ebt_arpreply_tg_check(const struct xt_tgchk_param *par)
{
	const struct ebt_arpreply_info *info = par->targinfo;
	const struct ebt_entry *e = par->entryinfo;

	if (BASE_CHAIN && info->target == EBT_RETURN)
		return false;
	if (e->ethproto != htons(ETH_P_ARP) ||
	    e->invflags & EBT_IPROTO)
		return false;
	return true;
}

static struct xt_target ebt_arpreply_tg_reg __read_mostly = {
	.name		= "arpreply",
	.revision	= 0,
	.family		= NFPROTO_BRIDGE,
	.table		= "nat",
	.hooks		= (1 << NF_BR_NUMHOOKS) | (1 << NF_BR_PRE_ROUTING),
	.target		= ebt_arpreply_tg,
	.checkentry	= ebt_arpreply_tg_check,
	.targetsize	= XT_ALIGN(sizeof(struct ebt_arpreply_info)),
	.me		= THIS_MODULE,
};

static int __init ebt_arpreply_init(void)
{
	return xt_register_target(&ebt_arpreply_tg_reg);
}

static void __exit ebt_arpreply_fini(void)
{
	xt_unregister_target(&ebt_arpreply_tg_reg);
}

module_init(ebt_arpreply_init);
module_exit(ebt_arpreply_fini);
MODULE_DESCRIPTION("Ebtables: ARP reply target");
MODULE_LICENSE("GPL");
