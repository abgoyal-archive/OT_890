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
 *  linux/arch/arm/lib/copy_from_user.S
 *
 *  Author:	Nicolas Pitre
 *  Created:	Sep 29, 2005
 *  Copyright:	MontaVista Software, Inc.
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License version 2 as
 *  published by the Free Software Foundation.
 */

#include <linux/linkage.h>
#include <asm/assembler.h>

/*
 * Prototype:
 *
 *	size_t __copy_from_user(void *to, const void *from, size_t n)
 *
 * Purpose:
 *
 *	copy a block to kernel memory from user memory
 *
 * Params:
 *
 *	to = kernel memory
 *	from = user memory
 *	n = number of bytes to copy
 *
 * Return value:
 *
 *	Number of bytes NOT copied.
 */

	.macro ldr1w ptr reg abort
100:	ldrt \reg, [\ptr], #4
	.section __ex_table, "a"
	.long 100b, \abort
	.previous
	.endm

	.macro ldr4w ptr reg1 reg2 reg3 reg4 abort
	ldr1w \ptr, \reg1, \abort
	ldr1w \ptr, \reg2, \abort
	ldr1w \ptr, \reg3, \abort
	ldr1w \ptr, \reg4, \abort
	.endm

	.macro ldr8w ptr reg1 reg2 reg3 reg4 reg5 reg6 reg7 reg8 abort
	ldr4w \ptr, \reg1, \reg2, \reg3, \reg4, \abort
	ldr4w \ptr, \reg5, \reg6, \reg7, \reg8, \abort
	.endm

	.macro ldr1b ptr reg cond=al abort
100:	ldr\cond\()bt \reg, [\ptr], #1
	.section __ex_table, "a"
	.long 100b, \abort
	.previous
	.endm

	.macro str1w ptr reg abort
	str \reg, [\ptr], #4
	.endm

	.macro str8w ptr reg1 reg2 reg3 reg4 reg5 reg6 reg7 reg8 abort
	stmia \ptr!, {\reg1, \reg2, \reg3, \reg4, \reg5, \reg6, \reg7, \reg8}
	.endm

	.macro str1b ptr reg cond=al abort
	str\cond\()b \reg, [\ptr], #1
	.endm

	.macro enter reg1 reg2
	mov	r3, #0
	stmdb	sp!, {r0, r2, r3, \reg1, \reg2}
	.endm

	.macro exit reg1 reg2
	add	sp, sp, #8
	ldmfd	sp!, {r0, \reg1, \reg2}
	.endm

	.text

ENTRY(__copy_from_user)

#include "copy_template.S"

ENDPROC(__copy_from_user)

	.section .fixup,"ax"
	.align 0
	copy_abort_preamble
	ldmfd	sp!, {r1, r2}
	sub	r3, r0, r1
	rsb	r1, r3, r2
	str	r1, [sp]
	bl	__memzero
	ldr	r0, [sp], #4
	copy_abort_end
	.previous

