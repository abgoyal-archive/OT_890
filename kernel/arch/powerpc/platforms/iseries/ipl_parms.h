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
#ifndef _ISERIES_IPL_PARMS_H
#define _ISERIES_IPL_PARMS_H

/*
 *	This struct maps the IPL Parameters DMA'd from the SP.
 *
 * Warning:
 *	This data must map in exactly 64 bytes and match the architecture for
 *	the IPL parms
 */

#include <asm/types.h>

struct ItIplParmsReal {
	u8	xFormat;		// Defines format of IplParms	x00-x00
	u8	xRsvd01:6;		// Reserved			x01-x01
	u8	xAlternateSearch:1;	// Alternate search indicator	...
	u8	xUaSupplied:1;		// UA Supplied on programmed IPL...
	u8	xLsUaFormat;		// Format byte for UA		x02-x02
	u8	xRsvd02;		// Reserved			x03-x03
	u32	xLsUa;			// LS UA			x04-x07
	u32	xUnusedLsLid;		// First OS LID to load		x08-x0B
	u16	xLsBusNumber;		// LS Bus Number		x0C-x0D
	u8	xLsCardAdr;		// LS Card Address		x0E-x0E
	u8	xLsBoardAdr;		// LS Board Address		x0F-x0F
	u32	xRsvd03;		// Reserved			x10-x13
	u8	xSpcnPresent:1;		// SPCN present			x14-x14
	u8	xCpmPresent:1;		// CPM present			...
	u8	xRsvd04:6;		// Reserved			...
	u8	xRsvd05:4;		// Reserved			x15-x15
	u8	xKeyLock:4;		// Keylock setting		...
	u8	xRsvd06:6;		// Reserved			x16-x16
	u8	xIplMode:2;		// Ipl mode (A|B|C|D)		...
	u8	xHwIplType;		// Fast v slow v slow EC HW IPL	x17-x17
	u16	xCpmEnabledIpl:1;	// CPM in effect when IPL initiatedx18-x19
	u16	xPowerOnResetIpl:1;	// Indicate POR condition	...
	u16	xMainStorePreserved:1;	// Main Storage is preserved	...
	u16	xRsvd07:13;		// Reserved			...
	u16	xIplSource:16;		// Ipl source			x1A-x1B
	u8	xIplReason:8;		// Reason for this IPL		x1C-x1C
	u8	xRsvd08;		// Reserved			x1D-x1D
	u16	xRsvd09;		// Reserved			x1E-x1F
	u16	xSysBoxType;		// System Box Type		x20-x21
	u16	xSysProcType;		// System Processor Type	x22-x23
	u32	xRsvd10;		// Reserved			x24-x27
	u64	xRsvd11;		// Reserved			x28-x2F
	u64	xRsvd12;		// Reserved			x30-x37
	u64	xRsvd13;		// Reserved			x38-x3F
};

#endif /* _ISERIES_IPL_PARMS_H */
