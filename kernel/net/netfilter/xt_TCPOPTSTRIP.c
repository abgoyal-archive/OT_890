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
 * A module for stripping a specific TCP option from TCP packets.
 *
 * Copyright (C) 2007 Sven Schnelle <svens@bitebene.org>
 * Copyright © CC Computer Consultants GmbH, 2007
 * Contact: Jan Engelhardt <jengelh@computergmbh.de>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#include <linux/module.h>
#include <linux/skbuff.h>
#include <linux/ip.h>
#include <linux/ipv6.h>
#include <linux/tcp.h>
#include <net/ipv6.h>
#include <net/tcp.h>
#include <linux/netfilter/x_tables.h>
#include <linux/netfilter/xt_TCPOPTSTRIP.h>

static inline unsigned int optlen(const u_int8_t *opt, unsigned int offset)
{
	/* Beware zero-length options: make finite progress */
	if (opt[offset] <= TCPOPT_NOP || opt[offset+1] == 0)
		return 1;
	else
		return opt[offset+1];
}

static unsigned int
tcpoptstrip_mangle_packet(struct sk_buff *skb,
			  const struct xt_tcpoptstrip_target_info *info,
			  unsigned int tcphoff, unsigned int minlen)
{
	unsigned int optl, i, j;
	struct tcphdr *tcph;
	u_int16_t n, o;
	u_int8_t *opt;

	if (!skb_make_writable(skb, skb->len))
		return NF_DROP;

	tcph = (struct tcphdr *)(skb_network_header(skb) + tcphoff);
	opt  = (u_int8_t *)tcph;

	/*
	 * Walk through all TCP options - if we find some option to remove,
	 * set all octets to %TCPOPT_NOP and adjust checksum.
	 */
	for (i = sizeof(struct tcphdr); i < tcp_hdrlen(skb); i += optl) {
		optl = optlen(opt, i);

		if (i + optl > tcp_hdrlen(skb))
			break;

		if (!tcpoptstrip_test_bit(info->strip_bmap, opt[i]))
			continue;

		for (j = 0; j < optl; ++j) {
			o = opt[i+j];
			n = TCPOPT_NOP;
			if ((i + j) % 2 == 0) {
				o <<= 8;
				n <<= 8;
			}
			inet_proto_csum_replace2(&tcph->check, skb, htons(o),
						 htons(n), 0);
		}
		memset(opt + i, TCPOPT_NOP, optl);
	}

	return XT_CONTINUE;
}

static unsigned int
tcpoptstrip_tg4(struct sk_buff *skb, const struct xt_target_param *par)
{
	return tcpoptstrip_mangle_packet(skb, par->targinfo, ip_hdrlen(skb),
	       sizeof(struct iphdr) + sizeof(struct tcphdr));
}

#if defined(CONFIG_IP6_NF_MANGLE) || defined(CONFIG_IP6_NF_MANGLE_MODULE)
static unsigned int
tcpoptstrip_tg6(struct sk_buff *skb, const struct xt_target_param *par)
{
	struct ipv6hdr *ipv6h = ipv6_hdr(skb);
	int tcphoff;
	u_int8_t nexthdr;

	nexthdr = ipv6h->nexthdr;
	tcphoff = ipv6_skip_exthdr(skb, sizeof(*ipv6h), &nexthdr);
	if (tcphoff < 0)
		return NF_DROP;

	return tcpoptstrip_mangle_packet(skb, par->targinfo, tcphoff,
	       sizeof(*ipv6h) + sizeof(struct tcphdr));
}
#endif

static struct xt_target tcpoptstrip_tg_reg[] __read_mostly = {
	{
		.name       = "TCPOPTSTRIP",
		.family     = NFPROTO_IPV4,
		.table      = "mangle",
		.proto      = IPPROTO_TCP,
		.target     = tcpoptstrip_tg4,
		.targetsize = sizeof(struct xt_tcpoptstrip_target_info),
		.me         = THIS_MODULE,
	},
#if defined(CONFIG_IP6_NF_MANGLE) || defined(CONFIG_IP6_NF_MANGLE_MODULE)
	{
		.name       = "TCPOPTSTRIP",
		.family     = NFPROTO_IPV6,
		.table      = "mangle",
		.proto      = IPPROTO_TCP,
		.target     = tcpoptstrip_tg6,
		.targetsize = sizeof(struct xt_tcpoptstrip_target_info),
		.me         = THIS_MODULE,
	},
#endif
};

static int __init tcpoptstrip_tg_init(void)
{
	return xt_register_targets(tcpoptstrip_tg_reg,
				   ARRAY_SIZE(tcpoptstrip_tg_reg));
}

static void __exit tcpoptstrip_tg_exit(void)
{
	xt_unregister_targets(tcpoptstrip_tg_reg,
			      ARRAY_SIZE(tcpoptstrip_tg_reg));
}

module_init(tcpoptstrip_tg_init);
module_exit(tcpoptstrip_tg_exit);
MODULE_AUTHOR("Sven Schnelle <svens@bitebene.org>, Jan Engelhardt <jengelh@computergmbh.de>");
MODULE_DESCRIPTION("Xtables: TCP option stripping");
MODULE_LICENSE("GPL");
MODULE_ALIAS("ipt_TCPOPTSTRIP");
MODULE_ALIAS("ip6t_TCPOPTSTRIP");
