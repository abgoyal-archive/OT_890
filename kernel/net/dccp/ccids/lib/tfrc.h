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

#ifndef _TFRC_H_
#define _TFRC_H_
/*
 *  net/dccp/ccids/lib/tfrc.h
 *
 *  Copyright (c) 2007   The University of Aberdeen, Scotland, UK
 *  Copyright (c) 2005-6 The University of Waikato, Hamilton, New Zealand.
 *  Copyright (c) 2005-6 Ian McDonald <ian.mcdonald@jandi.co.nz>
 *  Copyright (c) 2005   Arnaldo Carvalho de Melo <acme@conectiva.com.br>
 *  Copyright (c) 2003   Nils-Erik Mattsson, Joacim Haggmark, Magnus Erixzon
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 */
#include <linux/types.h>
#include <linux/math64.h>
#include "../../dccp.h"

/* internal includes that this library exports: */
#include "loss_interval.h"
#include "packet_history.h"

#ifdef CONFIG_IP_DCCP_TFRC_DEBUG
extern int tfrc_debug;
#define tfrc_pr_debug(format, a...)	DCCP_PR_DEBUG(tfrc_debug, format, ##a)
#else
#define tfrc_pr_debug(format, a...)
#endif

/* integer-arithmetic divisions of type (a * 1000000)/b */
static inline u64 scaled_div(u64 a, u64 b)
{
	BUG_ON(b==0);
	return div64_u64(a * 1000000, b);
}

static inline u32 scaled_div32(u64 a, u64 b)
{
	u64 result = scaled_div(a, b);

	if (result > UINT_MAX) {
		DCCP_CRIT("Overflow: %llu/%llu > UINT_MAX",
			  (unsigned long long)a, (unsigned long long)b);
		return UINT_MAX;
	}
	return result;
}

/**
 * tfrc_ewma  -  Exponentially weighted moving average
 * @weight: Weight to be used as damping factor, in units of 1/10
 */
static inline u32 tfrc_ewma(const u32 avg, const u32 newval, const u8 weight)
{
	return avg ? (weight * avg + (10 - weight) * newval) / 10 : newval;
}

extern u32  tfrc_calc_x(u16 s, u32 R, u32 p);
extern u32  tfrc_calc_x_reverse_lookup(u32 fvalue);

extern int  tfrc_tx_packet_history_init(void);
extern void tfrc_tx_packet_history_exit(void);
extern int  tfrc_rx_packet_history_init(void);
extern void tfrc_rx_packet_history_exit(void);

extern int  tfrc_li_init(void);
extern void tfrc_li_exit(void);

#ifdef CONFIG_IP_DCCP_TFRC_LIB
extern int  tfrc_lib_init(void);
extern void tfrc_lib_exit(void);
#else
#define tfrc_lib_init() (0)
#define tfrc_lib_exit()
#endif
#endif /* _TFRC_H_ */
