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

/* iptables module for using new netfilter netlink queue
 *
 * (C) 2005 by Harald Welte <laforge@netfilter.org>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 */

#include <linux/module.h>
#include <linux/skbuff.h>

#include <linux/netfilter.h>
#include <linux/netfilter_arp.h>
#include <linux/netfilter/x_tables.h>
#include <linux/netfilter/xt_NFQUEUE.h>

MODULE_AUTHOR("Harald Welte <laforge@netfilter.org>");
MODULE_DESCRIPTION("Xtables: packet forwarding to netlink");
MODULE_LICENSE("GPL");
MODULE_ALIAS("ipt_NFQUEUE");
MODULE_ALIAS("ip6t_NFQUEUE");
MODULE_ALIAS("arpt_NFQUEUE");

static unsigned int
nfqueue_tg(struct sk_buff *skb, const struct xt_target_param *par)
{
	const struct xt_NFQ_info *tinfo = par->targinfo;

	return NF_QUEUE_NR(tinfo->queuenum);
}

static struct xt_target nfqueue_tg_reg[] __read_mostly = {
	{
		.name		= "NFQUEUE",
		.family		= NFPROTO_IPV4,
		.target		= nfqueue_tg,
		.targetsize	= sizeof(struct xt_NFQ_info),
		.me		= THIS_MODULE,
	},
	{
		.name		= "NFQUEUE",
		.family		= NFPROTO_IPV6,
		.target		= nfqueue_tg,
		.targetsize	= sizeof(struct xt_NFQ_info),
		.me		= THIS_MODULE,
	},
	{
		.name		= "NFQUEUE",
		.family		= NFPROTO_ARP,
		.target		= nfqueue_tg,
		.targetsize	= sizeof(struct xt_NFQ_info),
		.me		= THIS_MODULE,
	},
};

static int __init nfqueue_tg_init(void)
{
	return xt_register_targets(nfqueue_tg_reg, ARRAY_SIZE(nfqueue_tg_reg));
}

static void __exit nfqueue_tg_exit(void)
{
	xt_unregister_targets(nfqueue_tg_reg, ARRAY_SIZE(nfqueue_tg_reg));
}

module_init(nfqueue_tg_init);
module_exit(nfqueue_tg_exit);
