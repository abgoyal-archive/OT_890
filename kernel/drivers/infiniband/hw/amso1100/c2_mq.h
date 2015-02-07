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
 * Copyright (c) 2005 Ammasso, Inc. All rights reserved.
 * Copyright (c) 2005 Open Grid Computing, Inc. All rights reserved.
 *
 * This software is available to you under a choice of one of two
 * licenses.  You may choose to be licensed under the terms of the GNU
 * General Public License (GPL) Version 2, available from the file
 * COPYING in the main directory of this source tree, or the
 * OpenIB.org BSD license below:
 *
 *     Redistribution and use in source and binary forms, with or
 *     without modification, are permitted provided that the following
 *     conditions are met:
 *
 *      - Redistributions of source code must retain the above
 *        copyright notice, this list of conditions and the following
 *        disclaimer.
 *
 *      - Redistributions in binary form must reproduce the above
 *        copyright notice, this list of conditions and the following
 *        disclaimer in the documentation and/or other materials
 *        provided with the distribution.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef _C2_MQ_H_
#define _C2_MQ_H_
#include <linux/kernel.h>
#include <linux/dma-mapping.h>
#include "c2_wr.h"

enum c2_shared_regs {

	C2_SHARED_ARMED = 0x10,
	C2_SHARED_NOTIFY = 0x18,
	C2_SHARED_SHARED = 0x40,
};

struct c2_mq_shared {
	u16 unused1;
	u8 armed;
	u8 notification_type;
	u32 unused2;
	u16 shared;
	/* Pad to 64 bytes. */
	u8 pad[64 - sizeof(u16) - 2 * sizeof(u8) - sizeof(u32) - sizeof(u16)];
};

enum c2_mq_type {
	C2_MQ_HOST_TARGET = 1,
	C2_MQ_ADAPTER_TARGET = 2,
};

/*
 * c2_mq_t is for kernel-mode MQs like the VQs Cand the AEQ.
 * c2_user_mq_t (which is the same format) is for user-mode MQs...
 */
#define C2_MQ_MAGIC 0x4d512020	/* 'MQ  ' */
struct c2_mq {
	u32 magic;
	union {
		u8 *host;
		u8 __iomem *adapter;
	} msg_pool;
	dma_addr_t host_dma;
	DECLARE_PCI_UNMAP_ADDR(mapping);
	u16 hint_count;
	u16 priv;
	struct c2_mq_shared __iomem *peer;
	__be16 *shared;
	dma_addr_t shared_dma;
	u32 q_size;
	u32 msg_size;
	u32 index;
	enum c2_mq_type type;
};

static __inline__ int c2_mq_empty(struct c2_mq *q)
{
	return q->priv == be16_to_cpu(*q->shared);
}

static __inline__ int c2_mq_full(struct c2_mq *q)
{
	return q->priv == (be16_to_cpu(*q->shared) + q->q_size - 1) % q->q_size;
}

extern void c2_mq_lconsume(struct c2_mq *q, u32 wqe_count);
extern void *c2_mq_alloc(struct c2_mq *q);
extern void c2_mq_produce(struct c2_mq *q);
extern void *c2_mq_consume(struct c2_mq *q);
extern void c2_mq_free(struct c2_mq *q);
extern void c2_mq_req_init(struct c2_mq *q, u32 index, u32 q_size, u32 msg_size,
		       u8 __iomem *pool_start, u16 __iomem *peer, u32 type);
extern void c2_mq_rep_init(struct c2_mq *q, u32 index, u32 q_size, u32 msg_size,
			   u8 *pool_start, u16 __iomem *peer, u32 type);

#endif				/* _C2_MQ_H_ */
