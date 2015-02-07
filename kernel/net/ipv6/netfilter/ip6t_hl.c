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

/* Hop Limit matching module */

/* (C) 2001-2002 Maciej Soltysiak <solt@dns.toxicfilms.tv>
 * Based on HW's ttl module
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#include <linux/ipv6.h>
#include <linux/module.h>
#include <linux/skbuff.h>

#include <linux/netfilter_ipv6/ip6t_hl.h>
#include <linux/netfilter/x_tables.h>

MODULE_AUTHOR("Maciej Soltysiak <solt@dns.toxicfilms.tv>");
MODULE_DESCRIPTION("Xtables: IPv6 Hop Limit field match");
MODULE_LICENSE("GPL");

static bool hl_mt6(const struct sk_buff *skb, const struct xt_match_param *par)
{
	const struct ip6t_hl_info *info = par->matchinfo;
	const struct ipv6hdr *ip6h = ipv6_hdr(skb);

	switch (info->mode) {
		case IP6T_HL_EQ:
			return ip6h->hop_limit == info->hop_limit;
			break;
		case IP6T_HL_NE:
			return ip6h->hop_limit != info->hop_limit;
			break;
		case IP6T_HL_LT:
			return ip6h->hop_limit < info->hop_limit;
			break;
		case IP6T_HL_GT:
			return ip6h->hop_limit > info->hop_limit;
			break;
		default:
			printk(KERN_WARNING "ip6t_hl: unknown mode %d\n",
				info->mode);
			return false;
	}

	return false;
}

static struct xt_match hl_mt6_reg __read_mostly = {
	.name		= "hl",
	.family		= NFPROTO_IPV6,
	.match		= hl_mt6,
	.matchsize	= sizeof(struct ip6t_hl_info),
	.me		= THIS_MODULE,
};

static int __init hl_mt6_init(void)
{
	return xt_register_match(&hl_mt6_reg);
}

static void __exit hl_mt6_exit(void)
{
	xt_unregister_match(&hl_mt6_reg);
}

module_init(hl_mt6_init);
module_exit(hl_mt6_exit);
