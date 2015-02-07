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

/* RxRPC kernel service interface definitions
 *
 * Copyright (C) 2007 Red Hat, Inc. All Rights Reserved.
 * Written by David Howells (dhowells@redhat.com)
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version
 * 2 of the License, or (at your option) any later version.
 */

#ifndef _NET_RXRPC_H
#define _NET_RXRPC_H

#ifdef __KERNEL__

#include <linux/rxrpc.h>

struct rxrpc_call;

/*
 * the mark applied to socket buffers that may be intercepted
 */
enum {
	RXRPC_SKB_MARK_DATA,		/* data message */
	RXRPC_SKB_MARK_FINAL_ACK,	/* final ACK received message */
	RXRPC_SKB_MARK_BUSY,		/* server busy message */
	RXRPC_SKB_MARK_REMOTE_ABORT,	/* remote abort message */
	RXRPC_SKB_MARK_NET_ERROR,	/* network error message */
	RXRPC_SKB_MARK_LOCAL_ERROR,	/* local error message */
	RXRPC_SKB_MARK_NEW_CALL,	/* local error message */
};

typedef void (*rxrpc_interceptor_t)(struct sock *, unsigned long,
				    struct sk_buff *);
extern void rxrpc_kernel_intercept_rx_messages(struct socket *,
					       rxrpc_interceptor_t);
extern struct rxrpc_call *rxrpc_kernel_begin_call(struct socket *,
						  struct sockaddr_rxrpc *,
						  struct key *,
						  unsigned long,
						  gfp_t);
extern int rxrpc_kernel_send_data(struct rxrpc_call *, struct msghdr *,
				  size_t);
extern void rxrpc_kernel_abort_call(struct rxrpc_call *, u32);
extern void rxrpc_kernel_end_call(struct rxrpc_call *);
extern bool rxrpc_kernel_is_data_last(struct sk_buff *);
extern u32 rxrpc_kernel_get_abort_code(struct sk_buff *);
extern int rxrpc_kernel_get_error_number(struct sk_buff *);
extern void rxrpc_kernel_data_delivered(struct sk_buff *);
extern void rxrpc_kernel_free_skb(struct sk_buff *);
extern struct rxrpc_call *rxrpc_kernel_accept_call(struct socket *,
						   unsigned long);
extern int rxrpc_kernel_reject_call(struct socket *);

#endif /* __KERNEL__ */
#endif /* _NET_RXRPC_H */
