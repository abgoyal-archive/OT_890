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

#ifndef _IEEE1394_TRANSACTIONS_H
#define _IEEE1394_TRANSACTIONS_H

#include <linux/types.h>

#include "ieee1394_types.h"

struct hpsb_packet;
struct hpsb_host;

int hpsb_get_tlabel(struct hpsb_packet *packet);
void hpsb_free_tlabel(struct hpsb_packet *packet);
struct hpsb_packet *hpsb_make_readpacket(struct hpsb_host *host, nodeid_t node,
					 u64 addr, size_t length);
struct hpsb_packet *hpsb_make_lockpacket(struct hpsb_host *host, nodeid_t node,
					 u64 addr, int extcode, quadlet_t *data,
					 quadlet_t arg);
struct hpsb_packet *hpsb_make_lock64packet(struct hpsb_host *host,
					   nodeid_t node, u64 addr, int extcode,
					   octlet_t *data, octlet_t arg);
struct hpsb_packet *hpsb_make_phypacket(struct hpsb_host *host, quadlet_t data);
struct hpsb_packet *hpsb_make_writepacket(struct hpsb_host *host,
					  nodeid_t node, u64 addr,
					  quadlet_t *buffer, size_t length);
struct hpsb_packet *hpsb_make_streampacket(struct hpsb_host *host, u8 *buffer,
                                           int length, int channel, int tag,
					   int sync);
int hpsb_packet_success(struct hpsb_packet *packet);
int hpsb_read(struct hpsb_host *host, nodeid_t node, unsigned int generation,
	      u64 addr, quadlet_t *buffer, size_t length);
int hpsb_write(struct hpsb_host *host, nodeid_t node, unsigned int generation,
	       u64 addr, quadlet_t *buffer, size_t length);
int hpsb_lock(struct hpsb_host *host, nodeid_t node, unsigned int generation,
	      u64 addr, int extcode, quadlet_t *data, quadlet_t arg);

#ifdef HPSB_DEBUG_TLABELS
extern spinlock_t hpsb_tlabel_lock;
#endif

#endif /* _IEEE1394_TRANSACTIONS_H */
