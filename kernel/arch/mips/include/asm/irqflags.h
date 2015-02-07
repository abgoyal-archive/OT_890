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
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 *
 * Copyright (C) 1994, 95, 96, 97, 98, 99, 2003 by Ralf Baechle
 * Copyright (C) 1996 by Paul M. Antoine
 * Copyright (C) 1999 Silicon Graphics
 * Copyright (C) 2000 MIPS Technologies, Inc.
 */
#ifndef _ASM_IRQFLAGS_H
#define _ASM_IRQFLAGS_H

#ifndef __ASSEMBLY__

#include <linux/compiler.h>
#include <asm/hazards.h>

__asm__(
	"	.macro	raw_local_irq_enable				\n"
	"	.set	push						\n"
	"	.set	reorder						\n"
	"	.set	noat						\n"
#ifdef CONFIG_MIPS_MT_SMTC
	"	mfc0	$1, $2, 1	# SMTC - clear TCStatus.IXMT	\n"
	"	ori	$1, 0x400					\n"
	"	xori	$1, 0x400					\n"
	"	mtc0	$1, $2, 1					\n"
#elif defined(CONFIG_CPU_MIPSR2)
	"	ei							\n"
#else
	"	mfc0	$1,$12						\n"
	"	ori	$1,0x1f						\n"
	"	xori	$1,0x1e						\n"
	"	mtc0	$1,$12						\n"
#endif
	"	irq_enable_hazard					\n"
	"	.set	pop						\n"
	"	.endm");

extern void smtc_ipi_replay(void);

static inline void raw_local_irq_enable(void)
{
#ifdef CONFIG_MIPS_MT_SMTC
	/*
	 * SMTC kernel needs to do a software replay of queued
	 * IPIs, at the cost of call overhead on each local_irq_enable()
	 */
	smtc_ipi_replay();
#endif
	__asm__ __volatile__(
		"raw_local_irq_enable"
		: /* no outputs */
		: /* no inputs */
		: "memory");
}


/*
 * For cli() we have to insert nops to make sure that the new value
 * has actually arrived in the status register before the end of this
 * macro.
 * R4000/R4400 need three nops, the R4600 two nops and the R10000 needs
 * no nops at all.
 */
/*
 * For TX49, operating only IE bit is not enough.
 *
 * If mfc0 $12 follows store and the mfc0 is last instruction of a
 * page and fetching the next instruction causes TLB miss, the result
 * of the mfc0 might wrongly contain EXL bit.
 *
 * ERT-TX49H2-027, ERT-TX49H3-012, ERT-TX49HL3-006, ERT-TX49H4-008
 *
 * Workaround: mask EXL bit of the result or place a nop before mfc0.
 */
__asm__(
	"	.macro	raw_local_irq_disable\n"
	"	.set	push						\n"
	"	.set	noat						\n"
#ifdef CONFIG_MIPS_MT_SMTC
	"	mfc0	$1, $2, 1					\n"
	"	ori	$1, 0x400					\n"
	"	.set	noreorder					\n"
	"	mtc0	$1, $2, 1					\n"
#elif defined(CONFIG_CPU_MIPSR2)
	"	di							\n"
#else
	"	mfc0	$1,$12						\n"
	"	ori	$1,0x1f						\n"
	"	xori	$1,0x1f						\n"
	"	.set	noreorder					\n"
	"	mtc0	$1,$12						\n"
#endif
	"	irq_disable_hazard					\n"
	"	.set	pop						\n"
	"	.endm							\n");

static inline void raw_local_irq_disable(void)
{
	__asm__ __volatile__(
		"raw_local_irq_disable"
		: /* no outputs */
		: /* no inputs */
		: "memory");
}

__asm__(
	"	.macro	raw_local_save_flags flags			\n"
	"	.set	push						\n"
	"	.set	reorder						\n"
#ifdef CONFIG_MIPS_MT_SMTC
	"	mfc0	\\flags, $2, 1					\n"
#else
	"	mfc0	\\flags, $12					\n"
#endif
	"	.set	pop						\n"
	"	.endm							\n");

#define raw_local_save_flags(x)						\
__asm__ __volatile__(							\
	"raw_local_save_flags %0"					\
	: "=r" (x))

__asm__(
	"	.macro	raw_local_irq_save result			\n"
	"	.set	push						\n"
	"	.set	reorder						\n"
	"	.set	noat						\n"
#ifdef CONFIG_MIPS_MT_SMTC
	"	mfc0	\\result, $2, 1					\n"
	"	ori	$1, \\result, 0x400				\n"
	"	.set	noreorder					\n"
	"	mtc0	$1, $2, 1					\n"
	"	andi	\\result, \\result, 0x400			\n"
#elif defined(CONFIG_CPU_MIPSR2)
	"	di	\\result					\n"
	"	andi	\\result, 1					\n"
#else
	"	mfc0	\\result, $12					\n"
	"	ori	$1, \\result, 0x1f				\n"
	"	xori	$1, 0x1f					\n"
	"	.set	noreorder					\n"
	"	mtc0	$1, $12						\n"
#endif
	"	irq_disable_hazard					\n"
	"	.set	pop						\n"
	"	.endm							\n");

#define raw_local_irq_save(x)						\
__asm__ __volatile__(							\
	"raw_local_irq_save\t%0"					\
	: "=r" (x)							\
	: /* no inputs */						\
	: "memory")

__asm__(
	"	.macro	raw_local_irq_restore flags			\n"
	"	.set	push						\n"
	"	.set	noreorder					\n"
	"	.set	noat						\n"
#ifdef CONFIG_MIPS_MT_SMTC
	"mfc0	$1, $2, 1						\n"
	"andi	\\flags, 0x400						\n"
	"ori	$1, 0x400						\n"
	"xori	$1, 0x400						\n"
	"or	\\flags, $1						\n"
	"mtc0	\\flags, $2, 1						\n"
#elif defined(CONFIG_CPU_MIPSR2) && defined(CONFIG_IRQ_CPU)
	/*
	 * Slow, but doesn't suffer from a relativly unlikely race
	 * condition we're having since days 1.
	 */
	"	beqz	\\flags, 1f					\n"
	"	 di							\n"
	"	ei							\n"
	"1:								\n"
#elif defined(CONFIG_CPU_MIPSR2)
	/*
	 * Fast, dangerous.  Life is fun, life is good.
	 */
	"	mfc0	$1, $12						\n"
	"	ins	$1, \\flags, 0, 1				\n"
	"	mtc0	$1, $12						\n"
#else
	"	mfc0	$1, $12						\n"
	"	andi	\\flags, 1					\n"
	"	ori	$1, 0x1f					\n"
	"	xori	$1, 0x1f					\n"
	"	or	\\flags, $1					\n"
	"	mtc0	\\flags, $12					\n"
#endif
	"	irq_disable_hazard					\n"
	"	.set	pop						\n"
	"	.endm							\n");


static inline void raw_local_irq_restore(unsigned long flags)
{
	unsigned long __tmp1;

#ifdef CONFIG_MIPS_MT_SMTC
	/*
	 * SMTC kernel needs to do a software replay of queued
	 * IPIs, at the cost of branch and call overhead on each
	 * local_irq_restore()
	 */
	if (unlikely(!(flags & 0x0400)))
		smtc_ipi_replay();
#endif

	__asm__ __volatile__(
		"raw_local_irq_restore\t%0"
		: "=r" (__tmp1)
		: "0" (flags)
		: "memory");
}

static inline void __raw_local_irq_restore(unsigned long flags)
{
	unsigned long __tmp1;

	__asm__ __volatile__(
		"raw_local_irq_restore\t%0"
		: "=r" (__tmp1)
		: "0" (flags)
		: "memory");
}

static inline int raw_irqs_disabled_flags(unsigned long flags)
{
#ifdef CONFIG_MIPS_MT_SMTC
	/*
	 * SMTC model uses TCStatus.IXMT to disable interrupts for a thread/CPU
	 */
	return flags & 0x400;
#else
	return !(flags & 1);
#endif
}

#endif

/*
 * Do the CPU's IRQ-state tracing from assembly code.
 */
#ifdef CONFIG_TRACE_IRQFLAGS
/* Reload some registers clobbered by trace_hardirqs_on */
#ifdef CONFIG_64BIT
# define TRACE_IRQS_RELOAD_REGS						\
	LONG_L	$11, PT_R11(sp);					\
	LONG_L	$10, PT_R10(sp);					\
	LONG_L	$9, PT_R9(sp);						\
	LONG_L	$8, PT_R8(sp);						\
	LONG_L	$7, PT_R7(sp);						\
	LONG_L	$6, PT_R6(sp);						\
	LONG_L	$5, PT_R5(sp);						\
	LONG_L	$4, PT_R4(sp);						\
	LONG_L	$2, PT_R2(sp)
#else
# define TRACE_IRQS_RELOAD_REGS						\
	LONG_L	$7, PT_R7(sp);						\
	LONG_L	$6, PT_R6(sp);						\
	LONG_L	$5, PT_R5(sp);						\
	LONG_L	$4, PT_R4(sp);						\
	LONG_L	$2, PT_R2(sp)
#endif
# define TRACE_IRQS_ON							\
	CLI;	/* make sure trace_hardirqs_on() is called in kernel level */ \
	jal	trace_hardirqs_on
# define TRACE_IRQS_ON_RELOAD						\
	TRACE_IRQS_ON;							\
	TRACE_IRQS_RELOAD_REGS
# define TRACE_IRQS_OFF							\
	jal	trace_hardirqs_off
#else
# define TRACE_IRQS_ON
# define TRACE_IRQS_ON_RELOAD
# define TRACE_IRQS_OFF
#endif

#endif /* _ASM_IRQFLAGS_H */
