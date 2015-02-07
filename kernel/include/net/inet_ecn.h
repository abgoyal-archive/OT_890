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

#ifndef _INET_ECN_H_
#define _INET_ECN_H_

#include <linux/ip.h>
#include <linux/skbuff.h>

#include <net/inet_sock.h>
#include <net/dsfield.h>

enum {
	INET_ECN_NOT_ECT = 0,
	INET_ECN_ECT_1 = 1,
	INET_ECN_ECT_0 = 2,
	INET_ECN_CE = 3,
	INET_ECN_MASK = 3,
};

static inline int INET_ECN_is_ce(__u8 dsfield)
{
	return (dsfield & INET_ECN_MASK) == INET_ECN_CE;
}

static inline int INET_ECN_is_not_ect(__u8 dsfield)
{
	return (dsfield & INET_ECN_MASK) == INET_ECN_NOT_ECT;
}

static inline int INET_ECN_is_capable(__u8 dsfield)
{
	return (dsfield & INET_ECN_ECT_0);
}

static inline __u8 INET_ECN_encapsulate(__u8 outer, __u8 inner)
{
	outer &= ~INET_ECN_MASK;
	outer |= !INET_ECN_is_ce(inner) ? (inner & INET_ECN_MASK) :
					  INET_ECN_ECT_0;
	return outer;
}

#define	INET_ECN_xmit(sk) do { inet_sk(sk)->tos |= INET_ECN_ECT_0; } while (0)
#define	INET_ECN_dontxmit(sk) \
	do { inet_sk(sk)->tos &= ~INET_ECN_MASK; } while (0)

#define IP6_ECN_flow_init(label) do {		\
      (label) &= ~htonl(INET_ECN_MASK << 20);	\
    } while (0)

#define	IP6_ECN_flow_xmit(sk, label) do {				\
	if (INET_ECN_is_capable(inet6_sk(sk)->tclass))			\
		(label) |= htonl(INET_ECN_ECT_0 << 20);			\
    } while (0)

static inline int IP_ECN_set_ce(struct iphdr *iph)
{
	u32 check = (__force u32)iph->check;
	u32 ecn = (iph->tos + 1) & INET_ECN_MASK;

	/*
	 * After the last operation we have (in binary):
	 * INET_ECN_NOT_ECT => 01
	 * INET_ECN_ECT_1   => 10
	 * INET_ECN_ECT_0   => 11
	 * INET_ECN_CE      => 00
	 */
	if (!(ecn & 2))
		return !ecn;

	/*
	 * The following gives us:
	 * INET_ECN_ECT_1 => check += htons(0xFFFD)
	 * INET_ECN_ECT_0 => check += htons(0xFFFE)
	 */
	check += (__force u16)htons(0xFFFB) + (__force u16)htons(ecn);

	iph->check = (__force __sum16)(check + (check>=0xFFFF));
	iph->tos |= INET_ECN_CE;
	return 1;
}

static inline void IP_ECN_clear(struct iphdr *iph)
{
	iph->tos &= ~INET_ECN_MASK;
}

static inline void ipv4_copy_dscp(unsigned int dscp, struct iphdr *inner)
{
	dscp &= ~INET_ECN_MASK;
	ipv4_change_dsfield(inner, INET_ECN_MASK, dscp);
}

struct ipv6hdr;

static inline int IP6_ECN_set_ce(struct ipv6hdr *iph)
{
	if (INET_ECN_is_not_ect(ipv6_get_dsfield(iph)))
		return 0;
	*(__be32*)iph |= htonl(INET_ECN_CE << 20);
	return 1;
}

static inline void IP6_ECN_clear(struct ipv6hdr *iph)
{
	*(__be32*)iph &= ~htonl(INET_ECN_MASK << 20);
}

static inline void ipv6_copy_dscp(unsigned int dscp, struct ipv6hdr *inner)
{
	dscp &= ~INET_ECN_MASK;
	ipv6_change_dsfield(inner, INET_ECN_MASK, dscp);
}

static inline int INET_ECN_set_ce(struct sk_buff *skb)
{
	switch (skb->protocol) {
	case __constant_htons(ETH_P_IP):
		if (skb->network_header + sizeof(struct iphdr) <= skb->tail)
			return IP_ECN_set_ce(ip_hdr(skb));
		break;

	case __constant_htons(ETH_P_IPV6):
		if (skb->network_header + sizeof(struct ipv6hdr) <= skb->tail)
			return IP6_ECN_set_ce(ipv6_hdr(skb));
		break;
	}

	return 0;
}

#endif
