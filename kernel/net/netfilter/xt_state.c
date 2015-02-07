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

/* Kernel module to match connection tracking information. */

/* (C) 1999-2001 Paul `Rusty' Russell
 * (C) 2002-2005 Netfilter Core Team <coreteam@netfilter.org>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#include <linux/module.h>
#include <linux/skbuff.h>
#include <net/netfilter/nf_conntrack.h>
#include <linux/netfilter/x_tables.h>
#include <linux/netfilter/xt_state.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Rusty Russell <rusty@rustcorp.com.au>");
MODULE_DESCRIPTION("ip[6]_tables connection tracking state match module");
MODULE_ALIAS("ipt_state");
MODULE_ALIAS("ip6t_state");

static bool
state_mt(const struct sk_buff *skb, const struct xt_match_param *par)
{
	const struct xt_state_info *sinfo = par->matchinfo;
	enum ip_conntrack_info ctinfo;
	unsigned int statebit;

	if (nf_ct_is_untracked(skb))
		statebit = XT_STATE_UNTRACKED;
	else if (!nf_ct_get(skb, &ctinfo))
		statebit = XT_STATE_INVALID;
	else
		statebit = XT_STATE_BIT(ctinfo);

	return (sinfo->statemask & statebit);
}

static bool state_mt_check(const struct xt_mtchk_param *par)
{
	if (nf_ct_l3proto_try_module_get(par->match->family) < 0) {
		printk(KERN_WARNING "can't load conntrack support for "
				    "proto=%u\n", par->match->family);
		return false;
	}
	return true;
}

static void state_mt_destroy(const struct xt_mtdtor_param *par)
{
	nf_ct_l3proto_module_put(par->match->family);
}

static struct xt_match state_mt_reg[] __read_mostly = {
	{
		.name		= "state",
		.family		= NFPROTO_IPV4,
		.checkentry	= state_mt_check,
		.match		= state_mt,
		.destroy	= state_mt_destroy,
		.matchsize	= sizeof(struct xt_state_info),
		.me		= THIS_MODULE,
	},
	{
		.name		= "state",
		.family		= NFPROTO_IPV6,
		.checkentry	= state_mt_check,
		.match		= state_mt,
		.destroy	= state_mt_destroy,
		.matchsize	= sizeof(struct xt_state_info),
		.me		= THIS_MODULE,
	},
};

static int __init state_mt_init(void)
{
	return xt_register_matches(state_mt_reg, ARRAY_SIZE(state_mt_reg));
}

static void __exit state_mt_exit(void)
{
	xt_unregister_matches(state_mt_reg, ARRAY_SIZE(state_mt_reg));
}

module_init(state_mt_init);
module_exit(state_mt_exit);
