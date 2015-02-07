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

#ifndef _XT_POLICY_H
#define _XT_POLICY_H

#define XT_POLICY_MAX_ELEM	4

enum xt_policy_flags
{
	XT_POLICY_MATCH_IN	= 0x1,
	XT_POLICY_MATCH_OUT	= 0x2,
	XT_POLICY_MATCH_NONE	= 0x4,
	XT_POLICY_MATCH_STRICT	= 0x8,
};

enum xt_policy_modes
{
	XT_POLICY_MODE_TRANSPORT,
	XT_POLICY_MODE_TUNNEL
};

struct xt_policy_spec
{
	u_int8_t	saddr:1,
			daddr:1,
			proto:1,
			mode:1,
			spi:1,
			reqid:1;
};

#ifndef __KERNEL__
union xt_policy_addr
{
	struct in_addr	a4;
	struct in6_addr	a6;
};
#endif

struct xt_policy_elem
{
	union {
#ifdef __KERNEL__
		struct {
			union nf_inet_addr saddr;
			union nf_inet_addr smask;
			union nf_inet_addr daddr;
			union nf_inet_addr dmask;
		};
#else
		struct {
			union xt_policy_addr saddr;
			union xt_policy_addr smask;
			union xt_policy_addr daddr;
			union xt_policy_addr dmask;
		};
#endif
	};
	__be32			spi;
	u_int32_t		reqid;
	u_int8_t		proto;
	u_int8_t		mode;

	struct xt_policy_spec	match;
	struct xt_policy_spec	invert;
};

struct xt_policy_info
{
	struct xt_policy_elem pol[XT_POLICY_MAX_ELEM];
	u_int16_t flags;
	u_int16_t len;
};

#endif /* _XT_POLICY_H */
