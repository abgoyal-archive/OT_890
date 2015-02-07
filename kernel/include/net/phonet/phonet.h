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
 * File: af_phonet.h
 *
 * Phonet sockets kernel definitions
 *
 * Copyright (C) 2008 Nokia Corporation.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA
 * 02110-1301 USA
 */

#ifndef AF_PHONET_H
#define AF_PHONET_H

/*
 * The lower layers may not require more space, ever. Make sure it's
 * enough.
 */
#define MAX_PHONET_HEADER	(8 + MAX_HEADER)

/*
 * Every Phonet* socket has this structure first in its
 * protocol-specific structure under name c.
 */
struct pn_sock {
	struct sock	sk;
	u16		sobject;
	u8		resource;
};

static inline struct pn_sock *pn_sk(struct sock *sk)
{
	return (struct pn_sock *)sk;
}

extern const struct proto_ops phonet_dgram_ops;

struct sock *pn_find_sock_by_sa(struct net *net, const struct sockaddr_pn *sa);
void phonet_get_local_port_range(int *min, int *max);
void pn_sock_hash(struct sock *sk);
void pn_sock_unhash(struct sock *sk);
int pn_sock_get_port(struct sock *sk, unsigned short sport);

int pn_skb_send(struct sock *sk, struct sk_buff *skb,
		const struct sockaddr_pn *target);

static inline struct phonethdr *pn_hdr(struct sk_buff *skb)
{
	return (struct phonethdr *)skb_network_header(skb);
}

static inline struct phonetmsg *pn_msg(struct sk_buff *skb)
{
	return (struct phonetmsg *)skb_transport_header(skb);
}

/*
 * Get the other party's sockaddr from received skb. The skb begins
 * with a Phonet header.
 */
static inline
void pn_skb_get_src_sockaddr(struct sk_buff *skb, struct sockaddr_pn *sa)
{
	struct phonethdr *ph = pn_hdr(skb);
	u16 obj = pn_object(ph->pn_sdev, ph->pn_sobj);

	sa->spn_family = AF_PHONET;
	pn_sockaddr_set_object(sa, obj);
	pn_sockaddr_set_resource(sa, ph->pn_res);
	memset(sa->spn_zero, 0, sizeof(sa->spn_zero));
}

static inline
void pn_skb_get_dst_sockaddr(struct sk_buff *skb, struct sockaddr_pn *sa)
{
	struct phonethdr *ph = pn_hdr(skb);
	u16 obj = pn_object(ph->pn_rdev, ph->pn_robj);

	sa->spn_family = AF_PHONET;
	pn_sockaddr_set_object(sa, obj);
	pn_sockaddr_set_resource(sa, ph->pn_res);
	memset(sa->spn_zero, 0, sizeof(sa->spn_zero));
}

/* Protocols in Phonet protocol family. */
struct phonet_protocol {
	const struct proto_ops	*ops;
	struct proto		*prot;
	int			sock_type;
};

int phonet_proto_register(int protocol, struct phonet_protocol *pp);
void phonet_proto_unregister(int protocol, struct phonet_protocol *pp);

int phonet_sysctl_init(void);
void phonet_sysctl_exit(void);
void phonet_netlink_register(void);
int isi_register(void);
void isi_unregister(void);

#endif
