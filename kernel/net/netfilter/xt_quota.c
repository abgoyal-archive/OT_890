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
 * netfilter module to enforce network quotas
 *
 * Sam Johnston <samj@samj.net>
 */
#include <linux/skbuff.h>
#include <linux/spinlock.h>

#include <linux/netfilter/x_tables.h>
#include <linux/netfilter/xt_quota.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Sam Johnston <samj@samj.net>");
MODULE_DESCRIPTION("Xtables: countdown quota match");
MODULE_ALIAS("ipt_quota");
MODULE_ALIAS("ip6t_quota");

static DEFINE_SPINLOCK(quota_lock);

static bool
quota_mt(const struct sk_buff *skb, const struct xt_match_param *par)
{
	struct xt_quota_info *q =
		((const struct xt_quota_info *)par->matchinfo)->master;
	bool ret = q->flags & XT_QUOTA_INVERT;

	spin_lock_bh(&quota_lock);
	if (q->quota >= skb->len) {
		q->quota -= skb->len;
		ret = !ret;
	} else {
		/* we do not allow even small packets from now on */
		q->quota = 0;
	}
	spin_unlock_bh(&quota_lock);

	return ret;
}

static bool quota_mt_check(const struct xt_mtchk_param *par)
{
	struct xt_quota_info *q = par->matchinfo;

	if (q->flags & ~XT_QUOTA_MASK)
		return false;
	/* For SMP, we only want to use one set of counters. */
	q->master = q;
	return true;
}

static struct xt_match quota_mt_reg __read_mostly = {
	.name       = "quota",
	.revision   = 0,
	.family     = NFPROTO_UNSPEC,
	.match      = quota_mt,
	.checkentry = quota_mt_check,
	.matchsize  = sizeof(struct xt_quota_info),
	.me         = THIS_MODULE,
};

static int __init quota_mt_init(void)
{
	return xt_register_match(&quota_mt_reg);
}

static void __exit quota_mt_exit(void)
{
	xt_unregister_match(&quota_mt_reg);
}

module_init(quota_mt_init);
module_exit(quota_mt_exit);
