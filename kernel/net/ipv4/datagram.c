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
 *	common UDP/RAW code
 *	Linux INET implementation
 *
 * Authors:
 * 	Hideaki YOSHIFUJI <yoshfuji@linux-ipv6.org>
 *
 * 	This program is free software; you can redistribute it and/or
 * 	modify it under the terms of the GNU General Public License
 * 	as published by the Free Software Foundation; either version
 * 	2 of the License, or (at your option) any later version.
 */

#include <linux/types.h>
#include <linux/module.h>
#include <linux/ip.h>
#include <linux/in.h>
#include <net/ip.h>
#include <net/sock.h>
#include <net/route.h>
#include <net/tcp_states.h>

int ip4_datagram_connect(struct sock *sk, struct sockaddr *uaddr, int addr_len)
{
	struct inet_sock *inet = inet_sk(sk);
	struct sockaddr_in *usin = (struct sockaddr_in *) uaddr;
	struct rtable *rt;
	__be32 saddr;
	int oif;
	int err;


	if (addr_len < sizeof(*usin))
		return -EINVAL;

	if (usin->sin_family != AF_INET)
		return -EAFNOSUPPORT;

	sk_dst_reset(sk);

	oif = sk->sk_bound_dev_if;
	saddr = inet->saddr;
	if (ipv4_is_multicast(usin->sin_addr.s_addr)) {
		if (!oif)
			oif = inet->mc_index;
		if (!saddr)
			saddr = inet->mc_addr;
	}
	err = ip_route_connect(&rt, usin->sin_addr.s_addr, saddr,
			       RT_CONN_FLAGS(sk), oif,
			       sk->sk_protocol,
			       inet->sport, usin->sin_port, sk, 1);
	if (err) {
		if (err == -ENETUNREACH)
			IP_INC_STATS_BH(sock_net(sk), IPSTATS_MIB_OUTNOROUTES);
		return err;
	}

	if ((rt->rt_flags & RTCF_BROADCAST) && !sock_flag(sk, SOCK_BROADCAST)) {
		ip_rt_put(rt);
		return -EACCES;
	}
	if (!inet->saddr)
		inet->saddr = rt->rt_src;	/* Update source address */
	if (!inet->rcv_saddr)
		inet->rcv_saddr = rt->rt_src;
	inet->daddr = rt->rt_dst;
	inet->dport = usin->sin_port;
	sk->sk_state = TCP_ESTABLISHED;
	inet->id = jiffies;

	sk_dst_set(sk, &rt->u.dst);
	return(0);
}

EXPORT_SYMBOL(ip4_datagram_connect);

