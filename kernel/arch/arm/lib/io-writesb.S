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
 *  linux/arch/arm/lib/io-writesb.S
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
		strb	\rd, [r0]
		mov	\rd, \rd, lsr #8
		strb	\rd, [r0]
		mov	\rd, \rd, lsr #8
		strb	\rd, [r0]
		mov	\rd, \rd, lsr #8
		strb	\rd, [r0]
#else
		mov	lr, \rd, lsr #24
		strb	lr, [r0]
		mov	lr, \rd, lsr #16
		strb	lr, [r0]
		mov	lr, \rd, lsr #8
		strb	lr, [r0]
		strb	\rd, [r0]
#endif
		.endm

.Loutsb_align:	rsb	ip, ip, #4
		cmp	ip, r2
		movgt	ip, r2
		cmp	ip, #2
		ldrb	r3, [r1], #1
		strb	r3, [r0]
		ldrgeb	r3, [r1], #1
		strgeb	r3, [r0]
		ldrgtb	r3, [r1], #1
		strgtb	r3, [r0]
		subs	r2, r2, ip
		bne	.Loutsb_aligned

ENTRY(__raw_writesb)
		teq	r2, #0		@ do we have to check for the zero len?
		moveq	pc, lr
		ands	ip, r1, #3
		bne	.Loutsb_align

.Loutsb_aligned:
		stmfd	sp!, {r4, r5, lr}

		subs	r2, r2, #16
		bmi	.Loutsb_no_16

.Loutsb_16_lp:	ldmia	r1!, {r3, r4, r5, ip}
		outword	r3
		outword	r4
		outword	r5
		outword	ip
		subs	r2, r2, #16
		bpl	.Loutsb_16_lp

		tst	r2, #15
		ldmeqfd	sp!, {r4, r5, pc}

.Loutsb_no_16:	tst	r2, #8
		beq	.Loutsb_no_8

		ldmia	r1!, {r3, r4}
		outword	r3
		outword	r4

.Loutsb_no_8:	tst	r2, #4
		beq	.Loutsb_no_4

		ldr	r3, [r1], #4
		outword	r3

.Loutsb_no_4:	ands	r2, r2, #3
		ldmeqfd	sp!, {r4, r5, pc}

		cmp	r2, #2
		ldrb	r3, [r1], #1
		strb	r3, [r0]
		ldrgeb	r3, [r1], #1
		strgeb	r3, [r0]
		ldrgtb	r3, [r1]
		strgtb	r3, [r0]

		ldmfd	sp!, {r4, r5, pc}
ENDPROC(__raw_writesb)
