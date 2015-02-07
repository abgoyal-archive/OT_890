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
 * INET		An implementation of the TCP/IP protocol suite for the LINUX
 *		operating system.  INET is implemented using the  BSD Socket
 *		interface as the means of communication with the user level.
 *
 *		PF_INET6 protocol dispatch tables.
 *
 * Authors:	Pedro Roque	<roque@di.fc.ul.pt>
 *
 *		This program is free software; you can redistribute it and/or
 *		modify it under the terms of the GNU General Public License
 *		as published by the Free Software Foundation; either version
 *		2 of the License, or (at your option) any later version.
 */

/*
 *      Changes:
 *
 *      Vince Laviano (vince@cs.stanford.edu)       16 May 2001
 *      - Removed unused variable 'inet6_protocol_base'
 *      - Modified inet6_del_protocol() to correctly maintain copy bit.
 */

#include <linux/errno.h>
#include <linux/types.h>
#include <linux/socket.h>
#include <linux/sockios.h>
#include <linux/net.h>
#include <linux/in6.h>
#include <linux/netdevice.h>
#include <linux/if_arp.h>

#include <net/sock.h>
#include <net/snmp.h>

#include <net/ipv6.h>
#include <net/protocol.h>

struct inet6_protocol *inet6_protos[MAX_INET_PROTOS];
static DEFINE_SPINLOCK(inet6_proto_lock);


int inet6_add_protocol(struct inet6_protocol *prot, unsigned char protocol)
{
	int ret, hash = protocol & (MAX_INET_PROTOS - 1);

	spin_lock_bh(&inet6_proto_lock);

	if (inet6_protos[hash]) {
		ret = -1;
	} else {
		inet6_protos[hash] = prot;
		ret = 0;
	}

	spin_unlock_bh(&inet6_proto_lock);

	return ret;
}

EXPORT_SYMBOL(inet6_add_protocol);

/*
 *	Remove a protocol from the hash tables.
 */

int inet6_del_protocol(struct inet6_protocol *prot, unsigned char protocol)
{
	int ret, hash = protocol & (MAX_INET_PROTOS - 1);

	spin_lock_bh(&inet6_proto_lock);

	if (inet6_protos[hash] != prot) {
		ret = -1;
	} else {
		inet6_protos[hash] = NULL;
		ret = 0;
	}

	spin_unlock_bh(&inet6_proto_lock);

	synchronize_net();

	return ret;
}

EXPORT_SYMBOL(inet6_del_protocol);
