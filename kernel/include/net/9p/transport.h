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
 * include/net/9p/transport.h
 *
 * Transport Definition
 *
 *  Copyright (C) 2005 by Latchesar Ionkov <lucho@ionkov.net>
 *  Copyright (C) 2004-2008 by Eric Van Hensbergen <ericvh@gmail.com>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License version 2
 *  as published by the Free Software Foundation.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to:
 *  Free Software Foundation
 *  51 Franklin Street, Fifth Floor
 *  Boston, MA  02111-1301  USA
 *
 */

#ifndef NET_9P_TRANSPORT_H
#define NET_9P_TRANSPORT_H

/**
 * struct p9_trans_module - transport module interface
 * @list: used to maintain a list of currently available transports
 * @name: the human-readable name of the transport
 * @maxsize: transport provided maximum packet size
 * @def: set if this transport should be considered the default
 * @create: member function to create a new connection on this transport
 * @request: member function to issue a request to the transport
 * @cancel: member function to cancel a request (if it hasn't been sent)
 *
 * This is the basic API for a transport module which is registered by the
 * transport module with the 9P core network module and used by the client
 * to instantiate a new connection on a transport.
 *
 * BUGS: the transport module list isn't protected.
 */

struct p9_trans_module {
	struct list_head list;
	char *name;		/* name of transport */
	int maxsize;		/* max message size of transport */
	int def;		/* this transport should be default */
	struct module *owner;
	int (*create)(struct p9_client *, const char *, char *);
	void (*close) (struct p9_client *);
	int (*request) (struct p9_client *, struct p9_req_t *req);
	int (*cancel) (struct p9_client *, struct p9_req_t *req);
};

void v9fs_register_trans(struct p9_trans_module *m);
void v9fs_unregister_trans(struct p9_trans_module *m);
struct p9_trans_module *v9fs_get_trans_by_name(const substring_t *name);
struct p9_trans_module *v9fs_get_default_trans(void);
void v9fs_put_trans(struct p9_trans_module *m);
#endif /* NET_9P_TRANSPORT_H */
