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

/* IRC extension for TCP NAT alteration.
 *
 * (C) 2000-2001 by Harald Welte <laforge@gnumonks.org>
 * (C) 2004 Rusty Russell <rusty@rustcorp.com.au> IBM Corporation
 * based on a copy of RR's ip_nat_ftp.c
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version
 * 2 of the License, or (at your option) any later version.
 */

#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/tcp.h>
#include <linux/kernel.h>

#include <net/netfilter/nf_nat.h>
#include <net/netfilter/nf_nat_helper.h>
#include <net/netfilter/nf_nat_rule.h>
#include <net/netfilter/nf_conntrack_helper.h>
#include <net/netfilter/nf_conntrack_expect.h>
#include <linux/netfilter/nf_conntrack_irc.h>

MODULE_AUTHOR("Harald Welte <laforge@gnumonks.org>");
MODULE_DESCRIPTION("IRC (DCC) NAT helper");
MODULE_LICENSE("GPL");
MODULE_ALIAS("ip_nat_irc");

static unsigned int help(struct sk_buff *skb,
			 enum ip_conntrack_info ctinfo,
			 unsigned int matchoff,
			 unsigned int matchlen,
			 struct nf_conntrack_expect *exp)
{
	char buffer[sizeof("4294967296 65635")];
	u_int32_t ip;
	u_int16_t port;
	unsigned int ret;

	/* Reply comes from server. */
	exp->saved_proto.tcp.port = exp->tuple.dst.u.tcp.port;
	exp->dir = IP_CT_DIR_REPLY;
	exp->expectfn = nf_nat_follow_master;

	/* Try to get same port: if not, try to change it. */
	for (port = ntohs(exp->saved_proto.tcp.port); port != 0; port++) {
		exp->tuple.dst.u.tcp.port = htons(port);
		if (nf_ct_expect_related(exp) == 0)
			break;
	}

	if (port == 0)
		return NF_DROP;

	ip = ntohl(exp->master->tuplehash[IP_CT_DIR_REPLY].tuple.dst.u3.ip);
	sprintf(buffer, "%u %u", ip, port);
	pr_debug("nf_nat_irc: inserting '%s' == %pI4, port %u\n",
		 buffer, &ip, port);

	ret = nf_nat_mangle_tcp_packet(skb, exp->master, ctinfo,
				       matchoff, matchlen, buffer,
				       strlen(buffer));
	if (ret != NF_ACCEPT)
		nf_ct_unexpect_related(exp);
	return ret;
}

static void __exit nf_nat_irc_fini(void)
{
	rcu_assign_pointer(nf_nat_irc_hook, NULL);
	synchronize_rcu();
}

static int __init nf_nat_irc_init(void)
{
	BUG_ON(nf_nat_irc_hook != NULL);
	rcu_assign_pointer(nf_nat_irc_hook, help);
	return 0;
}

/* Prior to 2.6.11, we had a ports param.  No longer, but don't break users. */
static int warn_set(const char *val, struct kernel_param *kp)
{
	printk(KERN_INFO KBUILD_MODNAME
	       ": kernel >= 2.6.10 only uses 'ports' for conntrack modules\n");
	return 0;
}
module_param_call(ports, warn_set, NULL, NULL, 0);

module_init(nf_nat_irc_init);
module_exit(nf_nat_irc_fini);
