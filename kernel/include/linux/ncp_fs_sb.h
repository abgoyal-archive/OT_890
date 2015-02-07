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
 *  ncp_fs_sb.h
 *
 *  Copyright (C) 1995, 1996 by Volker Lendecke
 *
 */

#ifndef _NCP_FS_SB
#define _NCP_FS_SB

#include <linux/types.h>
#include <linux/ncp_mount.h>
#include <linux/net.h>
#include <linux/mutex.h>

#ifdef __KERNEL__

#include <linux/workqueue.h>

#define NCP_DEFAULT_OPTIONS 0		/* 2 for packet signatures */

struct sock;

struct ncp_server {

	struct ncp_mount_data_kernel m;	/* Nearly all of the mount data is of
					   interest for us later, so we store
					   it completely. */

	__u8 name_space[NCP_NUMBER_OF_VOLUMES + 2];

	struct file *ncp_filp;	/* File pointer to ncp socket */
	struct socket *ncp_sock;/* ncp socket */
	struct file *info_filp;
	struct socket *info_sock;

	u8 sequence;
	u8 task;
	u16 connection;		/* Remote connection number */

	u8 completion;		/* Status message from server */
	u8 conn_status;		/* Bit 4 = 1 ==> Server going down, no
				   requests allowed anymore.
				   Bit 0 = 1 ==> Server is down. */

	int buffer_size;	/* Negotiated bufsize */

	int reply_size;		/* Size of last reply */

	int packet_size;
	unsigned char *packet;	/* Here we prepare requests and
				   receive replies */
	unsigned char *txbuf;	/* Storage for current request */
	unsigned char *rxbuf;	/* Storage for reply to current request */

	int lock;		/* To prevent mismatch in protocols. */
	struct mutex mutex;

	int current_size;	/* for packet preparation */
	int has_subfunction;
	int ncp_reply_size;

	int root_setuped;

	/* info for packet signing */
	int sign_wanted;	/* 1=Server needs signed packets */
	int sign_active;	/* 0=don't do signing, 1=do */
	char sign_root[8];	/* generated from password and encr. key */
	char sign_last[16];	

	/* Authentication info: NDS or BINDERY, username */
	struct {
		int	auth_type;
		size_t	object_name_len;
		void*	object_name;
		int	object_type;
	} auth;
	/* Password info */
	struct {
		size_t	len;
		void*	data;
	} priv;

	/* nls info: codepage for volume and charset for I/O */
	struct nls_table *nls_vol;
	struct nls_table *nls_io;

	/* maximum age in jiffies */
	int dentry_ttl;

	/* miscellaneous */
	unsigned int flags;

	spinlock_t requests_lock;	/* Lock accesses to tx.requests, tx.creq and rcv.creq when STREAM mode */

	void (*data_ready)(struct sock* sk, int len);
	void (*error_report)(struct sock* sk);
	void (*write_space)(struct sock* sk);	/* STREAM mode only */
	struct {
		struct work_struct tq;		/* STREAM/DGRAM: data/error ready */
		struct ncp_request_reply* creq;	/* STREAM/DGRAM: awaiting reply from this request */
		struct mutex creq_mutex;	/* DGRAM only: lock accesses to rcv.creq */

		unsigned int state;		/* STREAM only: receiver state */
		struct {
			__u32 magic __attribute__((packed));
			__u32 len __attribute__((packed));
			__u16 type __attribute__((packed));
			__u16 p1 __attribute__((packed));
			__u16 p2 __attribute__((packed));
			__u16 p3 __attribute__((packed));
			__u16 type2 __attribute__((packed));
		} buf;				/* STREAM only: temporary buffer */
		unsigned char* ptr;		/* STREAM only: pointer to data */
		size_t len;			/* STREAM only: length of data to receive */
	} rcv;
	struct {
		struct list_head requests;	/* STREAM only: queued requests */
		struct work_struct tq;		/* STREAM only: transmitter ready */
		struct ncp_request_reply* creq;	/* STREAM only: currently transmitted entry */
	} tx;
	struct timer_list timeout_tm;		/* DGRAM only: timeout timer */
	struct work_struct timeout_tq;		/* DGRAM only: associated queue, we run timers from process context */
	int timeout_last;			/* DGRAM only: current timeout length */
	int timeout_retries;			/* DGRAM only: retries left */
	struct {
		size_t len;
		__u8 data[128];
	} unexpected_packet;
};

extern void ncp_tcp_rcv_proc(struct work_struct *work);
extern void ncp_tcp_tx_proc(struct work_struct *work);
extern void ncpdgram_rcv_proc(struct work_struct *work);
extern void ncpdgram_timeout_proc(struct work_struct *work);
extern void ncpdgram_timeout_call(unsigned long server);
extern void ncp_tcp_data_ready(struct sock* sk, int len);
extern void ncp_tcp_write_space(struct sock* sk);
extern void ncp_tcp_error_report(struct sock* sk);

#define NCP_FLAG_UTF8	1

#define NCP_CLR_FLAG(server, flag)	((server)->flags &= ~(flag))
#define NCP_SET_FLAG(server, flag)	((server)->flags |= (flag))
#define NCP_IS_FLAG(server, flag)	((server)->flags & (flag))

static inline int ncp_conn_valid(struct ncp_server *server)
{
	return ((server->conn_status & 0x11) == 0);
}

static inline void ncp_invalidate_conn(struct ncp_server *server)
{
	server->conn_status |= 0x01;
}

#endif				/* __KERNEL__ */

#endif
 
