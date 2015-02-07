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
 *  linux/arch/arm/lib/io-readsb.S
 *
 *  Copyright (C) 1995-2000 Russell King
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */
#include <linux/linkage.h>
#include <asm/assembler.h>

.Linsb_align:	rsb	ip, ip, #4
		cmp	ip, r2
		movgt	ip, r2
		cmp	ip, #2
		ldrb	r3, [r0]
		strb	r3, [r1], #1
		ldrgeb	r3, [r0]
		strgeb	r3, [r1], #1
		ldrgtb	r3, [r0]
		strgtb	r3, [r1], #1
		subs	r2, r2, ip
		bne	.Linsb_aligned

ENTRY(__raw_readsb)
		teq	r2, #0		@ do we have to check for the zero len?
		moveq	pc, lr
		ands	ip, r1, #3
		bne	.Linsb_align

.Linsb_aligned:	stmfd	sp!, {r4 - r6, lr}

		subs	r2, r2, #16
		bmi	.Linsb_no_16

.Linsb_16_lp:	ldrb	r3, [r0]
		ldrb	r4, [r0]
		ldrb	r5, [r0]
		mov	r3, r3,     put_byte_0
		ldrb	r6, [r0]
		orr	r3, r3, r4, put_byte_1
		ldrb	r4, [r0]
		orr	r3, r3, r5, put_byte_2
		ldrb	r5, [r0]
		orr	r3, r3, r6, put_byte_3
		ldrb	r6, [r0]
		mov	r4, r4,     put_byte_0
		ldrb	ip, [r0]
		orr	r4, r4, r5, put_byte_1
		ldrb	r5, [r0]
		orr	r4, r4, r6, put_byte_2
		ldrb	r6, [r0]
		orr	r4, r4, ip, put_byte_3
		ldrb	ip, [r0]
		mov	r5, r5,     put_byte_0
		ldrb	lr, [r0]
		orr	r5, r5, r6, put_byte_1
		ldrb	r6, [r0]
		orr	r5, r5, ip, put_byte_2
		ldrb	ip, [r0]
		orr	r5, r5, lr, put_byte_3
		ldrb	lr, [r0]
		mov	r6, r6,     put_byte_0
		orr	r6, r6, ip, put_byte_1
		ldrb	ip, [r0]
		orr	r6, r6, lr, put_byte_2
		orr	r6, r6, ip, put_byte_3
		stmia	r1!, {r3 - r6}

		subs	r2, r2, #16
		bpl	.Linsb_16_lp

		tst	r2, #15
		ldmeqfd	sp!, {r4 - r6, pc}

.Linsb_no_16:	tst	r2, #8
		beq	.Linsb_no_8

		ldrb	r3, [r0]
		ldrb	r4, [r0]
		ldrb	r5, [r0]
		mov	r3, r3,     put_byte_0
		ldrb	r6, [r0]
		orr	r3, r3, r4, put_byte_1
		ldrb	r4, [r0]
		orr	r3, r3, r5, put_byte_2
		ldrb	r5, [r0]
		orr	r3, r3, r6, put_byte_3
		ldrb	r6, [r0]
		mov	r4, r4,     put_byte_0
		ldrb	ip, [r0]
		orr	r4, r4, r5, put_byte_1
		orr	r4, r4, r6, put_byte_2
		orr	r4, r4, ip, put_byte_3
		stmia	r1!, {r3, r4}

.Linsb_no_8:	tst	r2, #4
		beq	.Linsb_no_4

		ldrb	r3, [r0]
		ldrb	r4, [r0]
		ldrb	r5, [r0]
		ldrb	r6, [r0]
		mov	r3, r3,     put_byte_0
		orr	r3, r3, r4, put_byte_1
		orr	r3, r3, r5, put_byte_2
		orr	r3, r3, r6, put_byte_3
		str	r3, [r1], #4

.Linsb_no_4:	ands	r2, r2, #3
		ldmeqfd	sp!, {r4 - r6, pc}

		cmp	r2, #2
		ldrb	r3, [r0]
		strb	r3, [r1], #1
		ldrgeb	r3, [r0]
		strgeb	r3, [r1], #1
		ldrgtb	r3, [r0]
		strgtb	r3, [r1]

		ldmfd	sp!, {r4 - r6, pc}
ENDPROC(__raw_readsb)
