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

/* -*- mode: c; c-basic-offset: 8; -*-
 * vim: noexpandtab sw=8 ts=8 sts=0:
 *
 * tcp.h
 *
 * Function prototypes
 *
 * Copyright (C) 2004 Oracle.  All rights reserved.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public
 * License along with this program; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 021110-1307, USA.
 *
 */

#ifndef O2CLUSTER_TCP_H
#define O2CLUSTER_TCP_H

#include <linux/socket.h>
#ifdef __KERNEL__
#include <net/sock.h>
#include <linux/tcp.h>
#else
#include <sys/socket.h>
#endif
#include <linux/inet.h>
#include <linux/in.h>

struct o2net_msg
{
	__be16 magic;
	__be16 data_len;
	__be16 msg_type;
	__be16 pad1;
	__be32 sys_status;
	__be32 status;
	__be32 key;
	__be32 msg_num;
	__u8  buf[0];
};

typedef int (o2net_msg_handler_func)(struct o2net_msg *msg, u32 len, void *data,
				     void **ret_data);
typedef void (o2net_post_msg_handler_func)(int status, void *data,
					   void *ret_data);

#define O2NET_MAX_PAYLOAD_BYTES  (4096 - sizeof(struct o2net_msg))

/* same as hb delay, we're waiting for another node to recognize our hb */
#define O2NET_RECONNECT_DELAY_MS_DEFAULT	2000

#define O2NET_KEEPALIVE_DELAY_MS_DEFAULT	2000
#define O2NET_IDLE_TIMEOUT_MS_DEFAULT		30000


/* TODO: figure this out.... */
static inline int o2net_link_down(int err, struct socket *sock)
{
	if (sock) {
		if (sock->sk->sk_state != TCP_ESTABLISHED &&
	    	    sock->sk->sk_state != TCP_CLOSE_WAIT)
			return 1;
	}

	if (err >= 0)
		return 0;
	switch (err) {
		/* ????????????????????????? */
		case -ERESTARTSYS:
		case -EBADF:
		/* When the server has died, an ICMP port unreachable
		 * message prompts ECONNREFUSED. */
		case -ECONNREFUSED:
		case -ENOTCONN:
		case -ECONNRESET:
		case -EPIPE:
			return 1;
	}
	return 0;
}

enum {
	O2NET_DRIVER_UNINITED,
	O2NET_DRIVER_READY,
};

int o2net_send_message(u32 msg_type, u32 key, void *data, u32 len,
		       u8 target_node, int *status);
int o2net_send_message_vec(u32 msg_type, u32 key, struct kvec *vec,
			   size_t veclen, u8 target_node, int *status);

int o2net_register_handler(u32 msg_type, u32 key, u32 max_len,
			   o2net_msg_handler_func *func, void *data,
			   o2net_post_msg_handler_func *post_func,
			   struct list_head *unreg_list);
void o2net_unregister_handler_list(struct list_head *list);

struct o2nm_node;
int o2net_register_hb_callbacks(void);
void o2net_unregister_hb_callbacks(void);
int o2net_start_listening(struct o2nm_node *node);
void o2net_stop_listening(struct o2nm_node *node);
void o2net_disconnect_node(struct o2nm_node *node);
int o2net_num_connected_peers(void);

int o2net_init(void);
void o2net_exit(void);

struct o2net_send_tracking;
struct o2net_sock_container;

#ifdef CONFIG_DEBUG_FS
int o2net_debugfs_init(void);
void o2net_debugfs_exit(void);
void o2net_debug_add_nst(struct o2net_send_tracking *nst);
void o2net_debug_del_nst(struct o2net_send_tracking *nst);
void o2net_debug_add_sc(struct o2net_sock_container *sc);
void o2net_debug_del_sc(struct o2net_sock_container *sc);
#else
static inline int o2net_debugfs_init(void)
{
	return 0;
}
static inline void o2net_debugfs_exit(void)
{
}
static inline void o2net_debug_add_nst(struct o2net_send_tracking *nst)
{
}
static inline void o2net_debug_del_nst(struct o2net_send_tracking *nst)
{
}
static inline void o2net_debug_add_sc(struct o2net_sock_container *sc)
{
}
static inline void o2net_debug_del_sc(struct o2net_sock_container *sc)
{
}
#endif	/* CONFIG_DEBUG_FS */

#endif /* O2CLUSTER_TCP_H */
