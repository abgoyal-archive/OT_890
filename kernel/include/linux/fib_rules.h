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

#ifndef __LINUX_FIB_RULES_H
#define __LINUX_FIB_RULES_H

#include <linux/types.h>
#include <linux/rtnetlink.h>

/* rule is permanent, and cannot be deleted */
#define FIB_RULE_PERMANENT	0x00000001
#define FIB_RULE_INVERT		0x00000002
#define FIB_RULE_UNRESOLVED	0x00000004
#define FIB_RULE_DEV_DETACHED	0x00000008

/* try to find source address in routing lookups */
#define FIB_RULE_FIND_SADDR	0x00010000

struct fib_rule_hdr
{
	__u8		family;
	__u8		dst_len;
	__u8		src_len;
	__u8		tos;

	__u8		table;
	__u8		res1;	/* reserved */
	__u8		res2;	/* reserved */
	__u8		action;

	__u32		flags;
};

enum
{
	FRA_UNSPEC,
	FRA_DST,	/* destination address */
	FRA_SRC,	/* source address */
	FRA_IFNAME,	/* interface name */
	FRA_GOTO,	/* target to jump to (FR_ACT_GOTO) */
	FRA_UNUSED2,
	FRA_PRIORITY,	/* priority/preference */
	FRA_UNUSED3,
	FRA_UNUSED4,
	FRA_UNUSED5,
	FRA_FWMARK,	/* mark */
	FRA_FLOW,	/* flow/class id */
	FRA_UNUSED6,
	FRA_UNUSED7,
	FRA_UNUSED8,
	FRA_TABLE,	/* Extended table id */
	FRA_FWMASK,	/* mask for netfilter mark */
	__FRA_MAX
};

#define FRA_MAX (__FRA_MAX - 1)

enum
{
	FR_ACT_UNSPEC,
	FR_ACT_TO_TBL,		/* Pass to fixed table */
	FR_ACT_GOTO,		/* Jump to another rule */
	FR_ACT_NOP,		/* No operation */
	FR_ACT_RES3,
	FR_ACT_RES4,
	FR_ACT_BLACKHOLE,	/* Drop without notification */
	FR_ACT_UNREACHABLE,	/* Drop with ENETUNREACH */
	FR_ACT_PROHIBIT,	/* Drop with EACCES */
	__FR_ACT_MAX,
};

#define FR_ACT_MAX (__FR_ACT_MAX - 1)

#endif
