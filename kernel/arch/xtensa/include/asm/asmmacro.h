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
 * include/asm-xtensa/asmmacro.h
 *
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 *
 * Copyright (C) 2005 Tensilica Inc.
 */

#ifndef _XTENSA_ASMMACRO_H
#define _XTENSA_ASMMACRO_H

#include <variant/core.h>

/*
 * Some little helpers for loops. Use zero-overhead-loops
 * where applicable and if supported by the processor.
 *
 * __loopi ar, at, size, inc
 *         ar	register initialized with the start address
 *	   at	scratch register used by macro
 *	   size	size immediate value
 *	   inc	increment
 *
 * __loops ar, as, at, inc_log2[, mask_log2][, cond][, ncond]
 *	   ar	register initialized with the start address
 *	   as	register initialized with the size
 *	   at	scratch register use by macro
 *	   inc_log2	increment [in log2]
 *	   mask_log2	mask [in log2]
 *	   cond		true condition (used in loop'cond')
 *	   ncond	false condition (used in b'ncond')
 *
 * __loop  as
 *	   restart loop. 'as' register must not have been modified!
 *
 * __endla ar, at, incr
 *	   ar	start address (modified)
 *	   as	scratch register used by macro
 *	   inc	increment
 */

/*
 * loop for given size as immediate
 */

	.macro	__loopi ar, at, size, incr

#if XCHAL_HAVE_LOOPS
		movi	\at, ((\size + \incr - 1) / (\incr))
		loop	\at, 99f
#else
		addi	\at, \ar, \size
		98:
#endif

	.endm

/*
 * loop for given size in register
 */

	.macro	__loops	ar, as, at, incr_log2, mask_log2, cond, ncond

#if XCHAL_HAVE_LOOPS
		.ifgt \incr_log2 - 1
			addi	\at, \as, (1 << \incr_log2) - 1
			.ifnc \mask_log2,
				extui	\at, \at, \incr_log2, \mask_log2
			.else
				srli	\at, \at, \incr_log2
			.endif
		.endif
		loop\cond	\at, 99f
#else
		.ifnc \mask_log2,
			extui	\at, \as, \incr_log2, \mask_log2
		.else
			.ifnc \ncond,
				srli	\at, \as, \incr_log2
			.endif
		.endif
		.ifnc \ncond,
			b\ncond	\at, 99f

		.endif
		.ifnc \mask_log2,
			slli	\at, \at, \incr_log2
			add	\at, \ar, \at
		.else
			add	\at, \ar, \as
		.endif
#endif
		98:

	.endm

/*
 * loop from ar to ax
 */

	.macro	__loopt	ar, as, at, incr_log2

#if XCHAL_HAVE_LOOPS
		sub	\at, \as, \ar
		.ifgt	\incr_log2 - 1
			addi	\at, \at, (1 << \incr_log2) - 1
			srli	\at, \at, \incr_log2
		.endif
		loop	\at, 99f
#else
		98:
#endif

	.endm

/*
 * restart loop. registers must be unchanged
 */

	.macro	__loop	as

#if XCHAL_HAVE_LOOPS
		loop	\as, 99f
#else
		98:
#endif

	.endm

/*
 * end of loop with no increment of the address.
 */

	.macro	__endl	ar, as
#if !XCHAL_HAVE_LOOPS
		bltu	\ar, \as, 98b
#endif
		99:
	.endm

/*
 * end of loop with increment of the address.
 */

	.macro	__endla	ar, as, incr
		addi	\ar, \ar, \incr
		__endl	\ar \as
	.endm


#endif /* _XTENSA_ASMMACRO_H */
