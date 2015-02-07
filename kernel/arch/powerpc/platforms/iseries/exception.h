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

#ifndef _ASM_POWERPC_ISERIES_EXCEPTION_H
#define _ASM_POWERPC_ISERIES_EXCEPTION_H
/*
 * Extracted from head_64.S
 *
 *  PowerPC version
 *    Copyright (C) 1995-1996 Gary Thomas (gdt@linuxppc.org)
 *
 *  Rewritten by Cort Dougan (cort@cs.nmt.edu) for PReP
 *    Copyright (C) 1996 Cort Dougan <cort@cs.nmt.edu>
 *  Adapted for Power Macintosh by Paul Mackerras.
 *  Low-level exception handlers and MMU support
 *  rewritten by Paul Mackerras.
 *    Copyright (C) 1996 Paul Mackerras.
 *
 *  Adapted for 64bit PowerPC by Dave Engebretsen, Peter Bergner, and
 *    Mike Corrigan {engebret|bergner|mikejc}@us.ibm.com
 *
 *  This file contains the low-level support and setup for the
 *  PowerPC-64 platform, including trap and interrupt dispatch.
 *
 *  This program is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  as published by the Free Software Foundation; either version
 *  2 of the License, or (at your option) any later version.
 */
#include <asm/exception.h>

#define EXCEPTION_PROLOG_ISERIES_1					\
	mfmsr	r10;							\
	ld	r12,PACALPPACAPTR(r13);					\
	ld	r11,LPPACASRR0(r12);					\
	ld	r12,LPPACASRR1(r12);					\
	ori	r10,r10,MSR_RI;						\
	mtmsrd	r10,1

#define STD_EXCEPTION_ISERIES(label, area)				\
	.globl label##_iSeries;						\
label##_iSeries:							\
	HMT_MEDIUM;							\
	mtspr	SPRN_SPRG1,r13;		/* save r13 */			\
	EXCEPTION_PROLOG_1(area);					\
	EXCEPTION_PROLOG_ISERIES_1;					\
	b	label##_common

#define MASKABLE_EXCEPTION_ISERIES(label)				\
	.globl label##_iSeries;						\
label##_iSeries:							\
	HMT_MEDIUM;							\
	mtspr	SPRN_SPRG1,r13;		/* save r13 */			\
	EXCEPTION_PROLOG_1(PACA_EXGEN);					\
	lbz	r10,PACASOFTIRQEN(r13);					\
	cmpwi	0,r10,0;						\
	beq-	label##_iSeries_masked;					\
	EXCEPTION_PROLOG_ISERIES_1;					\
	b	label##_common;						\

#endif	/* _ASM_POWERPC_ISERIES_EXCEPTION_H */
