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

/* $Date$ $RCSfile: tp.h,v $ $Revision$ */
#ifndef CHELSIO_TP_H
#define CHELSIO_TP_H

#include "common.h"

#define TP_MAX_RX_COALESCING_SIZE 16224U

struct tp_mib_statistics {

	/* IP */
	u32 ipInReceive_hi;
	u32 ipInReceive_lo;
	u32 ipInHdrErrors_hi;
	u32 ipInHdrErrors_lo;
	u32 ipInAddrErrors_hi;
	u32 ipInAddrErrors_lo;
	u32 ipInUnknownProtos_hi;
	u32 ipInUnknownProtos_lo;
	u32 ipInDiscards_hi;
	u32 ipInDiscards_lo;
	u32 ipInDelivers_hi;
	u32 ipInDelivers_lo;
	u32 ipOutRequests_hi;
	u32 ipOutRequests_lo;
	u32 ipOutDiscards_hi;
	u32 ipOutDiscards_lo;
	u32 ipOutNoRoutes_hi;
	u32 ipOutNoRoutes_lo;
	u32 ipReasmTimeout;
	u32 ipReasmReqds;
	u32 ipReasmOKs;
	u32 ipReasmFails;

	u32 reserved[8];

	/* TCP */
	u32 tcpActiveOpens;
	u32 tcpPassiveOpens;
	u32 tcpAttemptFails;
	u32 tcpEstabResets;
	u32 tcpOutRsts;
	u32 tcpCurrEstab;
	u32 tcpInSegs_hi;
	u32 tcpInSegs_lo;
	u32 tcpOutSegs_hi;
	u32 tcpOutSegs_lo;
	u32 tcpRetransSeg_hi;
	u32 tcpRetransSeg_lo;
	u32 tcpInErrs_hi;
	u32 tcpInErrs_lo;
	u32 tcpRtoMin;
	u32 tcpRtoMax;
};

struct petp;
struct tp_params;

struct petp *t1_tp_create(adapter_t *adapter, struct tp_params *p);
void t1_tp_destroy(struct petp *tp);

void t1_tp_intr_disable(struct petp *tp);
void t1_tp_intr_enable(struct petp *tp);
void t1_tp_intr_clear(struct petp *tp);
int t1_tp_intr_handler(struct petp *tp);

void t1_tp_get_mib_statistics(adapter_t *adap, struct tp_mib_statistics *tps);
void t1_tp_set_udp_checksum_offload(struct petp *tp, int enable);
void t1_tp_set_tcp_checksum_offload(struct petp *tp, int enable);
void t1_tp_set_ip_checksum_offload(struct petp *tp, int enable);
int t1_tp_set_coalescing_size(struct petp *tp, unsigned int size);
int t1_tp_reset(struct petp *tp, struct tp_params *p, unsigned int tp_clk);
#endif
