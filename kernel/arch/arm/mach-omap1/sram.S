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
 * linux/arch/arm/plat-omap/sram-fn.S
 *
 * Functions that need to be run in internal SRAM
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#include <linux/linkage.h>
#include <asm/assembler.h>
#include <mach/io.h>
#include <mach/hardware.h>

	.text

/*
 * Reprograms ULPD and CKCTL.
 */
ENTRY(omap1_sram_reprogram_clock)
	stmfd	sp!, {r0 - r12, lr}		@ save registers on stack

	mov	r2, #IO_ADDRESS(DPLL_CTL) & 0xff000000
	orr	r2, r2, #IO_ADDRESS(DPLL_CTL) & 0x00ff0000
	orr	r2, r2, #IO_ADDRESS(DPLL_CTL) & 0x0000ff00

	mov	r3, #IO_ADDRESS(ARM_CKCTL) & 0xff000000
	orr	r3, r3, #IO_ADDRESS(ARM_CKCTL) & 0x00ff0000
	orr	r3, r3, #IO_ADDRESS(ARM_CKCTL) & 0x0000ff00

	tst	r0, #1 << 4			@ want lock mode?
	beq	newck				@ nope
	bic	r0, r0, #1 << 4			@ else clear lock bit
	strh	r0, [r2]			@ set dpll into bypass mode
	orr	r0, r0, #1 << 4			@ set lock bit again

newck:
	strh	r1, [r3]			@ write new ckctl value
	strh	r0, [r2]			@ write new dpll value

	mov	r4, #0x0700			@ let the clocks settle
	orr	r4, r4, #0x00ff
delay:	sub	r4, r4, #1
	cmp	r4, #0
	bne	delay

lock:	ldrh	r4, [r2], #0			@ read back dpll value
	tst	r0, #1 << 4			@ want lock mode?
	beq	out				@ nope
	tst	r4, #1 << 0			@ dpll rate locked?
	beq	lock				@ try again

out:
	ldmfd	sp!, {r0 - r12, pc}		@ restore regs and return
ENTRY(omap1_sram_reprogram_clock_sz)
	.word	. - omap1_sram_reprogram_clock
