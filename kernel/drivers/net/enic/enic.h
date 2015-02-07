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
 * Copyright 2008 Cisco Systems, Inc.  All rights reserved.
 * Copyright 2007 Nuova Systems, Inc.  All rights reserved.
 *
 * This program is free software; you may redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; version 2 of the License.
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

#ifndef _ENIC_H_
#define _ENIC_H_

#include <linux/inet_lro.h>

#include "vnic_enet.h"
#include "vnic_dev.h"
#include "vnic_wq.h"
#include "vnic_rq.h"
#include "vnic_cq.h"
#include "vnic_intr.h"
#include "vnic_stats.h"
#include "vnic_rss.h"

#define DRV_NAME		"enic"
#define DRV_DESCRIPTION		"Cisco 10G Ethernet Driver"
#define DRV_VERSION		"1.0.0.648"
#define DRV_COPYRIGHT		"Copyright 2008 Cisco Systems, Inc"
#define PFX			DRV_NAME ": "

#define ENIC_LRO_MAX_DESC	8
#define ENIC_LRO_MAX_AGGR	64

enum enic_cq_index {
	ENIC_CQ_RQ,
	ENIC_CQ_WQ,
	ENIC_CQ_MAX,
};

enum enic_intx_intr_index {
	ENIC_INTX_WQ_RQ,
	ENIC_INTX_ERR,
	ENIC_INTX_NOTIFY,
	ENIC_INTX_MAX,
};

enum enic_msix_intr_index {
	ENIC_MSIX_RQ,
	ENIC_MSIX_WQ,
	ENIC_MSIX_ERR,
	ENIC_MSIX_NOTIFY,
	ENIC_MSIX_MAX,
};

struct enic_msix_entry {
	int requested;
	char devname[IFNAMSIZ];
	irqreturn_t (*isr)(int, void *);
	void *devid;
};

/* Per-instance private data structure */
struct enic {
	struct net_device *netdev;
	struct pci_dev *pdev;
	struct vnic_enet_config config;
	struct vnic_dev_bar bar0;
	struct vnic_dev *vdev;
	struct timer_list notify_timer;
	struct work_struct reset;
	struct msix_entry msix_entry[ENIC_MSIX_MAX];
	struct enic_msix_entry msix[ENIC_MSIX_MAX];
	u32 msg_enable;
	spinlock_t devcmd_lock;
	u8 mac_addr[ETH_ALEN];
	u8 mc_addr[ENIC_MULTICAST_PERFECT_FILTERS][ETH_ALEN];
	unsigned int mc_count;
	int csum_rx_enabled;
	u32 port_mtu;

	/* work queue cache line section */
	____cacheline_aligned struct vnic_wq wq[1];
	spinlock_t wq_lock[1];
	unsigned int wq_count;
	struct vlan_group *vlan_group;

	/* receive queue cache line section */
	____cacheline_aligned struct vnic_rq rq[1];
	unsigned int rq_count;
	int (*rq_alloc_buf)(struct vnic_rq *rq);
	struct napi_struct napi;
	struct net_lro_mgr lro_mgr;
	struct net_lro_desc lro_desc[ENIC_LRO_MAX_DESC];

	/* interrupt resource cache line section */
	____cacheline_aligned struct vnic_intr intr[ENIC_MSIX_MAX];
	unsigned int intr_count;
	u32 __iomem *legacy_pba;		/* memory-mapped */

	/* completion queue cache line section */
	____cacheline_aligned struct vnic_cq cq[ENIC_CQ_MAX];
	unsigned int cq_count;
};

#endif /* _ENIC_H_ */
