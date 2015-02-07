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

#ifndef __LINUX_DECNET_NETFILTER_H
#define __LINUX_DECNET_NETFILTER_H

/* DECnet-specific defines for netfilter. 
 * This file (C) Steve Whitehouse 1999 derived from the
 * ipv4 netfilter header file which is
 * (C)1998 Rusty Russell -- This code is GPL.
 */

#include <linux/netfilter.h>

/* only for userspace compatibility */
#ifndef __KERNEL__
/* IP Cache bits. */
/* Src IP address. */
#define NFC_DN_SRC		0x0001
/* Dest IP address. */
#define NFC_DN_DST		0x0002
/* Input device. */
#define NFC_DN_IF_IN		0x0004
/* Output device. */
#define NFC_DN_IF_OUT		0x0008
#endif /* ! __KERNEL__ */

/* DECnet Hooks */
/* After promisc drops, checksum checks. */
#define NF_DN_PRE_ROUTING	0
/* If the packet is destined for this box. */
#define NF_DN_LOCAL_IN		1
/* If the packet is destined for another interface. */
#define NF_DN_FORWARD		2
/* Packets coming from a local process. */
#define NF_DN_LOCAL_OUT		3
/* Packets about to hit the wire. */
#define NF_DN_POST_ROUTING	4
/* Input Hello Packets */
#define NF_DN_HELLO		5
/* Input Routing Packets */
#define NF_DN_ROUTE		6
#define NF_DN_NUMHOOKS		7

enum nf_dn_hook_priorities {
	NF_DN_PRI_FIRST = INT_MIN,
	NF_DN_PRI_CONNTRACK = -200,
	NF_DN_PRI_MANGLE = -150,
	NF_DN_PRI_NAT_DST = -100,
	NF_DN_PRI_FILTER = 0,
	NF_DN_PRI_NAT_SRC = 100,
	NF_DN_PRI_DNRTMSG = 200,
	NF_DN_PRI_LAST = INT_MAX,
};

struct nf_dn_rtmsg {
	int nfdn_ifindex;
};

#define NFDN_RTMSG(r) ((unsigned char *)(r) + NLMSG_ALIGN(sizeof(struct nf_dn_rtmsg)))

#ifndef __KERNEL__
/* backwards compatibility for userspace */
#define DNRMG_L1_GROUP 0x01
#define DNRMG_L2_GROUP 0x02
#endif

enum {
	DNRNG_NLGRP_NONE,
#define DNRNG_NLGRP_NONE	DNRNG_NLGRP_NONE
	DNRNG_NLGRP_L1,
#define DNRNG_NLGRP_L1		DNRNG_NLGRP_L1
	DNRNG_NLGRP_L2,
#define DNRNG_NLGRP_L2		DNRNG_NLGRP_L2
	__DNRNG_NLGRP_MAX
};
#define DNRNG_NLGRP_MAX	(__DNRNG_NLGRP_MAX - 1)

#endif /*__LINUX_DECNET_NETFILTER_H*/
