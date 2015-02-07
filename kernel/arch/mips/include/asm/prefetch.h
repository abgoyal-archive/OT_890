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
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 *
 * Copyright (C) 2003 by Ralf Baechle
 */
#ifndef __ASM_PREFETCH_H
#define __ASM_PREFETCH_H


/*
 * R5000 and RM5200 implements pref and prefx instructions but they're nops, so
 * rather than wasting time we pretend these processors don't support
 * prefetching at all.
 *
 * R5432 implements Load, Store, LoadStreamed, StoreStreamed, LoadRetained,
 * StoreRetained and WriteBackInvalidate but not Pref_PrepareForStore.
 *
 * Hell (and the book on my shelf I can't open ...) know what the R8000 does.
 *
 * RM7000 version 1.0 interprets all hints as Pref_Load; version 2.0 implements
 * Pref_PrepareForStore also.
 *
 * RM9000 is MIPS IV but implements prefetching like MIPS32/MIPS64; it's
 * Pref_WriteBackInvalidate is a nop and Pref_PrepareForStore is broken in
 * current versions due to erratum G105.
 *
 * VR5500 (including VR5701 and VR7701) only implement load prefetch.
 *
 * Finally MIPS32 and MIPS64 implement all of the following hints.
 */

#define Pref_Load			0
#define Pref_Store			1
						/* 2 and 3 are reserved */
#define Pref_LoadStreamed		4
#define Pref_StoreStreamed		5
#define Pref_LoadRetained		6
#define Pref_StoreRetained		7
						/* 8 ... 24 are reserved */
#define Pref_WriteBackInvalidate	25
#define Pref_PrepareForStore		30

#ifdef __ASSEMBLY__

	.macro	__pref hint addr
#ifdef CONFIG_CPU_HAS_PREFETCH
	pref	\hint, \addr
#endif
	.endm

	.macro	pref_load addr
	__pref	Pref_Load, \addr
	.endm

	.macro	pref_store addr
	__pref	Pref_Store, \addr
	.endm

	.macro	pref_load_streamed addr
	__pref	Pref_LoadStreamed, \addr
	.endm

	.macro	pref_store_streamed addr
	__pref	Pref_StoreStreamed, \addr
	.endm

	.macro	pref_load_retained addr
	__pref	Pref_LoadRetained, \addr
	.endm

	.macro	pref_store_retained addr
	__pref	Pref_StoreRetained, \addr
	.endm

	.macro	pref_wback_inv addr
	__pref	Pref_WriteBackInvalidate, \addr
	.endm

	.macro	pref_prepare_for_store addr
	__pref	Pref_PrepareForStore, \addr
	.endm

#endif

#endif /* __ASM_PREFETCH_H */
