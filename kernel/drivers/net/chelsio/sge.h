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

/*****************************************************************************
 *                                                                           *
 * File: sge.h                                                               *
 * $Revision$                                                          *
 * $Date$                                              *
 * Description:                                                              *
 *  part of the Chelsio 10Gb Ethernet Driver.                                *
 *                                                                           *
 * This program is free software; you can redistribute it and/or modify      *
 * it under the terms of the GNU General Public License, version 2, as       *
 * published by the Free Software Foundation.                                *
 *                                                                           *
 * You should have received a copy of the GNU General Public License along   *
 * with this program; if not, write to the Free Software Foundation, Inc.,   *
 * 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.                 *
 *                                                                           *
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND WITHOUT ANY EXPRESS OR IMPLIED    *
 * WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED WARRANTIES OF      *
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.                     *
 *                                                                           *
 * http://www.chelsio.com                                                    *
 *                                                                           *
 * Copyright (c) 2003 - 2005 Chelsio Communications, Inc.                    *
 * All rights reserved.                                                      *
 *                                                                           *
 * Maintainers: maintainers@chelsio.com                                      *
 *                                                                           *
 * Authors: Dimitrios Michailidis   <dm@chelsio.com>                         *
 *          Tina Yang               <tainay@chelsio.com>                     *
 *          Felix Marti             <felix@chelsio.com>                      *
 *          Scott Bardone           <sbardone@chelsio.com>                   *
 *          Kurt Ottaway            <kottaway@chelsio.com>                   *
 *          Frank DiMambro          <frank@chelsio.com>                      *
 *                                                                           *
 * History:                                                                  *
 *                                                                           *
 ****************************************************************************/

#ifndef _CXGB_SGE_H_
#define _CXGB_SGE_H_

#include <linux/types.h>
#include <linux/interrupt.h>
#include <asm/byteorder.h>

struct sge_intr_counts {
	unsigned int rx_drops;        /* # of packets dropped due to no mem */
	unsigned int pure_rsps;        /* # of non-payload responses */
	unsigned int unhandled_irqs;   /* # of unhandled interrupts */
	unsigned int respQ_empty;      /* # times respQ empty */
	unsigned int respQ_overflow;   /* # respQ overflow (fatal) */
	unsigned int freelistQ_empty;  /* # times freelist empty */
	unsigned int pkt_too_big;      /* packet too large (fatal) */
	unsigned int pkt_mismatch;
	unsigned int cmdQ_full[3];     /* not HW IRQ, host cmdQ[] full */
	unsigned int cmdQ_restarted[3];/* # of times cmdQ X was restarted */
};

struct sge_port_stats {
	u64 rx_cso_good;     /* # of successful RX csum offloads */
	u64 tx_cso;          /* # of TX checksum offloads */
	u64 tx_tso;          /* # of TSO requests */
	u64 vlan_xtract;     /* # of VLAN tag extractions */
	u64 vlan_insert;     /* # of VLAN tag insertions */
	u64 tx_need_hdrroom; /* # of TX skbs in need of more header room */
};

struct sk_buff;
struct net_device;
struct adapter;
struct sge_params;
struct sge;

struct sge *t1_sge_create(struct adapter *, struct sge_params *);
int t1_sge_configure(struct sge *, struct sge_params *);
int t1_sge_set_coalesce_params(struct sge *, struct sge_params *);
void t1_sge_destroy(struct sge *);
irqreturn_t t1_interrupt(int irq, void *cookie);
int t1_poll(struct napi_struct *, int);

int t1_start_xmit(struct sk_buff *skb, struct net_device *dev);
void t1_set_vlan_accel(struct adapter *adapter, int on_off);
void t1_sge_start(struct sge *);
void t1_sge_stop(struct sge *);
int t1_sge_intr_error_handler(struct sge *);
void t1_sge_intr_enable(struct sge *);
void t1_sge_intr_disable(struct sge *);
void t1_sge_intr_clear(struct sge *);
const struct sge_intr_counts *t1_sge_get_intr_counts(const struct sge *sge);
void t1_sge_get_port_stats(const struct sge *sge, int port, struct sge_port_stats *);
unsigned int t1_sched_update_parms(struct sge *, unsigned int, unsigned int,
			   unsigned int);

#endif /* _CXGB_SGE_H_ */
