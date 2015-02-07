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
 *
 */

#ifndef C2_PROVIDER_H
#define C2_PROVIDER_H
#include <linux/inetdevice.h>

#include <rdma/ib_verbs.h>
#include <rdma/ib_pack.h>

#include "c2_mq.h"
#include <rdma/iw_cm.h>

#define C2_MPT_FLAG_ATOMIC        (1 << 14)
#define C2_MPT_FLAG_REMOTE_WRITE  (1 << 13)
#define C2_MPT_FLAG_REMOTE_READ   (1 << 12)
#define C2_MPT_FLAG_LOCAL_WRITE   (1 << 11)
#define C2_MPT_FLAG_LOCAL_READ    (1 << 10)

struct c2_buf_list {
	void *buf;
	 DECLARE_PCI_UNMAP_ADDR(mapping)
};


/* The user context keeps track of objects allocated for a
 * particular user-mode client. */
struct c2_ucontext {
	struct ib_ucontext ibucontext;
};

struct c2_mtt;

/* All objects associated with a PD are kept in the
 * associated user context if present.
 */
struct c2_pd {
	struct ib_pd ibpd;
	u32 pd_id;
};

struct c2_mr {
	struct ib_mr ibmr;
	struct c2_pd *pd;
	struct ib_umem *umem;
};

struct c2_av;

enum c2_ah_type {
	C2_AH_ON_HCA,
	C2_AH_PCI_POOL,
	C2_AH_KMALLOC
};

struct c2_ah {
	struct ib_ah ibah;
};

struct c2_cq {
	struct ib_cq ibcq;
	spinlock_t lock;
	atomic_t refcount;
	int cqn;
	int is_kernel;
	wait_queue_head_t wait;

	u32 adapter_handle;
	struct c2_mq mq;
};

struct c2_wq {
	spinlock_t lock;
};
struct iw_cm_id;
struct c2_qp {
	struct ib_qp ibqp;
	struct iw_cm_id *cm_id;
	spinlock_t lock;
	atomic_t refcount;
	wait_queue_head_t wait;
	int qpn;

	u32 adapter_handle;
	u32 send_sgl_depth;
	u32 recv_sgl_depth;
	u32 rdma_write_sgl_depth;
	u8 state;

	struct c2_mq sq_mq;
	struct c2_mq rq_mq;
};

struct c2_cr_query_attrs {
	u32 local_addr;
	u32 remote_addr;
	u16 local_port;
	u16 remote_port;
};

static inline struct c2_pd *to_c2pd(struct ib_pd *ibpd)
{
	return container_of(ibpd, struct c2_pd, ibpd);
}

static inline struct c2_ucontext *to_c2ucontext(struct ib_ucontext *ibucontext)
{
	return container_of(ibucontext, struct c2_ucontext, ibucontext);
}

static inline struct c2_mr *to_c2mr(struct ib_mr *ibmr)
{
	return container_of(ibmr, struct c2_mr, ibmr);
}


static inline struct c2_ah *to_c2ah(struct ib_ah *ibah)
{
	return container_of(ibah, struct c2_ah, ibah);
}

static inline struct c2_cq *to_c2cq(struct ib_cq *ibcq)
{
	return container_of(ibcq, struct c2_cq, ibcq);
}

static inline struct c2_qp *to_c2qp(struct ib_qp *ibqp)
{
	return container_of(ibqp, struct c2_qp, ibqp);
}

static inline int is_rnic_addr(struct net_device *netdev, u32 addr)
{
	struct in_device *ind;
	int ret = 0;

	ind = in_dev_get(netdev);
	if (!ind)
		return 0;

	for_ifa(ind) {
		if (ifa->ifa_address == addr) {
			ret = 1;
			break;
		}
	}
	endfor_ifa(ind);
	in_dev_put(ind);
	return ret;
}
#endif				/* C2_PROVIDER_H */
