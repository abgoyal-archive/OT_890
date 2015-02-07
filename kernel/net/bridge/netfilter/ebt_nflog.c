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
 * ebt_nflog
 *
 *	Author:
 *	Peter Warasin <peter@endian.com>
 *
 *  February, 2008
 *
 * Based on:
 *  xt_NFLOG.c, (C) 2006 by Patrick McHardy <kaber@trash.net>
 *  ebt_ulog.c, (C) 2004 by Bart De Schuymer <bdschuym@pandora.be>
 *
 */

#include <linux/module.h>
#include <linux/spinlock.h>
#include <linux/netfilter/x_tables.h>
#include <linux/netfilter_bridge/ebtables.h>
#include <linux/netfilter_bridge/ebt_nflog.h>
#include <net/netfilter/nf_log.h>

static unsigned int
ebt_nflog_tg(struct sk_buff *skb, const struct xt_target_param *par)
{
	const struct ebt_nflog_info *info = par->targinfo;
	struct nf_loginfo li;

	li.type = NF_LOG_TYPE_ULOG;
	li.u.ulog.copy_len = info->len;
	li.u.ulog.group = info->group;
	li.u.ulog.qthreshold = info->threshold;

	nf_log_packet(PF_BRIDGE, par->hooknum, skb, par->in, par->out,
	              &li, "%s", info->prefix);
	return EBT_CONTINUE;
}

static bool ebt_nflog_tg_check(const struct xt_tgchk_param *par)
{
	struct ebt_nflog_info *info = par->targinfo;

	if (info->flags & ~EBT_NFLOG_MASK)
		return false;
	info->prefix[EBT_NFLOG_PREFIX_SIZE - 1] = '\0';
	return true;
}

static struct xt_target ebt_nflog_tg_reg __read_mostly = {
	.name       = "nflog",
	.revision   = 0,
	.family     = NFPROTO_BRIDGE,
	.target     = ebt_nflog_tg,
	.checkentry = ebt_nflog_tg_check,
	.targetsize = XT_ALIGN(sizeof(struct ebt_nflog_info)),
	.me         = THIS_MODULE,
};

static int __init ebt_nflog_init(void)
{
	return xt_register_target(&ebt_nflog_tg_reg);
}

static void __exit ebt_nflog_fini(void)
{
	xt_unregister_target(&ebt_nflog_tg_reg);
}

module_init(ebt_nflog_init);
module_exit(ebt_nflog_fini);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Peter Warasin <peter@endian.com>");
MODULE_DESCRIPTION("ebtables NFLOG netfilter logging module");
