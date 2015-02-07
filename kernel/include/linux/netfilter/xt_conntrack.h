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

/* Header file for kernel module to match connection tracking information.
 * GPL (C) 2001  Marc Boucher (marc@mbsi.ca).
 */

#ifndef _XT_CONNTRACK_H
#define _XT_CONNTRACK_H

#include <linux/types.h>
#include <linux/netfilter/nf_conntrack_tuple_common.h>

#define XT_CONNTRACK_STATE_BIT(ctinfo) (1 << ((ctinfo)%IP_CT_IS_REPLY+1))
#define XT_CONNTRACK_STATE_INVALID (1 << 0)

#define XT_CONNTRACK_STATE_SNAT (1 << (IP_CT_NUMBER + 1))
#define XT_CONNTRACK_STATE_DNAT (1 << (IP_CT_NUMBER + 2))
#define XT_CONNTRACK_STATE_UNTRACKED (1 << (IP_CT_NUMBER + 3))

/* flags, invflags: */
enum {
	XT_CONNTRACK_STATE        = 1 << 0,
	XT_CONNTRACK_PROTO        = 1 << 1,
	XT_CONNTRACK_ORIGSRC      = 1 << 2,
	XT_CONNTRACK_ORIGDST      = 1 << 3,
	XT_CONNTRACK_REPLSRC      = 1 << 4,
	XT_CONNTRACK_REPLDST      = 1 << 5,
	XT_CONNTRACK_STATUS       = 1 << 6,
	XT_CONNTRACK_EXPIRES      = 1 << 7,
	XT_CONNTRACK_ORIGSRC_PORT = 1 << 8,
	XT_CONNTRACK_ORIGDST_PORT = 1 << 9,
	XT_CONNTRACK_REPLSRC_PORT = 1 << 10,
	XT_CONNTRACK_REPLDST_PORT = 1 << 11,
	XT_CONNTRACK_DIRECTION    = 1 << 12,
};

/* This is exposed to userspace, so remains frozen in time. */
struct ip_conntrack_old_tuple
{
	struct {
		__be32 ip;
		union {
			__u16 all;
		} u;
	} src;

	struct {
		__be32 ip;
		union {
			__u16 all;
		} u;

		/* The protocol. */
		__u16 protonum;
	} dst;
};

struct xt_conntrack_info
{
	unsigned int statemask, statusmask;

	struct ip_conntrack_old_tuple tuple[IP_CT_DIR_MAX];
	struct in_addr sipmsk[IP_CT_DIR_MAX], dipmsk[IP_CT_DIR_MAX];

	unsigned long expires_min, expires_max;

	/* Flags word */
	u_int8_t flags;
	/* Inverse flags */
	u_int8_t invflags;
};

struct xt_conntrack_mtinfo1 {
	union nf_inet_addr origsrc_addr, origsrc_mask;
	union nf_inet_addr origdst_addr, origdst_mask;
	union nf_inet_addr replsrc_addr, replsrc_mask;
	union nf_inet_addr repldst_addr, repldst_mask;
	u_int32_t expires_min, expires_max;
	u_int16_t l4proto;
	__be16 origsrc_port, origdst_port;
	__be16 replsrc_port, repldst_port;
	u_int16_t match_flags, invert_flags;
	u_int8_t state_mask, status_mask;
};

#endif /*_XT_CONNTRACK_H*/
