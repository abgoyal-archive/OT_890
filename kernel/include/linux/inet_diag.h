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

#ifndef _INET_DIAG_H_
#define _INET_DIAG_H_ 1

#include <linux/types.h>

/* Just some random number */
#define TCPDIAG_GETSOCK 18
#define DCCPDIAG_GETSOCK 19

#define INET_DIAG_GETSOCK_MAX 24

/* Socket identity */
struct inet_diag_sockid {
	__be16	idiag_sport;
	__be16	idiag_dport;
	__be32	idiag_src[4];
	__be32	idiag_dst[4];
	__u32	idiag_if;
	__u32	idiag_cookie[2];
#define INET_DIAG_NOCOOKIE (~0U)
};

/* Request structure */

struct inet_diag_req {
	__u8	idiag_family;		/* Family of addresses. */
	__u8	idiag_src_len;
	__u8	idiag_dst_len;
	__u8	idiag_ext;		/* Query extended information */

	struct inet_diag_sockid id;

	__u32	idiag_states;		/* States to dump */
	__u32	idiag_dbs;		/* Tables to dump (NI) */
};

enum {
	INET_DIAG_REQ_NONE,
	INET_DIAG_REQ_BYTECODE,
};

#define INET_DIAG_REQ_MAX INET_DIAG_REQ_BYTECODE

/* Bytecode is sequence of 4 byte commands followed by variable arguments.
 * All the commands identified by "code" are conditional jumps forward:
 * to offset cc+"yes" or to offset cc+"no". "yes" is supposed to be
 * length of the command and its arguments.
 */
 
struct inet_diag_bc_op {
	unsigned char	code;
	unsigned char	yes;
	unsigned short	no;
};

enum {
	INET_DIAG_BC_NOP,
	INET_DIAG_BC_JMP,
	INET_DIAG_BC_S_GE,
	INET_DIAG_BC_S_LE,
	INET_DIAG_BC_D_GE,
	INET_DIAG_BC_D_LE,
	INET_DIAG_BC_AUTO,
	INET_DIAG_BC_S_COND,
	INET_DIAG_BC_D_COND,
};

struct inet_diag_hostcond {
	__u8	family;
	__u8	prefix_len;
	int	port;
	__be32	addr[0];
};

/* Base info structure. It contains socket identity (addrs/ports/cookie)
 * and, alas, the information shown by netstat. */
struct inet_diag_msg {
	__u8	idiag_family;
	__u8	idiag_state;
	__u8	idiag_timer;
	__u8	idiag_retrans;

	struct inet_diag_sockid id;

	__u32	idiag_expires;
	__u32	idiag_rqueue;
	__u32	idiag_wqueue;
	__u32	idiag_uid;
	__u32	idiag_inode;
};

/* Extensions */

enum {
	INET_DIAG_NONE,
	INET_DIAG_MEMINFO,
	INET_DIAG_INFO,
	INET_DIAG_VEGASINFO,
	INET_DIAG_CONG,
};

#define INET_DIAG_MAX INET_DIAG_CONG


/* INET_DIAG_MEM */

struct inet_diag_meminfo {
	__u32	idiag_rmem;
	__u32	idiag_wmem;
	__u32	idiag_fmem;
	__u32	idiag_tmem;
};

/* INET_DIAG_VEGASINFO */

struct tcpvegas_info {
	__u32	tcpv_enabled;
	__u32	tcpv_rttcnt;
	__u32	tcpv_rtt;
	__u32	tcpv_minrtt;
};

#ifdef __KERNEL__
struct sock;
struct inet_hashinfo;

struct inet_diag_handler {
	struct inet_hashinfo    *idiag_hashinfo;
	void			(*idiag_get_info)(struct sock *sk,
						  struct inet_diag_msg *r,
						  void *info);
	__u16                   idiag_info_size;
	__u16                   idiag_type;
};

extern int  inet_diag_register(const struct inet_diag_handler *handler);
extern void inet_diag_unregister(const struct inet_diag_handler *handler);
#endif /* __KERNEL__ */

#endif /* _INET_DIAG_H_ */
