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

#ifndef _ASM_M32R_ASSEMBLER_H
#define _ASM_M32R_ASSEMBLER_H

/*
 * linux/asm-m32r/assembler.h
 *
 * Copyright (C) 2004  Hirokazu Takata <takata at linux-m32r.org>
 *
 * This file contains M32R architecture specific macro definitions.
 */


#ifndef __STR
#ifdef __ASSEMBLY__
#define __STR(x) x
#else
#define __STR(x) #x
#endif
#endif /* __STR */

#ifdef CONFIG_SMP
#define M32R_LOCK	__STR(lock)
#define M32R_UNLOCK	__STR(unlock)
#else
#define M32R_LOCK	__STR(ld)
#define M32R_UNLOCK	__STR(st)
#endif

#ifdef __ASSEMBLY__
#undef ENTRY
#define ENTRY(name) ENTRY_M name
	.macro  ENTRY_M name
	.global \name
	ALIGN
\name:
	.endm
#endif


/**
 * LDIMM - load immediate value
 * STI - enable interruption
 * CLI - disable interruption
 */

#ifdef __ASSEMBLY__

#define LDIMM(reg,x) LDIMM reg x
	.macro LDIMM reg x
	seth	\reg, #high(\x)
	or3	\reg, \reg, #low(\x)
	.endm

#if !(defined(CONFIG_CHIP_M32102) || defined(CONFIG_CHIP_M32104))
#define ENABLE_INTERRUPTS(reg) ENABLE_INTERRUPTS reg
	.macro ENABLE_INTERRUPTS reg
	setpsw  #0x40	    ->	nop
	; WORKAROUND: "-> nop" is a workaround for the M32700(TS1).
	.endm

#define DISABLE_INTERRUPTS(reg) DISABLE_INTERRUPTS reg
	.macro DISABLE_INTERRUPTS reg
	clrpsw  #0x40	    ->	nop
	; WORKAROUND: "-> nop" is a workaround for the M32700(TS1).
	.endm
#else	/* CONFIG_CHIP_M32102 || CONFIG_CHIP_M32104 */
#define ENABLE_INTERRUPTS(reg) ENABLE_INTERRUPTS reg
	.macro ENABLE_INTERRUPTS reg
	mvfc	\reg, psw
	or3	\reg, \reg, #0x0040
	mvtc	\reg, psw
	.endm

#define DISABLE_INTERRUPTS(reg) DISABLE_INTERRUPTS reg
	.macro DISABLE_INTERRUPTS reg
	mvfc	\reg, psw
	and3	\reg, \reg, #0xffbf
	mvtc	\reg, psw
	.endm
#endif	/* CONFIG_CHIP_M32102 */

	.macro	SAVE_ALL
	push	r0		; orig_r0
	push	sp		; spi (r15)
	push	lr		; r14
	push	r13
	mvfc	r13, cr3	; spu
	push	r13
	mvfc	r13, bbpc
	push	r13
	mvfc	r13, bbpsw
	push	r13
	mvfc	r13, bpc
	push	r13
	mvfc	r13, psw
	push	r13
#if defined(CONFIG_ISA_M32R2) && defined(CONFIG_ISA_DSP_LEVEL2)
	mvfaclo	r13, a1
	push	r13
	mvfachi r13, a1
	push	r13
	mvfaclo	r13, a0
	push	r13
	mvfachi	r13, a0
	push	r13
#elif defined(CONFIG_ISA_M32R2) || defined(CONFIG_ISA_M32R)
	mvfaclo	r13
	push	r13
	mvfachi	r13
	push	r13
	ldi	r13, #0
	push	r13		; dummy push acc1h
	push	r13		; dummy push acc1l
#else
#error unknown isa configuration
#endif
	ldi	r13, #-1
	push	r13		; syscall_nr (default: -1)
	push	r12
	push	r11
	push	r10
	push	r9
	push	r8
	push	r7
	push	r3
	push	r2
	push	r1
	push	r0
	addi	sp, #-4		; room for implicit pt_regs parameter
	push	r6
	push	r5
	push	r4
	.endm

	.macro	RESTORE_ALL
	pop	r4
	pop	r5
	pop	r6
	addi	sp, #4
	pop	r0
	pop	r1
	pop	r2
	pop	r3
	pop	r7
	pop	r8
	pop	r9
	pop	r10
	pop	r11
	pop	r12
	addi	r15, #4		; Skip syscall number
#if defined(CONFIG_ISA_M32R2) && defined(CONFIG_ISA_DSP_LEVEL2)
	pop	r13
	mvtachi	r13, a0
	pop	r13
	mvtaclo	r13, a0
	pop	r13
	mvtachi	r13, a1
	pop	r13
	mvtaclo	r13, a1
#elif defined(CONFIG_ISA_M32R2) || defined(CONFIG_ISA_M32R)
	pop	r13		; dummy pop acc1h
	pop	r13		; dummy pop acc1l
	pop	r13
	mvtachi	r13
	pop	r13
	mvtaclo	r13
#else
#error unknown isa configuration
#endif
	pop	r14
	mvtc	r14, psw
	pop	r14
	mvtc	r14, bpc
	addi	sp, #8		; Skip bbpsw, bbpc
	pop	r14
	mvtc	r14, cr3	; spu
	pop	r13
	pop	lr		; r14
	pop	sp		; spi (r15)
	addi	sp, #4		; Skip orig_r0
	.fillinsn
1:	rte
	.section .fixup,"ax"
2:	bl	do_exit
	.previous
	.section __ex_table,"a"
	ALIGN
	.long	1b, 2b
	.previous
	.endm

#define GET_CURRENT(reg)  get_current reg
	.macro get_current reg
	ldi  \reg, #-8192
	and  \reg, sp
	.endm

#if !(defined(CONFIG_CHIP_M32102) || defined(CONFIG_CHIP_M32104))
	.macro	SWITCH_TO_KERNEL_STACK
	; switch to kernel stack (spi)
	clrpsw	#0x80	    ->	nop
	.endm
#else	/* CONFIG_CHIP_M32102 || CONFIG_CHIP_M32104 */
	.macro	SWITCH_TO_KERNEL_STACK
	push	r0		; save r0 for working
	mvfc	r0, psw
	and3	r0, r0, #0x00ff7f
	mvtc	r0, psw
	slli	r0, #16
	bltz	r0, 1f		; check BSM-bit
;
	;; called from kernel context: previous stack = spi
	pop	r0		; retrieve r0
	bra	2f
	.fillinsn
1:
	;; called from user context: previous stack = spu
	mvfc	r0, cr3		; spu
	addi	r0, #4
	mvtc	r0, cr3		; spu
	ld	r0, @(-4,r0)	; retrieve r0
	.fillinsn
2:
	.endm
#endif	/* CONFIG_CHIP_M32102 || CONFIG_CHIP_M32104 */

#endif	/* __ASSEMBLY__ */

#endif	/* _ASM_M32R_ASSEMBLER_H */
