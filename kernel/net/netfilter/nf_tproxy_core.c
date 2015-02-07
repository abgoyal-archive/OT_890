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
 * Transparent proxy support for Linux/iptables
 *
 * Copyright (c) 2006-2007 BalaBit IT Ltd.
 * Author: Balazs Scheidler, Krisztian Kovacs
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 */

#include <linux/module.h>

#include <linux/net.h>
#include <linux/if.h>
#include <linux/netdevice.h>
#include <net/udp.h>
#include <net/netfilter/nf_tproxy_core.h>

struct sock *
nf_tproxy_get_sock_v4(struct net *net, const u8 protocol,
		      const __be32 saddr, const __be32 daddr,
		      const __be16 sport, const __be16 dport,
		      const struct net_device *in, bool listening_only)
{
	struct sock *sk;

	/* look up socket */
	switch (protocol) {
	case IPPROTO_TCP:
		if (listening_only)
			sk = __inet_lookup_listener(net, &tcp_hashinfo,
						    daddr, ntohs(dport),
						    in->ifindex);
		else
			sk = __inet_lookup(net, &tcp_hashinfo,
					   saddr, sport, daddr, dport,
					   in->ifindex);
		break;
	case IPPROTO_UDP:
		sk = udp4_lib_lookup(net, saddr, sport, daddr, dport,
				     in->ifindex);
		break;
	default:
		WARN_ON(1);
		sk = NULL;
	}

	pr_debug("tproxy socket lookup: proto %u %08x:%u -> %08x:%u, listener only: %d, sock %p\n",
		 protocol, ntohl(saddr), ntohs(sport), ntohl(daddr), ntohs(dport), listening_only, sk);

	return sk;
}
EXPORT_SYMBOL_GPL(nf_tproxy_get_sock_v4);

static void
nf_tproxy_destructor(struct sk_buff *skb)
{
	struct sock *sk = skb->sk;

	skb->sk = NULL;
	skb->destructor = NULL;

	if (sk)
		nf_tproxy_put_sock(sk);
}

/* consumes sk */
int
nf_tproxy_assign_sock(struct sk_buff *skb, struct sock *sk)
{
	if (inet_sk(sk)->transparent) {
		skb->sk = sk;
		skb->destructor = nf_tproxy_destructor;
		return 1;
	} else
		nf_tproxy_put_sock(sk);

	return 0;
}
EXPORT_SYMBOL_GPL(nf_tproxy_assign_sock);

static int __init nf_tproxy_init(void)
{
	pr_info("NF_TPROXY: Transparent proxy support initialized, version 4.1.0\n");
	pr_info("NF_TPROXY: Copyright (c) 2006-2007 BalaBit IT Ltd.\n");
	return 0;
}

module_init(nf_tproxy_init);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Krisztian Kovacs");
MODULE_DESCRIPTION("Transparent proxy support core routines");
