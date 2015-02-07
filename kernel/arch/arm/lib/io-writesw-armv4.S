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
 *  linux/arch/arm/lib/io-writesw-armv4.S
 *
 *  Copyright (C) 1995-2000 Russell King
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */
#include <linux/linkage.h>
#include <asm/assembler.h>

		.macro	outword, rd
#ifndef __ARMEB__
		strh	\rd, [r0]
		mov	\rd, \rd, lsr #16
		strh	\rd, [r0]
#else
		mov	lr, \rd, lsr #16
		strh	lr, [r0]
		strh	\rd, [r0]
#endif
		.endm

.Loutsw_align:	movs	ip, r1, lsl #31
		bne	.Loutsw_noalign

		ldrh	r3, [r1], #2
		sub	r2, r2, #1
		strh	r3, [r0]

ENTRY(__raw_writesw)
		teq	r2, #0
		moveq	pc, lr
		ands	r3, r1, #3
		bne	.Loutsw_align

		stmfd	sp!, {r4, r5, lr}

		subs	r2, r2, #8
		bmi	.Lno_outsw_8

.Loutsw_8_lp:	ldmia	r1!, {r3, r4, r5, ip}
		subs	r2, r2, #8
		outword	r3
		outword	r4
		outword	r5
		outword	ip
		bpl	.Loutsw_8_lp

.Lno_outsw_8:	tst	r2, #4
		beq	.Lno_outsw_4

		ldmia	r1!, {r3, ip}
		outword	r3
		outword	ip

.Lno_outsw_4:	movs	r2, r2, lsl #31
		bcc	.Lno_outsw_2

		ldr	r3, [r1], #4
		outword	r3

.Lno_outsw_2:	ldrneh	r3, [r1]
		strneh	r3, [r0]

		ldmfd	sp!, {r4, r5, pc}

#ifdef __ARMEB__
#define pull_hbyte0	lsl #8
#define push_hbyte1	lsr #24
#else
#define pull_hbyte0	lsr #24
#define push_hbyte1	lsl #8
#endif

.Loutsw_noalign:
		ldr	r3, [r1, -r3]!
		subcs	r2, r2, #1
		bcs	2f
		subs	r2, r2, #2
		bmi	3f

1:		mov	ip, r3, lsr #8
		strh	ip, [r0]
2:		mov	ip, r3, pull_hbyte0
		ldr	r3, [r1, #4]!
		subs	r2, r2, #2
		orr	ip, ip, r3, push_hbyte1
		strh	ip, [r0]
		bpl	1b

		tst	r2, #1
3:		movne	ip, r3, lsr #8
		strneh	ip, [r0]
		mov	pc, lr
ENDPROC(__raw_writesw)
