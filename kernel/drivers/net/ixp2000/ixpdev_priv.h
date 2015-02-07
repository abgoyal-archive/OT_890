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
 * IXP2000 MSF network device driver
 * Copyright (C) 2004, 2005 Lennert Buytenhek <buytenh@wantstofly.org>
 * Dedicated to Marija Kulikova.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */

#ifndef __IXPDEV_PRIV_H
#define __IXPDEV_PRIV_H

#define RX_BUF_DESC_BASE	0x00001000
#define RX_BUF_COUNT		((3 * PAGE_SIZE) / (4 * sizeof(struct ixpdev_rx_desc)))
#define TX_BUF_DESC_BASE	0x00002000
#define TX_BUF_COUNT		((3 * PAGE_SIZE) / (4 * sizeof(struct ixpdev_tx_desc)))
#define TX_BUF_COUNT_PER_CHAN	(TX_BUF_COUNT / 4)

#define RING_RX_PENDING		((u32 *)IXP2000_SCRATCH_RING_VIRT_BASE)
#define RING_RX_DONE		((u32 *)(IXP2000_SCRATCH_RING_VIRT_BASE + 4))
#define RING_TX_PENDING		((u32 *)(IXP2000_SCRATCH_RING_VIRT_BASE + 8))
#define RING_TX_DONE		((u32 *)(IXP2000_SCRATCH_RING_VIRT_BASE + 12))

#define SCRATCH_REG(x)		((u32 *)(IXP2000_GLOBAL_REG_VIRT_BASE | 0x0800 | (x)))
#define RING_RX_PENDING_BASE	SCRATCH_REG(0x00)
#define RING_RX_PENDING_HEAD	SCRATCH_REG(0x04)
#define RING_RX_PENDING_TAIL	SCRATCH_REG(0x08)
#define RING_RX_DONE_BASE	SCRATCH_REG(0x10)
#define RING_RX_DONE_HEAD	SCRATCH_REG(0x14)
#define RING_RX_DONE_TAIL	SCRATCH_REG(0x18)
#define RING_TX_PENDING_BASE	SCRATCH_REG(0x20)
#define RING_TX_PENDING_HEAD	SCRATCH_REG(0x24)
#define RING_TX_PENDING_TAIL	SCRATCH_REG(0x28)
#define RING_TX_DONE_BASE	SCRATCH_REG(0x30)
#define RING_TX_DONE_HEAD	SCRATCH_REG(0x34)
#define RING_TX_DONE_TAIL	SCRATCH_REG(0x38)

struct ixpdev_rx_desc
{
	u32	buf_addr;
	u32	buf_length;
	u32	channel;
	u32	pkt_length;
};

struct ixpdev_tx_desc
{
	u32	buf_addr;
	u32	pkt_length;
	u32	channel;
	u32	unused;
};


#endif
