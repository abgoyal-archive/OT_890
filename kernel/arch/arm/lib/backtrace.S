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
 *  linux/arch/arm/lib/backtrace.S
 *
 *  Copyright (C) 1995, 1996 Russell King
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * 27/03/03 Ian Molton Clean up CONFIG_CPU
 *
 */
#include <linux/linkage.h>
#include <asm/assembler.h>
		.text

@ fp is 0 or stack frame

#define frame	r4
#define sv_fp	r5
#define sv_pc	r6
#define mask	r7
#define offset	r8

ENTRY(__backtrace)
		mov	r1, #0x10
		mov	r0, fp

ENTRY(c_backtrace)

#if !defined(CONFIG_FRAME_POINTER) || !defined(CONFIG_PRINTK)
		mov	pc, lr
ENDPROC(__backtrace)
ENDPROC(c_backtrace)
#else
		stmfd	sp!, {r4 - r8, lr}	@ Save an extra register so we have a location...
		movs	frame, r0		@ if frame pointer is zero
		beq	no_frame		@ we have no stack frames

		tst	r1, #0x10		@ 26 or 32-bit mode?
		moveq	mask, #0xfc000003	@ mask for 26-bit
		movne	mask, #0		@ mask for 32-bit

1:		stmfd	sp!, {pc}		@ calculate offset of PC stored
		ldr	r0, [sp], #4		@ by stmfd for this CPU
		adr	r1, 1b
		sub	offset, r0, r1

/*
 * Stack frame layout:
 *             optionally saved caller registers (r4 - r10)
 *             saved fp
 *             saved sp
 *             saved lr
 *    frame => saved pc
 *             optionally saved arguments (r0 - r3)
 * saved sp => <next word>
 *
 * Functions start with the following code sequence:
 *                  mov   ip, sp
 *                  stmfd sp!, {r0 - r3} (optional)
 * corrected pc =>  stmfd sp!, {..., fp, ip, lr, pc}
 */
for_each_frame:	tst	frame, mask		@ Check for address exceptions
		bne	no_frame

1001:		ldr	sv_pc, [frame, #0]	@ get saved pc
1002:		ldr	sv_fp, [frame, #-12]	@ get saved fp

		sub	sv_pc, sv_pc, offset	@ Correct PC for prefetching
		bic	sv_pc, sv_pc, mask	@ mask PC/LR for the mode

1003:		ldr	r2, [sv_pc, #-4]	@ if stmfd sp!, {args} exists,
		ldr	r3, .Ldsi+4		@ adjust saved 'pc' back one
		teq	r3, r2, lsr #10		@ instruction
		subne	r0, sv_pc, #4		@ allow for mov
		subeq	r0, sv_pc, #8		@ allow for mov + stmia

		ldr	r1, [frame, #-4]	@ get saved lr
		mov	r2, frame
		bic	r1, r1, mask		@ mask PC/LR for the mode
		bl	dump_backtrace_entry

		ldr	r1, [sv_pc, #-4]	@ if stmfd sp!, {args} exists,
		ldr	r3, .Ldsi+4
		teq	r3, r1, lsr #10
		ldreq	r0, [frame, #-8]	@ get sp
		subeq	r0, r0, #4		@ point at the last arg
		bleq	.Ldumpstm		@ dump saved registers

1004:		ldr	r1, [sv_pc, #0]		@ if stmfd sp!, {..., fp, ip, lr, pc}
		ldr	r3, .Ldsi		@ instruction exists,
		teq	r3, r1, lsr #10
		subeq	r0, frame, #16
		bleq	.Ldumpstm		@ dump saved registers

		teq	sv_fp, #0		@ zero saved fp means
		beq	no_frame		@ no further frames

		cmp	sv_fp, frame		@ next frame must be
		mov	frame, sv_fp		@ above the current frame
		bhi	for_each_frame

1006:		adr	r0, .Lbad
		mov	r1, frame
		bl	printk
no_frame:	ldmfd	sp!, {r4 - r8, pc}
ENDPROC(__backtrace)
ENDPROC(c_backtrace)
		
		.section __ex_table,"a"
		.align	3
		.long	1001b, 1006b
		.long	1002b, 1006b
		.long	1003b, 1006b
		.long	1004b, 1006b
		.previous

#define instr r4
#define reg   r5
#define stack r6

.Ldumpstm:	stmfd	sp!, {instr, reg, stack, r7, lr}
		mov	stack, r0
		mov	instr, r1
		mov	reg, #10
		mov	r7, #0
1:		mov	r3, #1
		tst	instr, r3, lsl reg
		beq	2f
		add	r7, r7, #1
		teq	r7, #6
		moveq	r7, #1
		moveq	r1, #'\n'
		movne	r1, #' '
		ldr	r3, [stack], #-4
		mov	r2, reg
		adr	r0, .Lfp
		bl	printk
2:		subs	reg, reg, #1
		bpl	1b
		teq	r7, #0
		adrne	r0, .Lcr
		blne	printk
		ldmfd	sp!, {instr, reg, stack, r7, pc}

.Lfp:		.asciz	"%cr%d:%08x"
.Lcr:		.asciz	"\n"
.Lbad:		.asciz	"Backtrace aborted due to bad frame pointer <%p>\n"
		.align
.Ldsi:		.word	0xe92dd800 >> 10	@ stmfd sp!, {... fp, ip, lr, pc}
		.word	0xe92d0000 >> 10	@ stmfd sp!, {}

#endif
