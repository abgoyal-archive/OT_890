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
 *  ebt_redirect
 *
 *	Authors:
 *	Bart De Schuymer <bdschuym@pandora.be>
 *
 *  April, 2002
 *
 */
#include <linux/module.h>
#include <net/sock.h>
#include "../br_private.h"
#include <linux/netfilter.h>
#include <linux/netfilter/x_tables.h>
#include <linux/netfilter_bridge/ebtables.h>
#include <linux/netfilter_bridge/ebt_redirect.h>

static unsigned int
ebt_redirect_tg(struct sk_buff *skb, const struct xt_target_param *par)
{
	const struct ebt_redirect_info *info = par->targinfo;

	if (!skb_make_writable(skb, 0))
		return EBT_DROP;

	if (par->hooknum != NF_BR_BROUTING)
		memcpy(eth_hdr(skb)->h_dest,
		       par->in->br_port->br->dev->dev_addr, ETH_ALEN);
	else
		memcpy(eth_hdr(skb)->h_dest, par->in->dev_addr, ETH_ALEN);
	skb->pkt_type = PACKET_HOST;
	return info->target;
}

static bool ebt_redirect_tg_check(const struct xt_tgchk_param *par)
{
	const struct ebt_redirect_info *info = par->targinfo;
	unsigned int hook_mask;

	if (BASE_CHAIN && info->target == EBT_RETURN)
		return false;

	hook_mask = par->hook_mask & ~(1 << NF_BR_NUMHOOKS);
	if ((strcmp(par->table, "nat") != 0 ||
	    hook_mask & ~(1 << NF_BR_PRE_ROUTING)) &&
	    (strcmp(par->table, "broute") != 0 ||
	    hook_mask & ~(1 << NF_BR_BROUTING)))
		return false;
	if (INVALID_TARGET)
		return false;
	return true;
}

static struct xt_target ebt_redirect_tg_reg __read_mostly = {
	.name		= "redirect",
	.revision	= 0,
	.family		= NFPROTO_BRIDGE,
	.hooks		= (1 << NF_BR_NUMHOOKS) | (1 << NF_BR_PRE_ROUTING) |
			  (1 << NF_BR_BROUTING),
	.target		= ebt_redirect_tg,
	.checkentry	= ebt_redirect_tg_check,
	.targetsize	= XT_ALIGN(sizeof(struct ebt_redirect_info)),
	.me		= THIS_MODULE,
};

static int __init ebt_redirect_init(void)
{
	return xt_register_target(&ebt_redirect_tg_reg);
}

static void __exit ebt_redirect_fini(void)
{
	xt_unregister_target(&ebt_redirect_tg_reg);
}

module_init(ebt_redirect_init);
module_exit(ebt_redirect_fini);
MODULE_DESCRIPTION("Ebtables: Packet redirection to localhost");
MODULE_LICENSE("GPL");
