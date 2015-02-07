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
   CMTP implementation for Linux Bluetooth stack (BlueZ).
   Copyright (C) 2002-2003 Marcel Holtmann <marcel@holtmann.org>

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License version 2 as
   published by the Free Software Foundation;

   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
   OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT OF THIRD PARTY RIGHTS.
   IN NO EVENT SHALL THE COPYRIGHT HOLDER(S) AND AUTHOR(S) BE LIABLE FOR ANY
   CLAIM, OR ANY SPECIAL INDIRECT OR CONSEQUENTIAL DAMAGES, OR ANY DAMAGES
   WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
   ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
   OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.

   ALL LIABILITY, INCLUDING LIABILITY FOR INFRINGEMENT OF ANY PATENTS,
   COPYRIGHTS, TRADEMARKS OR OTHER RIGHTS, RELATING TO USE OF THIS
   SOFTWARE IS DISCLAIMED.
*/

#ifndef __CMTP_H
#define __CMTP_H

#include <linux/types.h>
#include <net/bluetooth/bluetooth.h>

#define BTNAMSIZ 18

/* CMTP ioctl defines */
#define CMTPCONNADD	_IOW('C', 200, int)
#define CMTPCONNDEL	_IOW('C', 201, int)
#define CMTPGETCONNLIST	_IOR('C', 210, int)
#define CMTPGETCONNINFO	_IOR('C', 211, int)

#define CMTP_LOOPBACK	0

struct cmtp_connadd_req {
	int   sock;	// Connected socket
	__u32 flags;
};

struct cmtp_conndel_req {
	bdaddr_t bdaddr;
	__u32    flags;
};

struct cmtp_conninfo {
	bdaddr_t bdaddr;
	__u32    flags;
	__u16    state;
	int      num;
};

struct cmtp_connlist_req {
	__u32  cnum;
	struct cmtp_conninfo __user *ci;
};

int cmtp_add_connection(struct cmtp_connadd_req *req, struct socket *sock);
int cmtp_del_connection(struct cmtp_conndel_req *req);
int cmtp_get_connlist(struct cmtp_connlist_req *req);
int cmtp_get_conninfo(struct cmtp_conninfo *ci);

/* CMTP session defines */
#define CMTP_INTEROP_TIMEOUT	(HZ * 5)
#define CMTP_INITIAL_MSGNUM	0xff00

struct cmtp_session {
	struct list_head list;

	struct socket *sock;

	bdaddr_t bdaddr;

	unsigned long state;
	unsigned long flags;

	uint mtu;

	char name[BTNAMSIZ];

	atomic_t terminate;

	wait_queue_head_t wait;

	int ncontroller;
	int num;
	struct capi_ctr ctrl;

	struct list_head applications;

	unsigned long blockids;
	int msgnum;

	struct sk_buff_head transmit;

	struct sk_buff *reassembly[16];
};

struct cmtp_application {
	struct list_head list;

	unsigned long state;
	int err;

	__u16 appl;
	__u16 mapping;

	__u16 msgnum;
};

struct cmtp_scb {
	int id;
	int data;
};

int  cmtp_attach_device(struct cmtp_session *session);
void cmtp_detach_device(struct cmtp_session *session);

void cmtp_recv_capimsg(struct cmtp_session *session, struct sk_buff *skb);

static inline void cmtp_schedule(struct cmtp_session *session)
{
	struct sock *sk = session->sock->sk;

	wake_up_interruptible(sk->sk_sleep);
}

/* CMTP init defines */
int cmtp_init_sockets(void);
void cmtp_cleanup_sockets(void);

#endif /* __CMTP_H */
