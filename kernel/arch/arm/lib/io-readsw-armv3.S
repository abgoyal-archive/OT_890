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
 *  linux/arch/arm/lib/io-readsw-armv3.S
 *
 *  Copyright (C) 1995-2000 Russell King
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */
#include <linux/linkage.h>
#include <asm/assembler.h>
#include <mach/hardware.h>

.Linsw_bad_alignment:
		adr	r0, .Linsw_bad_align_msg
		mov	r2, lr
		b	panic
.Linsw_bad_align_msg:
		.asciz	"insw: bad buffer alignment (0x%p, lr=0x%08lX)\n"
		.align

.Linsw_align:	tst	r1, #1
		bne	.Linsw_bad_alignment

		ldr	r3, [r0]
		strb	r3, [r1], #1
		mov	r3, r3, lsr #8
		strb	r3, [r1], #1

		subs	r2, r2, #1
		moveq	pc, lr

ENTRY(__raw_readsw)
		teq	r2, #0		@ do we have to check for the zero len?
		moveq	pc, lr
		tst	r1, #3
		bne	.Linsw_align

.Linsw_aligned:	mov	ip, #0xff
		orr	ip, ip, ip, lsl #8
		stmfd	sp!, {r4, r5, r6, lr}

		subs	r2, r2, #8
		bmi	.Lno_insw_8

.Linsw_8_lp:	ldr	r3, [r0]
		and	r3, r3, ip
		ldr	r4, [r0]
		orr	r3, r3, r4, lsl #16

		ldr	r4, [r0]
		and	r4, r4, ip
		ldr	r5, [r0]
		orr	r4, r4, r5, lsl #16

		ldr	r5, [r0]
		and	r5, r5, ip
		ldr	r6, [r0]
		orr	r5, r5, r6, lsl #16

		ldr	r6, [r0]
		and	r6, r6, ip
		ldr	lr, [r0]
		orr	r6, r6, lr, lsl #16

		stmia	r1!, {r3 - r6}

		subs	r2, r2, #8
		bpl	.Linsw_8_lp

		tst	r2, #7
		ldmeqfd	sp!, {r4, r5, r6, pc}

.Lno_insw_8:	tst	r2, #4
		beq	.Lno_insw_4

		ldr	r3, [r0]
		and	r3, r3, ip
		ldr	r4, [r0]
		orr	r3, r3, r4, lsl #16

		ldr	r4, [r0]
		and	r4, r4, ip
		ldr	r5, [r0]
		orr	r4, r4, r5, lsl #16

		stmia	r1!, {r3, r4}

.Lno_insw_4:	tst	r2, #2
		beq	.Lno_insw_2

		ldr	r3, [r0]
		and	r3, r3, ip
		ldr	r4, [r0]
		orr	r3, r3, r4, lsl #16

		str	r3, [r1], #4

.Lno_insw_2:	tst	r2, #1
		ldrne	r3, [r0]
		strneb	r3, [r1], #1
		movne	r3, r3, lsr #8
		strneb	r3, [r1]

		ldmfd	sp!, {r4, r5, r6, pc}


