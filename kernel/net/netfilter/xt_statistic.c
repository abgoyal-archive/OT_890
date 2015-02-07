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
 * Copyright (c) 2006 Patrick McHardy <kaber@trash.net>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * Based on ipt_random and ipt_nth by Fabrice MARIE <fabrice@netfilter.org>.
 */

#include <linux/init.h>
#include <linux/spinlock.h>
#include <linux/skbuff.h>
#include <linux/net.h>

#include <linux/netfilter/xt_statistic.h>
#include <linux/netfilter/x_tables.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Patrick McHardy <kaber@trash.net>");
MODULE_DESCRIPTION("Xtables: statistics-based matching (\"Nth\", random)");
MODULE_ALIAS("ipt_statistic");
MODULE_ALIAS("ip6t_statistic");

static DEFINE_SPINLOCK(nth_lock);

static bool
statistic_mt(const struct sk_buff *skb, const struct xt_match_param *par)
{
	struct xt_statistic_info *info = (void *)par->matchinfo;
	bool ret = info->flags & XT_STATISTIC_INVERT;

	switch (info->mode) {
	case XT_STATISTIC_MODE_RANDOM:
		if ((net_random() & 0x7FFFFFFF) < info->u.random.probability)
			ret = !ret;
		break;
	case XT_STATISTIC_MODE_NTH:
		info = info->master;
		spin_lock_bh(&nth_lock);
		if (info->u.nth.count++ == info->u.nth.every) {
			info->u.nth.count = 0;
			ret = !ret;
		}
		spin_unlock_bh(&nth_lock);
		break;
	}

	return ret;
}

static bool statistic_mt_check(const struct xt_mtchk_param *par)
{
	struct xt_statistic_info *info = par->matchinfo;

	if (info->mode > XT_STATISTIC_MODE_MAX ||
	    info->flags & ~XT_STATISTIC_MASK)
		return false;
	info->master = info;
	return true;
}

static struct xt_match xt_statistic_mt_reg __read_mostly = {
	.name       = "statistic",
	.revision   = 0,
	.family     = NFPROTO_UNSPEC,
	.match      = statistic_mt,
	.checkentry = statistic_mt_check,
	.matchsize  = sizeof(struct xt_statistic_info),
	.me         = THIS_MODULE,
};

static int __init statistic_mt_init(void)
{
	return xt_register_match(&xt_statistic_mt_reg);
}

static void __exit statistic_mt_exit(void)
{
	xt_unregister_match(&xt_statistic_mt_reg);
}

module_init(statistic_mt_init);
module_exit(statistic_mt_exit);
