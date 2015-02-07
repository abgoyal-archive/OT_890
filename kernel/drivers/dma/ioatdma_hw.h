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
 * Copyright(c) 2004 - 2009 Intel Corporation. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation; either version 2 of the License, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program; if not, write to the Free Software Foundation, Inc., 59
 * Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 *
 * The full GNU General Public License is included in this distribution in the
 * file called COPYING.
 */
#ifndef _IOAT_HW_H_
#define _IOAT_HW_H_

/* PCI Configuration Space Values */
#define IOAT_PCI_VID            0x8086

/* CB device ID's */
#define IOAT_PCI_DID_5000       0x1A38
#define IOAT_PCI_DID_CNB        0x360B
#define IOAT_PCI_DID_SCNB       0x65FF
#define IOAT_PCI_DID_SNB        0x402F

#define IOAT_PCI_RID            0x00
#define IOAT_PCI_SVID           0x8086
#define IOAT_PCI_SID            0x8086
#define IOAT_VER_1_2            0x12    /* Version 1.2 */
#define IOAT_VER_2_0            0x20    /* Version 2.0 */
#define IOAT_VER_3_0            0x30    /* Version 3.0 */

struct ioat_dma_descriptor {
	uint32_t	size;
	uint32_t	ctl;
	uint64_t	src_addr;
	uint64_t	dst_addr;
	uint64_t	next;
	uint64_t	rsv1;
	uint64_t	rsv2;
	uint64_t	user1;
	uint64_t	user2;
};

#define IOAT_DMA_DESCRIPTOR_CTL_INT_GN	0x00000001
#define IOAT_DMA_DESCRIPTOR_CTL_SRC_SN	0x00000002
#define IOAT_DMA_DESCRIPTOR_CTL_DST_SN	0x00000004
#define IOAT_DMA_DESCRIPTOR_CTL_CP_STS	0x00000008
#define IOAT_DMA_DESCRIPTOR_CTL_FRAME	0x00000010
#define IOAT_DMA_DESCRIPTOR_NUL		0x00000020
#define IOAT_DMA_DESCRIPTOR_CTL_SP_BRK	0x00000040
#define IOAT_DMA_DESCRIPTOR_CTL_DP_BRK	0x00000080
#define IOAT_DMA_DESCRIPTOR_CTL_BNDL	0x00000100
#define IOAT_DMA_DESCRIPTOR_CTL_DCA	0x00000200
#define IOAT_DMA_DESCRIPTOR_CTL_BUFHINT	0x00000400

#define IOAT_DMA_DESCRIPTOR_CTL_OPCODE_CONTEXT	0xFF000000
#define IOAT_DMA_DESCRIPTOR_CTL_OPCODE_DMA	0x00000000

#define IOAT_DMA_DESCRIPTOR_CTL_CONTEXT_DCA	0x00000001
#define IOAT_DMA_DESCRIPTOR_CTL_OPCODE_MASK	0xFF000000

#endif
