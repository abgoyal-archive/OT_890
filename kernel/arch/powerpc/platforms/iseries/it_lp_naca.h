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
 * Copyright (C) 2001  Mike Corrigan IBM Corporation
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
 */
#ifndef _PLATFORMS_ISERIES_IT_LP_NACA_H
#define _PLATFORMS_ISERIES_IT_LP_NACA_H

#include <linux/types.h>

/*
 *	This control block contains the data that is shared between the
 *	hypervisor (PLIC) and the OS.
 */

struct ItLpNaca {
// CACHE_LINE_1 0x0000 - 0x007F Contains read-only data
	u32	xDesc;			// Eye catcher			x00-x03
	u16	xSize;			// Size of this class		x04-x05
	u16	xIntHdlrOffset;		// Offset to IntHdlr array	x06-x07
	u8	xMaxIntHdlrEntries;	// Number of entries in array	x08-x08
	u8	xPrimaryLpIndex;	// LP Index of Primary		x09-x09
	u8	xServiceLpIndex;	// LP Ind of Service Focal Pointx0A-x0A
	u8	xLpIndex;		// LP Index			x0B-x0B
	u16	xMaxLpQueues;		// Number of allocated queues	x0C-x0D
	u16	xLpQueueOffset;		// Offset to start of LP queues	x0E-x0F
	u8	xPirEnvironMode;	// Piranha or hardware		x10-x10
	u8	xPirConsoleMode;	// Piranha console indicator	x11-x11
	u8	xPirDasdMode;		// Piranha dasd indicator	x12-x12
	u8	xRsvd1_0[5];		// Reserved for Piranha related	x13-x17
	u8	flags;			// flags, see below		x18-x1F
	u8	xSpVpdFormat;		// VPD areas are in CSP format	...
	u8	xIntProcRatio;		// Ratio of int procs to procs	...
	u8	xRsvd1_2[5];		// Reserved			...
	u16	xRsvd1_3;		// Reserved			x20-x21
	u16	xPlicVrmIndex;		// VRM index of PLIC		x22-x23
	u16	xMinSupportedSlicVrmInd;// Min supported OS VRM index	x24-x25
	u16	xMinCompatableSlicVrmInd;// Min compatible OS VRM index x26-x27
	u64	xLoadAreaAddr;		// ER address of load area	x28-x2F
	u32	xLoadAreaChunks;	// Chunks for the load area	x30-x33
	u32	xPaseSysCallCRMask;	// Mask used to test CR before  x34-x37
					// doing an ASR switch on PASE
					// system call.
	u64	xSlicSegmentTablePtr;	// Pointer to Slic seg table.   x38-x3f
	u8	xRsvd1_4[64];		//				x40-x7F

// CACHE_LINE_2 0x0080 - 0x00FF Contains local read-write data
	u8	xRsvd2_0[128];		// Reserved			x00-x7F

// CACHE_LINE_3-6 0x0100 - 0x02FF Contains LP Queue indicators
// NB: Padding required to keep xInterruptHdlr at x300 which is required
// for v4r4 PLIC.
	u8	xOldLpQueue[128];	// LP Queue needed for v4r4	100-17F
	u8	xRsvd3_0[384];		// Reserved			180-2FF

// CACHE_LINE_7-8 0x0300 - 0x03FF Contains the address of the OS interrupt
//  handlers
	u64	xInterruptHdlr[32];	// Interrupt handlers		300-x3FF
};

extern struct ItLpNaca		itLpNaca;

#define ITLPNACA_LPAR		0x80	/* Is LPAR installed on the system */
#define ITLPNACA_PARTITIONED	0x40	/* Is the system partitioned */
#define ITLPNACA_HWSYNCEDTBS	0x20	/* Hardware synced TBs */
#define ITLPNACA_HMTINT		0x10	/* Utilize MHT for interrupts */

#endif /* _PLATFORMS_ISERIES_IT_LP_NACA_H */
