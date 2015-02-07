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
 *  UDPLITEv6   An implementation of the UDP-Lite protocol over IPv6.
 *              See also net/ipv4/udplite.c
 *
 *  Authors:    Gerrit Renker       <gerrit@erg.abdn.ac.uk>
 *
 *  Changes:
 *  Fixes:
 *		This program is free software; you can redistribute it and/or
 *		modify it under the terms of the GNU General Public License
 *		as published by the Free Software Foundation; either version
 *		2 of the License, or (at your option) any later version.
 */
#include "udp_impl.h"

static int udplitev6_rcv(struct sk_buff *skb)
{
	return __udp6_lib_rcv(skb, &udplite_table, IPPROTO_UDPLITE);
}

static void udplitev6_err(struct sk_buff *skb,
			  struct inet6_skb_parm *opt,
			  int type, int code, int offset, __be32 info)
{
	__udp6_lib_err(skb, opt, type, code, offset, info, &udplite_table);
}

static struct inet6_protocol udplitev6_protocol = {
	.handler	=	udplitev6_rcv,
	.err_handler	=	udplitev6_err,
	.flags		=	INET6_PROTO_NOPOLICY|INET6_PROTO_FINAL,
};

struct proto udplitev6_prot = {
	.name		   = "UDPLITEv6",
	.owner		   = THIS_MODULE,
	.close		   = udp_lib_close,
	.connect	   = ip6_datagram_connect,
	.disconnect	   = udp_disconnect,
	.ioctl		   = udp_ioctl,
	.init		   = udplite_sk_init,
	.destroy	   = udpv6_destroy_sock,
	.setsockopt	   = udpv6_setsockopt,
	.getsockopt	   = udpv6_getsockopt,
	.sendmsg	   = udpv6_sendmsg,
	.recvmsg	   = udpv6_recvmsg,
	.backlog_rcv	   = udpv6_queue_rcv_skb,
	.hash		   = udp_lib_hash,
	.unhash		   = udp_lib_unhash,
	.get_port	   = udp_v6_get_port,
	.obj_size	   = sizeof(struct udp6_sock),
	.slab_flags	   = SLAB_DESTROY_BY_RCU,
	.h.udp_table	   = &udplite_table,
#ifdef CONFIG_COMPAT
	.compat_setsockopt = compat_udpv6_setsockopt,
	.compat_getsockopt = compat_udpv6_getsockopt,
#endif
};

static struct inet_protosw udplite6_protosw = {
	.type		= SOCK_DGRAM,
	.protocol	= IPPROTO_UDPLITE,
	.prot		= &udplitev6_prot,
	.ops		= &inet6_dgram_ops,
	.capability	= -1,
	.no_check	= 0,
	.flags		= INET_PROTOSW_PERMANENT,
};

int __init udplitev6_init(void)
{
	int ret;

	ret = inet6_add_protocol(&udplitev6_protocol, IPPROTO_UDPLITE);
	if (ret)
		goto out;

	ret = inet6_register_protosw(&udplite6_protosw);
	if (ret)
		goto out_udplitev6_protocol;
out:
	return ret;

out_udplitev6_protocol:
	inet6_del_protocol(&udplitev6_protocol, IPPROTO_UDPLITE);
	goto out;
}

void udplitev6_exit(void)
{
	inet6_unregister_protosw(&udplite6_protosw);
	inet6_del_protocol(&udplitev6_protocol, IPPROTO_UDPLITE);
}

#ifdef CONFIG_PROC_FS
static struct udp_seq_afinfo udplite6_seq_afinfo = {
	.name		= "udplite6",
	.family		= AF_INET6,
	.udp_table	= &udplite_table,
	.seq_fops	= {
		.owner	=	THIS_MODULE,
	},
	.seq_ops	= {
		.show		= udp6_seq_show,
	},
};

static int udplite6_proc_init_net(struct net *net)
{
	return udp_proc_register(net, &udplite6_seq_afinfo);
}

static void udplite6_proc_exit_net(struct net *net)
{
	udp_proc_unregister(net, &udplite6_seq_afinfo);
}

static struct pernet_operations udplite6_net_ops = {
	.init = udplite6_proc_init_net,
	.exit = udplite6_proc_exit_net,
};

int __init udplite6_proc_init(void)
{
	return register_pernet_subsys(&udplite6_net_ops);
}

void udplite6_proc_exit(void)
{
	unregister_pernet_subsys(&udplite6_net_ops);
}
#endif
