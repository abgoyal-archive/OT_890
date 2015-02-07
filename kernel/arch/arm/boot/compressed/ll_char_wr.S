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
 *  linux/arch/arm/lib/ll_char_wr.S
 *
 *  Copyright (C) 1995, 1996 Russell King.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 *  Speedups & 1bpp code (C) 1996 Philip Blundell & Russell King.
 *
 *  10-04-96	RMK	Various cleanups & reduced register usage.
 *  08-04-98	RMK	Shifts re-ordered
 */

@ Regs: [] = corruptible
@       {} = used
@       () = do not use

#include <linux/linkage.h>
#include <asm/assembler.h>
	.text

LC0:	.word	LC0
	.word	bytes_per_char_h
	.word	video_size_row
	.word	acorndata_8x8
	.word	con_charconvtable

/*
 * r0 = ptr
 * r1 = char
 * r2 = white
 */
ENTRY(ll_write_char)
	stmfd	sp!, {r4 - r7, lr}
@
@ Smashable regs: {r0 - r3}, [r4 - r7], (r8 - fp), [ip], (sp), [lr], (pc)
@
	/*
	 * calculate offset into character table
	 */
	mov	r1, r1, lsl #3
	/*
	 * calculate offset required for each row.
	 */
	adr	ip, LC0
	ldmia	ip, {r3, r4, r5, r6, lr}
	sub	ip, ip, r3
	add	r6, r6, ip
	add	lr, lr, ip
	ldr	r4, [r4, ip]
	ldr	r5, [r5, ip]
	/*
	 * Go to resolution-dependent routine...
	 */
	cmp	r4, #4
	blt	Lrow1bpp
	add	r0, r0, r5, lsl #3		@ Move to bottom of character
	orr	r1, r1, #7
	ldrb	r7, [r6, r1]
	teq	r4, #8
	beq	Lrow8bpplp
@
@ Smashable regs: {r0 - r3}, [r4], {r5 - r7}, (r8 - fp), [ip], (sp), {lr}, (pc)
@
Lrow4bpplp:
	ldr	r7, [lr, r7, lsl #2]
	mul	r7, r2, r7
	sub	r1, r1, #1			@ avoid using r7 directly after
	str	r7, [r0, -r5]!
	ldrb	r7, [r6, r1]
	ldr	r7, [lr, r7, lsl #2]
	mul	r7, r2, r7
	tst	r1, #7				@ avoid using r7 directly after
	str	r7, [r0, -r5]!
	subne	r1, r1, #1
	ldrneb	r7, [r6, r1]
	bne	Lrow4bpplp
	ldmfd	sp!, {r4 - r7, pc}

@
@ Smashable regs: {r0 - r3}, [r4], {r5 - r7}, (r8 - fp), [ip], (sp), {lr}, (pc)
@
Lrow8bpplp:
	mov	ip, r7, lsr #4
	ldr	ip, [lr, ip, lsl #2]
	mul	r4, r2, ip
	and	ip, r7, #15			@ avoid r4
	ldr	ip, [lr, ip, lsl #2]		@ avoid r4
	mul	ip, r2, ip			@ avoid r4
	sub	r1, r1, #1			@ avoid ip
	sub	r0, r0, r5			@ avoid ip
	stmia	r0, {r4, ip}
	ldrb	r7, [r6, r1]
	mov	ip, r7, lsr #4
	ldr	ip, [lr, ip, lsl #2]
	mul	r4, r2, ip
	and	ip, r7, #15			@ avoid r4
	ldr	ip, [lr, ip, lsl #2]		@ avoid r4
	mul	ip, r2, ip			@ avoid r4
	tst	r1, #7				@ avoid ip
	sub	r0, r0, r5			@ avoid ip
	stmia	r0, {r4, ip}
	subne	r1, r1, #1
	ldrneb	r7, [r6, r1]
	bne	Lrow8bpplp
	ldmfd	sp!, {r4 - r7, pc}

@
@ Smashable regs: {r0 - r3}, [r4], {r5, r6}, [r7], (r8 - fp), [ip], (sp), [lr], (pc)
@
Lrow1bpp:
	add	r6, r6, r1
	ldmia	r6, {r4, r7}
	strb	r4, [r0], r5
	mov	r4, r4, lsr #8
	strb	r4, [r0], r5
	mov	r4, r4, lsr #8
	strb	r4, [r0], r5
	mov	r4, r4, lsr #8
	strb	r4, [r0], r5
	strb	r7, [r0], r5
	mov	r7, r7, lsr #8
	strb	r7, [r0], r5
	mov	r7, r7, lsr #8
	strb	r7, [r0], r5
	mov	r7, r7, lsr #8
	strb	r7, [r0], r5
	ldmfd	sp!, {r4 - r7, pc}

	.bss
ENTRY(con_charconvtable)
	.space	1024
