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
 *  linux/arch/arm/lib/io-readsl.S
 *
 *  Copyright (C) 1995-2000 Russell King
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */
#include <linux/linkage.h>
#include <asm/assembler.h>

ENTRY(__raw_readsl)
		teq	r2, #0		@ do we have to check for the zero len?
		moveq	pc, lr
		ands	ip, r1, #3
		bne	3f

		subs	r2, r2, #4
		bmi	2f
		stmfd	sp!, {r4, lr}
1:		ldr	r3, [r0, #0]
		ldr	r4, [r0, #0]
		ldr	ip, [r0, #0]
		ldr	lr, [r0, #0]
		subs	r2, r2, #4
		stmia	r1!, {r3, r4, ip, lr}
		bpl	1b
		ldmfd	sp!, {r4, lr}
2:		movs	r2, r2, lsl #31
		ldrcs	r3, [r0, #0]
		ldrcs	ip, [r0, #0]
		stmcsia	r1!, {r3, ip}
		ldrne	r3, [r0, #0]
		strne	r3, [r1, #0]
		mov	pc, lr

3:		ldr	r3, [r0]
		cmp	ip, #2
		mov	ip, r3, get_byte_0
		strb	ip, [r1], #1
		bgt	6f
		mov	ip, r3, get_byte_1
		strb	ip, [r1], #1
		beq	5f
		mov	ip, r3, get_byte_2
		strb	ip, [r1], #1

4:		subs	r2, r2, #1
		mov	ip, r3, pull #24
		ldrne	r3, [r0]
		orrne	ip, ip, r3, push #8
		strne	ip, [r1], #4
		bne	4b
		b	8f

5:		subs	r2, r2, #1
		mov	ip, r3, pull #16
		ldrne	r3, [r0]
		orrne	ip, ip, r3, push #16
		strne	ip, [r1], #4
		bne	5b
		b	7f

6:		subs	r2, r2, #1
		mov	ip, r3, pull #8
		ldrne	r3, [r0]
		orrne	ip, ip, r3, push #24
		strne	ip, [r1], #4
		bne	6b

		mov	r3, ip, get_byte_2
		strb	r3, [r1, #2]
7:		mov	r3, ip, get_byte_1
		strb	r3, [r1, #1]
8:		mov	r3, ip, get_byte_0
		strb	r3, [r1, #0]
		mov	pc, lr
ENDPROC(__raw_readsl)
