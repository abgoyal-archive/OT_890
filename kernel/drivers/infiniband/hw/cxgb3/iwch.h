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
 * Copyright (c) 2006 Chelsio, Inc. All rights reserved.
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
#ifndef __IWCH_H__
#define __IWCH_H__

#include <linux/mutex.h>
#include <linux/list.h>
#include <linux/spinlock.h>
#include <linux/idr.h>

#include <rdma/ib_verbs.h>

#include "cxio_hal.h"
#include "cxgb3_offload.h"

struct iwch_pd;
struct iwch_cq;
struct iwch_qp;
struct iwch_mr;

struct iwch_rnic_attributes {
	u32 max_qps;
	u32 max_wrs;				/* Max for any SQ/RQ */
	u32 max_sge_per_wr;
	u32 max_sge_per_rdma_write_wr;	/* for RDMA Write WR */
	u32 max_cqs;
	u32 max_cqes_per_cq;
	u32 max_mem_regs;
	u32 max_phys_buf_entries;		/* for phys buf list */
	u32 max_pds;

	/*
	 * The memory page sizes supported by this RNIC.
	 * Bit position i in bitmap indicates page of
	 * size (4k)^i.  Phys block list mode unsupported.
	 */
	u32 mem_pgsizes_bitmask;
	u64 max_mr_size;
	u8 can_resize_wq;

	/*
	 * The maximum number of RDMA Reads that can be outstanding
	 * per QP with this RNIC as the target.
	 */
	u32 max_rdma_reads_per_qp;

	/*
	 * The maximum number of resources used for RDMA Reads
	 * by this RNIC with this RNIC as the target.
	 */
	u32 max_rdma_read_resources;

	/*
	 * The max depth per QP for initiation of RDMA Read
	 * by this RNIC.
	 */
	u32 max_rdma_read_qp_depth;

	/*
	 * The maximum depth for initiation of RDMA Read
	 * operations by this RNIC on all QPs
	 */
	u32 max_rdma_read_depth;
	u8 rq_overflow_handled;
	u32 can_modify_ird;
	u32 can_modify_ord;
	u32 max_mem_windows;
	u32 stag0_value;
	u8 zbva_support;
	u8 local_invalidate_fence;
	u32 cq_overflow_detection;
};

struct iwch_dev {
	struct ib_device ibdev;
	struct cxio_rdev rdev;
	u32 device_cap_flags;
	struct iwch_rnic_attributes attr;
	struct idr cqidr;
	struct idr qpidr;
	struct idr mmidr;
	spinlock_t lock;
	struct list_head entry;
};

static inline struct iwch_dev *to_iwch_dev(struct ib_device *ibdev)
{
	return container_of(ibdev, struct iwch_dev, ibdev);
}

static inline int t3b_device(const struct iwch_dev *rhp)
{
	return rhp->rdev.t3cdev_p->type == T3B;
}

static inline int t3a_device(const struct iwch_dev *rhp)
{
	return rhp->rdev.t3cdev_p->type == T3A;
}

static inline struct iwch_cq *get_chp(struct iwch_dev *rhp, u32 cqid)
{
	return idr_find(&rhp->cqidr, cqid);
}

static inline struct iwch_qp *get_qhp(struct iwch_dev *rhp, u32 qpid)
{
	return idr_find(&rhp->qpidr, qpid);
}

static inline struct iwch_mr *get_mhp(struct iwch_dev *rhp, u32 mmid)
{
	return idr_find(&rhp->mmidr, mmid);
}

static inline int insert_handle(struct iwch_dev *rhp, struct idr *idr,
				void *handle, u32 id)
{
	int ret;
	int newid;

	do {
		if (!idr_pre_get(idr, GFP_KERNEL)) {
			return -ENOMEM;
		}
		spin_lock_irq(&rhp->lock);
		ret = idr_get_new_above(idr, handle, id, &newid);
		BUG_ON(newid != id);
		spin_unlock_irq(&rhp->lock);
	} while (ret == -EAGAIN);

	return ret;
}

static inline void remove_handle(struct iwch_dev *rhp, struct idr *idr, u32 id)
{
	spin_lock_irq(&rhp->lock);
	idr_remove(idr, id);
	spin_unlock_irq(&rhp->lock);
}

extern struct cxgb3_client t3c_client;
extern cxgb3_cpl_handler_func t3c_handlers[NUM_CPL_CMDS];
extern void iwch_ev_dispatch(struct cxio_rdev *rdev_p, struct sk_buff *skb);

#endif
