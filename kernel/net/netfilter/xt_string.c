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

/* String matching match for iptables
 *
 * (C) 2005 Pablo Neira Ayuso <pablo@eurodev.net>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/skbuff.h>
#include <linux/netfilter/x_tables.h>
#include <linux/netfilter/xt_string.h>
#include <linux/textsearch.h>

MODULE_AUTHOR("Pablo Neira Ayuso <pablo@eurodev.net>");
MODULE_DESCRIPTION("Xtables: string-based matching");
MODULE_LICENSE("GPL");
MODULE_ALIAS("ipt_string");
MODULE_ALIAS("ip6t_string");

static bool
string_mt(const struct sk_buff *skb, const struct xt_match_param *par)
{
	const struct xt_string_info *conf = par->matchinfo;
	struct ts_state state;
	int invert;

	memset(&state, 0, sizeof(struct ts_state));

	invert = (par->match->revision == 0 ? conf->u.v0.invert :
				    conf->u.v1.flags & XT_STRING_FLAG_INVERT);

	return (skb_find_text((struct sk_buff *)skb, conf->from_offset,
			     conf->to_offset, conf->config, &state)
			     != UINT_MAX) ^ invert;
}

#define STRING_TEXT_PRIV(m) ((struct xt_string_info *)(m))

static bool string_mt_check(const struct xt_mtchk_param *par)
{
	struct xt_string_info *conf = par->matchinfo;
	struct ts_config *ts_conf;
	int flags = TS_AUTOLOAD;

	/* Damn, can't handle this case properly with iptables... */
	if (conf->from_offset > conf->to_offset)
		return false;
	if (conf->algo[XT_STRING_MAX_ALGO_NAME_SIZE - 1] != '\0')
		return false;
	if (conf->patlen > XT_STRING_MAX_PATTERN_SIZE)
		return false;
	if (par->match->revision == 1) {
		if (conf->u.v1.flags &
		    ~(XT_STRING_FLAG_IGNORECASE | XT_STRING_FLAG_INVERT))
			return false;
		if (conf->u.v1.flags & XT_STRING_FLAG_IGNORECASE)
			flags |= TS_IGNORECASE;
	}
	ts_conf = textsearch_prepare(conf->algo, conf->pattern, conf->patlen,
				     GFP_KERNEL, flags);
	if (IS_ERR(ts_conf))
		return false;

	conf->config = ts_conf;

	return true;
}

static void string_mt_destroy(const struct xt_mtdtor_param *par)
{
	textsearch_destroy(STRING_TEXT_PRIV(par->matchinfo)->config);
}

static struct xt_match xt_string_mt_reg[] __read_mostly = {
	{
		.name 		= "string",
		.revision	= 0,
		.family		= NFPROTO_UNSPEC,
		.checkentry	= string_mt_check,
		.match 		= string_mt,
		.destroy 	= string_mt_destroy,
		.matchsize	= sizeof(struct xt_string_info),
		.me 		= THIS_MODULE
	},
	{
		.name 		= "string",
		.revision	= 1,
		.family		= NFPROTO_UNSPEC,
		.checkentry	= string_mt_check,
		.match 		= string_mt,
		.destroy 	= string_mt_destroy,
		.matchsize	= sizeof(struct xt_string_info),
		.me 		= THIS_MODULE
	},
};

static int __init string_mt_init(void)
{
	return xt_register_matches(xt_string_mt_reg,
				   ARRAY_SIZE(xt_string_mt_reg));
}

static void __exit string_mt_exit(void)
{
	xt_unregister_matches(xt_string_mt_reg, ARRAY_SIZE(xt_string_mt_reg));
}

module_init(string_mt_init);
module_exit(string_mt_exit);
