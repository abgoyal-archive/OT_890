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

/* Kernel module to match packet length. */
/* (C) 1999-2001 James Morris <jmorros@intercode.com.au>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#include <linux/module.h>
#include <linux/skbuff.h>
#include <linux/ipv6.h>
#include <net/ip.h>

#include <linux/netfilter/xt_length.h>
#include <linux/netfilter/x_tables.h>

MODULE_AUTHOR("James Morris <jmorris@intercode.com.au>");
MODULE_DESCRIPTION("Xtables: Packet length (Layer3,4,5) match");
MODULE_LICENSE("GPL");
MODULE_ALIAS("ipt_length");
MODULE_ALIAS("ip6t_length");

static bool
length_mt(const struct sk_buff *skb, const struct xt_match_param *par)
{
	const struct xt_length_info *info = par->matchinfo;
	u_int16_t pktlen = ntohs(ip_hdr(skb)->tot_len);

	return (pktlen >= info->min && pktlen <= info->max) ^ info->invert;
}

static bool
length_mt6(const struct sk_buff *skb, const struct xt_match_param *par)
{
	const struct xt_length_info *info = par->matchinfo;
	const u_int16_t pktlen = ntohs(ipv6_hdr(skb)->payload_len) +
				 sizeof(struct ipv6hdr);

	return (pktlen >= info->min && pktlen <= info->max) ^ info->invert;
}

static struct xt_match length_mt_reg[] __read_mostly = {
	{
		.name		= "length",
		.family		= NFPROTO_IPV4,
		.match		= length_mt,
		.matchsize	= sizeof(struct xt_length_info),
		.me		= THIS_MODULE,
	},
	{
		.name		= "length",
		.family		= NFPROTO_IPV6,
		.match		= length_mt6,
		.matchsize	= sizeof(struct xt_length_info),
		.me		= THIS_MODULE,
	},
};

static int __init length_mt_init(void)
{
	return xt_register_matches(length_mt_reg, ARRAY_SIZE(length_mt_reg));
}

static void __exit length_mt_exit(void)
{
	xt_unregister_matches(length_mt_reg, ARRAY_SIZE(length_mt_reg));
}

module_init(length_mt_init);
module_exit(length_mt_exit);
