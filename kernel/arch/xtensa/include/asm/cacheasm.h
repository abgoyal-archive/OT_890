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
 * include/asm-xtensa/cacheasm.h
 *
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 *
 * Copyright (C) 2006 Tensilica Inc.
 */

#include <asm/cache.h>
#include <asm/asmmacro.h>
#include <linux/stringify.h>

/*
 * Define cache functions as macros here so that they can be used
 * by the kernel and boot loader. We should consider moving them to a
 * library that can be linked by both.
 *
 * Locking
 *
 *   ___unlock_dcache_all
 *   ___unlock_icache_all
 *
 * Flush and invaldating
 *
 *   ___flush_invalidate_dcache_{all|range|page}
 *   ___flush_dcache_{all|range|page}
 *   ___invalidate_dcache_{all|range|page}
 *   ___invalidate_icache_{all|range|page}
 *
 */

	.macro	__loop_cache_all ar at insn size line_width

	movi	\ar, 0

	__loopi	\ar, \at, \size, (4 << (\line_width))
	\insn	\ar, 0 << (\line_width)
	\insn	\ar, 1 << (\line_width)
	\insn	\ar, 2 << (\line_width)
	\insn	\ar, 3 << (\line_width)
	__endla	\ar, \at, 4 << (\line_width)

	.endm


	.macro	__loop_cache_range ar as at insn line_width

	extui	\at, \ar, 0, \line_width
	add	\as, \as, \at

	__loops	\ar, \as, \at, \line_width
	\insn	\ar, 0
	__endla	\ar, \at, (1 << (\line_width))

	.endm


	.macro	__loop_cache_page ar at insn line_width

	__loopi	\ar, \at, PAGE_SIZE, 4 << (\line_width)
	\insn	\ar, 0 << (\line_width)
	\insn	\ar, 1 << (\line_width)
	\insn	\ar, 2 << (\line_width)
	\insn	\ar, 3 << (\line_width)
	__endla	\ar, \at, 4 << (\line_width)

	.endm


#if XCHAL_DCACHE_LINE_LOCKABLE

	.macro	___unlock_dcache_all ar at

	__loop_cache_all \ar \at diu XCHAL_DCACHE_SIZE XCHAL_DCACHE_LINEWIDTH

	.endm

#endif

#if XCHAL_ICACHE_LINE_LOCKABLE

	.macro	___unlock_icache_all ar at

	__loop_cache_all \ar \at iiu XCHAL_ICACHE_SIZE XCHAL_ICACHE_LINEWIDTH

	.endm
#endif

	.macro	___flush_invalidate_dcache_all ar at

	__loop_cache_all \ar \at diwbi XCHAL_DCACHE_SIZE XCHAL_DCACHE_LINEWIDTH

	.endm


	.macro	___flush_dcache_all ar at

	__loop_cache_all \ar \at diwb XCHAL_DCACHE_SIZE XCHAL_DCACHE_LINEWIDTH

	.endm


	.macro	___invalidate_dcache_all ar at

	__loop_cache_all \ar \at dii __stringify(DCACHE_WAY_SIZE) \
			 XCHAL_DCACHE_LINEWIDTH

	.endm


	.macro	___invalidate_icache_all ar at

	__loop_cache_all \ar \at iii __stringify(ICACHE_WAY_SIZE) \
			 XCHAL_ICACHE_LINEWIDTH

	.endm



	.macro	___flush_invalidate_dcache_range ar as at

	__loop_cache_range \ar \as \at dhwbi XCHAL_DCACHE_LINEWIDTH

	.endm


	.macro	___flush_dcache_range ar as at

	__loop_cache_range \ar \as \at dhwb XCHAL_DCACHE_LINEWIDTH

	.endm


	.macro	___invalidate_dcache_range ar as at

	__loop_cache_range \ar \as \at dhi XCHAL_DCACHE_LINEWIDTH

	.endm


	.macro	___invalidate_icache_range ar as at

	__loop_cache_range \ar \as \at ihi XCHAL_ICACHE_LINEWIDTH

	.endm



	.macro	___flush_invalidate_dcache_page ar as

	__loop_cache_page \ar \as dhwbi XCHAL_DCACHE_LINEWIDTH

	.endm


	.macro ___flush_dcache_page ar as

	__loop_cache_page \ar \as dhwb XCHAL_DCACHE_LINEWIDTH

	.endm


	.macro	___invalidate_dcache_page ar as

	__loop_cache_page \ar \as dhi XCHAL_DCACHE_LINEWIDTH

	.endm


	.macro	___invalidate_icache_page ar as

	__loop_cache_page \ar \as ihi XCHAL_ICACHE_LINEWIDTH

	.endm

