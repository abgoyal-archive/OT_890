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
 * linux/include/linux/sunrpc/auth_gss.h
 *
 * Declarations for RPCSEC_GSS
 *
 * Dug Song <dugsong@monkey.org>
 * Andy Adamson <andros@umich.edu>
 * Bruce Fields <bfields@umich.edu>
 * Copyright (c) 2000 The Regents of the University of Michigan
 */

#ifndef _LINUX_SUNRPC_AUTH_GSS_H
#define _LINUX_SUNRPC_AUTH_GSS_H

#ifdef __KERNEL__
#include <linux/sunrpc/auth.h>
#include <linux/sunrpc/svc.h>
#include <linux/sunrpc/gss_api.h>

#define RPC_GSS_VERSION		1

#define MAXSEQ 0x80000000 /* maximum legal sequence number, from rfc 2203 */

enum rpc_gss_proc {
	RPC_GSS_PROC_DATA = 0,
	RPC_GSS_PROC_INIT = 1,
	RPC_GSS_PROC_CONTINUE_INIT = 2,
	RPC_GSS_PROC_DESTROY = 3
};

enum rpc_gss_svc {
	RPC_GSS_SVC_NONE = 1,
	RPC_GSS_SVC_INTEGRITY = 2,
	RPC_GSS_SVC_PRIVACY = 3
};

/* on-the-wire gss cred: */
struct rpc_gss_wire_cred {
	u32			gc_v;		/* version */
	u32			gc_proc;	/* control procedure */
	u32			gc_seq;		/* sequence number */
	u32			gc_svc;		/* service */
	struct xdr_netobj	gc_ctx;		/* context handle */
};

/* on-the-wire gss verifier: */
struct rpc_gss_wire_verf {
	u32			gv_flavor;
	struct xdr_netobj	gv_verf;
};

/* return from gss NULL PROC init sec context */
struct rpc_gss_init_res {
	struct xdr_netobj	gr_ctx;		/* context handle */
	u32			gr_major;	/* major status */
	u32			gr_minor;	/* minor status */
	u32			gr_win;		/* sequence window */
	struct xdr_netobj	gr_token;	/* token */
};

/* The gss_cl_ctx struct holds all the information the rpcsec_gss client
 * code needs to know about a single security context.  In particular,
 * gc_gss_ctx is the context handle that is used to do gss-api calls, while
 * gc_wire_ctx is the context handle that is used to identify the context on
 * the wire when communicating with a server. */

struct gss_cl_ctx {
	atomic_t		count;
	enum rpc_gss_proc	gc_proc;
	u32			gc_seq;
	spinlock_t		gc_seq_lock;
	struct gss_ctx		*gc_gss_ctx;
	struct xdr_netobj	gc_wire_ctx;
	u32			gc_win;
	unsigned long		gc_expiry;
	struct rcu_head		gc_rcu;
};

struct gss_upcall_msg;
struct gss_cred {
	struct rpc_cred		gc_base;
	enum rpc_gss_svc	gc_service;
	struct gss_cl_ctx	*gc_ctx;
	struct gss_upcall_msg	*gc_upcall;
	unsigned char		gc_machine_cred : 1;
};

#endif /* __KERNEL__ */
#endif /* _LINUX_SUNRPC_AUTH_GSS_H */

