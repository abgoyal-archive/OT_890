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

#ifndef _CCID_H
#define _CCID_H
/*
 *  net/dccp/ccid.h
 *
 *  An implementation of the DCCP protocol
 *  Arnaldo Carvalho de Melo <acme@conectiva.com.br>
 *
 *  CCID infrastructure
 *
 *	This program is free software; you can redistribute it and/or modify it
 *	under the terms of the GNU General Public License version 2 as
 *	published by the Free Software Foundation.
 */

#include <net/sock.h>
#include <linux/compiler.h>
#include <linux/dccp.h>
#include <linux/list.h>
#include <linux/module.h>

#define CCID_MAX 255

struct tcp_info;

/**
 *  struct ccid_operations  -  Interface to Congestion-Control Infrastructure
 *
 *  @ccid_id: numerical CCID ID (up to %CCID_MAX, cf. table 5 in RFC 4340, 10.)
 *  @ccid_ccmps: the CCMPS including network/transport headers (0 when disabled)
 *  @ccid_name: alphabetical identifier string for @ccid_id
 *  @ccid_hc_{r,t}x_slab: memory pool for the receiver/sender half-connection
 *  @ccid_hc_{r,t}x_obj_size: size of the receiver/sender half-connection socket
 *
 *  @ccid_hc_{r,t}x_init: CCID-specific initialisation routine (before startup)
 *  @ccid_hc_{r,t}x_exit: CCID-specific cleanup routine (before destruction)
 *  @ccid_hc_rx_packet_recv: implements the HC-receiver side
 *  @ccid_hc_{r,t}x_parse_options: parsing routine for CCID/HC-specific options
 *  @ccid_hc_{r,t}x_insert_options: insert routine for CCID/HC-specific options
 *  @ccid_hc_tx_packet_recv: implements feedback processing for the HC-sender
 *  @ccid_hc_tx_send_packet: implements the sending part of the HC-sender
 *  @ccid_hc_tx_packet_sent: does accounting for packets in flight by HC-sender
 *  @ccid_hc_{r,t}x_get_info: INET_DIAG information for HC-receiver/sender
 *  @ccid_hc_{r,t}x_getsockopt: socket options specific to HC-receiver/sender
 */
struct ccid_operations {
	unsigned char		ccid_id;
	__u32			ccid_ccmps;
	const char		*ccid_name;
	struct kmem_cache	*ccid_hc_rx_slab,
				*ccid_hc_tx_slab;
	__u32			ccid_hc_rx_obj_size,
				ccid_hc_tx_obj_size;
	/* Interface Routines */
	int		(*ccid_hc_rx_init)(struct ccid *ccid, struct sock *sk);
	int		(*ccid_hc_tx_init)(struct ccid *ccid, struct sock *sk);
	void		(*ccid_hc_rx_exit)(struct sock *sk);
	void		(*ccid_hc_tx_exit)(struct sock *sk);
	void		(*ccid_hc_rx_packet_recv)(struct sock *sk,
						  struct sk_buff *skb);
	int		(*ccid_hc_rx_parse_options)(struct sock *sk,
						    unsigned char option,
						    unsigned char len, u16 idx,
						    unsigned char* value);
	int		(*ccid_hc_rx_insert_options)(struct sock *sk,
						     struct sk_buff *skb);
	void		(*ccid_hc_tx_packet_recv)(struct sock *sk,
						  struct sk_buff *skb);
	int		(*ccid_hc_tx_parse_options)(struct sock *sk,
						    unsigned char option,
						    unsigned char len, u16 idx,
						    unsigned char* value);
	int		(*ccid_hc_tx_send_packet)(struct sock *sk,
						  struct sk_buff *skb);
	void		(*ccid_hc_tx_packet_sent)(struct sock *sk,
						  int more, unsigned int len);
	void		(*ccid_hc_rx_get_info)(struct sock *sk,
					       struct tcp_info *info);
	void		(*ccid_hc_tx_get_info)(struct sock *sk,
					       struct tcp_info *info);
	int		(*ccid_hc_rx_getsockopt)(struct sock *sk,
						 const int optname, int len,
						 u32 __user *optval,
						 int __user *optlen);
	int		(*ccid_hc_tx_getsockopt)(struct sock *sk,
						 const int optname, int len,
						 u32 __user *optval,
						 int __user *optlen);
};

extern struct ccid_operations ccid2_ops;
#ifdef CONFIG_IP_DCCP_CCID3
extern struct ccid_operations ccid3_ops;
#endif

extern int  ccid_initialize_builtins(void);
extern void ccid_cleanup_builtins(void);

struct ccid {
	struct ccid_operations *ccid_ops;
	char		       ccid_priv[0];
};

static inline void *ccid_priv(const struct ccid *ccid)
{
	return (void *)ccid->ccid_priv;
}

extern bool ccid_support_check(u8 const *ccid_array, u8 array_len);
extern int  ccid_get_builtin_ccids(u8 **ccid_array, u8 *array_len);
extern int  ccid_getsockopt_builtin_ccids(struct sock *sk, int len,
					  char __user *, int __user *);

extern struct ccid *ccid_new(const u8 id, struct sock *sk, bool rx);

static inline int ccid_get_current_rx_ccid(struct dccp_sock *dp)
{
	struct ccid *ccid = dp->dccps_hc_rx_ccid;

	if (ccid == NULL || ccid->ccid_ops == NULL)
		return -1;
	return ccid->ccid_ops->ccid_id;
}

static inline int ccid_get_current_tx_ccid(struct dccp_sock *dp)
{
	struct ccid *ccid = dp->dccps_hc_tx_ccid;

	if (ccid == NULL || ccid->ccid_ops == NULL)
		return -1;
	return ccid->ccid_ops->ccid_id;
}

extern void ccid_hc_rx_delete(struct ccid *ccid, struct sock *sk);
extern void ccid_hc_tx_delete(struct ccid *ccid, struct sock *sk);

static inline int ccid_hc_tx_send_packet(struct ccid *ccid, struct sock *sk,
					 struct sk_buff *skb)
{
	int rc = 0;
	if (ccid->ccid_ops->ccid_hc_tx_send_packet != NULL)
		rc = ccid->ccid_ops->ccid_hc_tx_send_packet(sk, skb);
	return rc;
}

static inline void ccid_hc_tx_packet_sent(struct ccid *ccid, struct sock *sk,
					  int more, unsigned int len)
{
	if (ccid->ccid_ops->ccid_hc_tx_packet_sent != NULL)
		ccid->ccid_ops->ccid_hc_tx_packet_sent(sk, more, len);
}

static inline void ccid_hc_rx_packet_recv(struct ccid *ccid, struct sock *sk,
					  struct sk_buff *skb)
{
	if (ccid->ccid_ops->ccid_hc_rx_packet_recv != NULL)
		ccid->ccid_ops->ccid_hc_rx_packet_recv(sk, skb);
}

static inline void ccid_hc_tx_packet_recv(struct ccid *ccid, struct sock *sk,
					  struct sk_buff *skb)
{
	if (ccid->ccid_ops->ccid_hc_tx_packet_recv != NULL)
		ccid->ccid_ops->ccid_hc_tx_packet_recv(sk, skb);
}

static inline int ccid_hc_tx_parse_options(struct ccid *ccid, struct sock *sk,
					   unsigned char option,
					   unsigned char len, u16 idx,
					   unsigned char* value)
{
	int rc = 0;
	if (ccid->ccid_ops->ccid_hc_tx_parse_options != NULL)
		rc = ccid->ccid_ops->ccid_hc_tx_parse_options(sk, option, len, idx,
						    value);
	return rc;
}

static inline int ccid_hc_rx_parse_options(struct ccid *ccid, struct sock *sk,
					   unsigned char option,
					   unsigned char len, u16 idx,
					   unsigned char* value)
{
	int rc = 0;
	if (ccid->ccid_ops->ccid_hc_rx_parse_options != NULL)
		rc = ccid->ccid_ops->ccid_hc_rx_parse_options(sk, option, len, idx, value);
	return rc;
}

static inline int ccid_hc_rx_insert_options(struct ccid *ccid, struct sock *sk,
					    struct sk_buff *skb)
{
	if (ccid->ccid_ops->ccid_hc_rx_insert_options != NULL)
		return ccid->ccid_ops->ccid_hc_rx_insert_options(sk, skb);
	return 0;
}

static inline void ccid_hc_rx_get_info(struct ccid *ccid, struct sock *sk,
				       struct tcp_info *info)
{
	if (ccid->ccid_ops->ccid_hc_rx_get_info != NULL)
		ccid->ccid_ops->ccid_hc_rx_get_info(sk, info);
}

static inline void ccid_hc_tx_get_info(struct ccid *ccid, struct sock *sk,
				       struct tcp_info *info)
{
	if (ccid->ccid_ops->ccid_hc_tx_get_info != NULL)
		ccid->ccid_ops->ccid_hc_tx_get_info(sk, info);
}

static inline int ccid_hc_rx_getsockopt(struct ccid *ccid, struct sock *sk,
					const int optname, int len,
					u32 __user *optval, int __user *optlen)
{
	int rc = -ENOPROTOOPT;
	if (ccid->ccid_ops->ccid_hc_rx_getsockopt != NULL)
		rc = ccid->ccid_ops->ccid_hc_rx_getsockopt(sk, optname, len,
						 optval, optlen);
	return rc;
}

static inline int ccid_hc_tx_getsockopt(struct ccid *ccid, struct sock *sk,
					const int optname, int len,
					u32 __user *optval, int __user *optlen)
{
	int rc = -ENOPROTOOPT;
	if (ccid->ccid_ops->ccid_hc_tx_getsockopt != NULL)
		rc = ccid->ccid_ops->ccid_hc_tx_getsockopt(sk, optname, len,
						 optval, optlen);
	return rc;
}
#endif /* _CCID_H */
