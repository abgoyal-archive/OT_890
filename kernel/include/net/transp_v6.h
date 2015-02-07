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

#ifndef _TRANSP_V6_H
#define _TRANSP_V6_H

#include <net/checksum.h>

/*
 *	IPv6 transport protocols
 */

#ifdef __KERNEL__

extern struct proto rawv6_prot;
extern struct proto udpv6_prot;
extern struct proto udplitev6_prot;
extern struct proto tcpv6_prot;

struct flowi;

/* extention headers */
extern int				ipv6_exthdrs_init(void);
extern void				ipv6_exthdrs_exit(void);
extern int				ipv6_frag_init(void);
extern void				ipv6_frag_exit(void);

/* transport protocols */
extern int				rawv6_init(void);
extern void				rawv6_exit(void);
extern int				udpv6_init(void);
extern void				udpv6_exit(void);
extern int 				udplitev6_init(void);
extern void 				udplitev6_exit(void);
extern int				tcpv6_init(void);
extern void				tcpv6_exit(void);

extern int				udpv6_connect(struct sock *sk,
						      struct sockaddr *uaddr,
						      int addr_len);

extern int			datagram_recv_ctl(struct sock *sk,
						  struct msghdr *msg,
						  struct sk_buff *skb);

extern int			datagram_send_ctl(struct net *net,
						  struct msghdr *msg,
						  struct flowi *fl,
						  struct ipv6_txoptions *opt,
						  int *hlimit, int *tclass);

#define		LOOPBACK4_IPV6		__constant_htonl(0x7f000006)

/*
 *	address family specific functions
 */
extern struct inet_connection_sock_af_ops ipv4_specific;

extern void inet6_destroy_sock(struct sock *sk);

#endif

#endif
