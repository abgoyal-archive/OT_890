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
 * SELinux interface to the NetLabel subsystem
 *
 * Author : Paul Moore <paul.moore@hp.com>
 *
 */

/*
 * (c) Copyright Hewlett-Packard Development Company, L.P., 2006
 *
 * This program is free software;  you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY;  without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See
 * the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program;  if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 *
 */

#ifndef _SELINUX_NETLABEL_H_
#define _SELINUX_NETLABEL_H_

#include <linux/types.h>
#include <linux/fs.h>
#include <linux/net.h>
#include <linux/skbuff.h>
#include <net/sock.h>

#include "avc.h"
#include "objsec.h"

#ifdef CONFIG_NETLABEL
void selinux_netlbl_cache_invalidate(void);

void selinux_netlbl_err(struct sk_buff *skb, int error, int gateway);

void selinux_netlbl_sk_security_free(struct sk_security_struct *ssec);
void selinux_netlbl_sk_security_reset(struct sk_security_struct *ssec,
				      int family);

int selinux_netlbl_skbuff_getsid(struct sk_buff *skb,
				 u16 family,
				 u32 *type,
				 u32 *sid);
int selinux_netlbl_skbuff_setsid(struct sk_buff *skb,
				 u16 family,
				 u32 sid);

void selinux_netlbl_inet_conn_established(struct sock *sk, u16 family);
int selinux_netlbl_socket_post_create(struct socket *sock);
int selinux_netlbl_inode_permission(struct inode *inode, int mask);
int selinux_netlbl_sock_rcv_skb(struct sk_security_struct *sksec,
				struct sk_buff *skb,
				u16 family,
				struct avc_audit_data *ad);
int selinux_netlbl_socket_setsockopt(struct socket *sock,
				     int level,
				     int optname);
int selinux_netlbl_socket_connect(struct sock *sk, struct sockaddr *addr);

#else
static inline void selinux_netlbl_cache_invalidate(void)
{
	return;
}

static inline void selinux_netlbl_err(struct sk_buff *skb,
				      int error,
				      int gateway)
{
	return;
}

static inline void selinux_netlbl_sk_security_free(
					       struct sk_security_struct *ssec)
{
	return;
}

static inline void selinux_netlbl_sk_security_reset(
					       struct sk_security_struct *ssec,
					       int family)
{
	return;
}

static inline int selinux_netlbl_skbuff_getsid(struct sk_buff *skb,
					       u16 family,
					       u32 *type,
					       u32 *sid)
{
	*type = NETLBL_NLTYPE_NONE;
	*sid = SECSID_NULL;
	return 0;
}
static inline int selinux_netlbl_skbuff_setsid(struct sk_buff *skb,
					       u16 family,
					       u32 sid)
{
	return 0;
}

static inline int selinux_netlbl_conn_setsid(struct sock *sk,
					     struct sockaddr *addr)
{
	return 0;
}

static inline void selinux_netlbl_inet_conn_established(struct sock *sk,
							u16 family)
{
	return;
}
static inline int selinux_netlbl_socket_post_create(struct socket *sock)
{
	return 0;
}
static inline int selinux_netlbl_inode_permission(struct inode *inode,
						  int mask)
{
	return 0;
}
static inline int selinux_netlbl_sock_rcv_skb(struct sk_security_struct *sksec,
					      struct sk_buff *skb,
					      u16 family,
					      struct avc_audit_data *ad)
{
	return 0;
}
static inline int selinux_netlbl_socket_setsockopt(struct socket *sock,
						   int level,
						   int optname)
{
	return 0;
}
static inline int selinux_netlbl_socket_connect(struct sock *sk,
						struct sockaddr *addr)
{
	return 0;
}
#endif /* CONFIG_NETLABEL */

#endif
