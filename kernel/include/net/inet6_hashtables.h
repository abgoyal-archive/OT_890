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
 * INET		An implementation of the TCP/IP protocol suite for the LINUX
 *		operating system.  INET is implemented using the BSD Socket
 *		interface as the means of communication with the user level.
 *
 * Authors:	Lotsa people, from code originally in tcp
 *
 *	This program is free software; you can redistribute it and/or
 *      modify it under the terms of the GNU General Public License
 *      as published by the Free Software Foundation; either version
 *      2 of the License, or (at your option) any later version.
 */

#ifndef _INET6_HASHTABLES_H
#define _INET6_HASHTABLES_H


#if defined(CONFIG_IPV6) || defined (CONFIG_IPV6_MODULE)
#include <linux/in6.h>
#include <linux/ipv6.h>
#include <linux/types.h>
#include <linux/jhash.h>

#include <net/inet_sock.h>

#include <net/ipv6.h>
#include <net/netns/hash.h>

struct inet_hashinfo;

/* I have no idea if this is a good hash for v6 or not. -DaveM */
static inline unsigned int inet6_ehashfn(struct net *net,
				const struct in6_addr *laddr, const u16 lport,
				const struct in6_addr *faddr, const __be16 fport)
{
	u32 ports = (lport ^ (__force u16)fport);

	return jhash_3words((__force u32)laddr->s6_addr32[3],
			    (__force u32)faddr->s6_addr32[3],
			    ports, inet_ehash_secret + net_hash_mix(net));
}

static inline int inet6_sk_ehashfn(const struct sock *sk)
{
	const struct inet_sock *inet = inet_sk(sk);
	const struct ipv6_pinfo *np = inet6_sk(sk);
	const struct in6_addr *laddr = &np->rcv_saddr;
	const struct in6_addr *faddr = &np->daddr;
	const __u16 lport = inet->num;
	const __be16 fport = inet->dport;
	struct net *net = sock_net(sk);

	return inet6_ehashfn(net, laddr, lport, faddr, fport);
}

extern void __inet6_hash(struct sock *sk);

/*
 * Sockets in TCP_CLOSE state are _always_ taken out of the hash, so
 * we need not check it for TCP lookups anymore, thanks Alexey. -DaveM
 *
 * The sockhash lock must be held as a reader here.
 */
extern struct sock *__inet6_lookup_established(struct net *net,
					   struct inet_hashinfo *hashinfo,
					   const struct in6_addr *saddr,
					   const __be16 sport,
					   const struct in6_addr *daddr,
					   const u16 hnum,
					   const int dif);

extern struct sock *inet6_lookup_listener(struct net *net,
					  struct inet_hashinfo *hashinfo,
					  const struct in6_addr *daddr,
					  const unsigned short hnum,
					  const int dif);

static inline struct sock *__inet6_lookup(struct net *net,
					  struct inet_hashinfo *hashinfo,
					  const struct in6_addr *saddr,
					  const __be16 sport,
					  const struct in6_addr *daddr,
					  const u16 hnum,
					  const int dif)
{
	struct sock *sk = __inet6_lookup_established(net, hashinfo, saddr,
						sport, daddr, hnum, dif);
	if (sk)
		return sk;

	return inet6_lookup_listener(net, hashinfo, daddr, hnum, dif);
}

static inline struct sock *__inet6_lookup_skb(struct inet_hashinfo *hashinfo,
					      struct sk_buff *skb,
					      const __be16 sport,
					      const __be16 dport)
{
	struct sock *sk;

	if (unlikely(sk = skb_steal_sock(skb)))
		return sk;
	else return __inet6_lookup(dev_net(skb->dst->dev), hashinfo,
				   &ipv6_hdr(skb)->saddr, sport,
				   &ipv6_hdr(skb)->daddr, ntohs(dport),
				   inet6_iif(skb));
}

extern struct sock *inet6_lookup(struct net *net, struct inet_hashinfo *hashinfo,
				 const struct in6_addr *saddr, const __be16 sport,
				 const struct in6_addr *daddr, const __be16 dport,
				 const int dif);
#endif /* defined(CONFIG_IPV6) || defined (CONFIG_IPV6_MODULE) */
#endif /* _INET6_HASHTABLES_H */
