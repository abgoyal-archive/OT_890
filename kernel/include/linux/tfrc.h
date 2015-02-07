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

#ifndef _LINUX_TFRC_H_
#define _LINUX_TFRC_H_
/*
 *  TFRC - Data Structures for the TCP-Friendly Rate Control congestion
 *         control mechanism as specified in RFC 3448.
 *
 *  Copyright (c) 2005 The University of Waikato, Hamilton, New Zealand.
 *  Copyright (c) 2005 Ian McDonald <iam4@cs.waikato.ac.nz>
 *  Copyright (c) 2005 Arnaldo Carvalho de Melo <acme@conectiva.com.br>
 *  Copyright (c) 2003 Nils-Erik Mattsson, Joacim Haggmark, Magnus Erixzon
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 */
#include <linux/types.h>

/** 	tfrc_rx_info    -    TFRC Receiver Data Structure
 *
 * 	@tfrcrx_x_recv:	receiver estimate of sending rate (3.2.2)
 * 	@tfrcrx_rtt:	round-trip-time (communicated by sender)
 * 	@tfrcrx_p:	current estimate of loss event rate (3.2.2)
 */
struct tfrc_rx_info {
  	__u32 tfrcrx_x_recv;
	__u32 tfrcrx_rtt;
  	__u32 tfrcrx_p;
};

/** 	tfrc_tx_info    -    TFRC Sender Data Structure
 *
 * 	@tfrctx_x:	computed transmit rate (4.3 (4))
 * 	@tfrctx_x_recv: receiver estimate of send rate (4.3)
 * 	@tfrctx_x_calc:	return value of throughput equation (3.1)
 * 	@tfrctx_rtt:	(moving average) estimate of RTT (4.3)
 * 	@tfrctx_p:	current loss event rate (5.4)
 * 	@tfrctx_rto:	estimate of RTO, equals 4*RTT (4.3)
 * 	@tfrctx_ipi:	inter-packet interval (4.6)
 *
 *  Note: X and X_recv are both maintained in units of 64 * bytes/second. This
 *        enables a finer resolution of sending rates and avoids problems with
 *        integer arithmetic; u32 is not sufficient as scaling consumes 6 bits.
 */
struct tfrc_tx_info {
	__u64 tfrctx_x;
	__u64 tfrctx_x_recv;
	__u32 tfrctx_x_calc;
	__u32 tfrctx_rtt;
	__u32 tfrctx_p;
	__u32 tfrctx_rto;
	__u32 tfrctx_ipi;
};

#endif /* _LINUX_TFRC_H_ */
