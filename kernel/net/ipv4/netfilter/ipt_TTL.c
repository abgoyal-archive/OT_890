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

/* TTL modification target for IP tables
 * (C) 2000,2005 by Harald Welte <laforge@netfilter.org>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 */

#include <linux/module.h>
#include <linux/skbuff.h>
#include <linux/ip.h>
#include <net/checksum.h>

#include <linux/netfilter/x_tables.h>
#include <linux/netfilter_ipv4/ipt_TTL.h>

MODULE_AUTHOR("Harald Welte <laforge@netfilter.org>");
MODULE_DESCRIPTION("Xtables: IPv4 TTL field modification target");
MODULE_LICENSE("GPL");

static unsigned int
ttl_tg(struct sk_buff *skb, const struct xt_target_param *par)
{
	struct iphdr *iph;
	const struct ipt_TTL_info *info = par->targinfo;
	int new_ttl;

	if (!skb_make_writable(skb, skb->len))
		return NF_DROP;

	iph = ip_hdr(skb);

	switch (info->mode) {
		case IPT_TTL_SET:
			new_ttl = info->ttl;
			break;
		case IPT_TTL_INC:
			new_ttl = iph->ttl + info->ttl;
			if (new_ttl > 255)
				new_ttl = 255;
			break;
		case IPT_TTL_DEC:
			new_ttl = iph->ttl - info->ttl;
			if (new_ttl < 0)
				new_ttl = 0;
			break;
		default:
			new_ttl = iph->ttl;
			break;
	}

	if (new_ttl != iph->ttl) {
		csum_replace2(&iph->check, htons(iph->ttl << 8),
					   htons(new_ttl << 8));
		iph->ttl = new_ttl;
	}

	return XT_CONTINUE;
}

static bool ttl_tg_check(const struct xt_tgchk_param *par)
{
	const struct ipt_TTL_info *info = par->targinfo;

	if (info->mode > IPT_TTL_MAXMODE) {
		printk(KERN_WARNING "ipt_TTL: invalid or unknown Mode %u\n",
			info->mode);
		return false;
	}
	if (info->mode != IPT_TTL_SET && info->ttl == 0)
		return false;
	return true;
}

static struct xt_target ttl_tg_reg __read_mostly = {
	.name 		= "TTL",
	.family		= NFPROTO_IPV4,
	.target 	= ttl_tg,
	.targetsize	= sizeof(struct ipt_TTL_info),
	.table		= "mangle",
	.checkentry 	= ttl_tg_check,
	.me 		= THIS_MODULE,
};

static int __init ttl_tg_init(void)
{
	return xt_register_target(&ttl_tg_reg);
}

static void __exit ttl_tg_exit(void)
{
	xt_unregister_target(&ttl_tg_reg);
}

module_init(ttl_tg_init);
module_exit(ttl_tg_exit);
