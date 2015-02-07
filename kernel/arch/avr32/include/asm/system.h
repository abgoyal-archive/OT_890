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
 * Copyright (C) 2004-2006 Atmel Corporation
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */
#ifndef __ASM_AVR32_SYSTEM_H
#define __ASM_AVR32_SYSTEM_H

#include <linux/compiler.h>
#include <linux/linkage.h>
#include <linux/types.h>

#include <asm/ptrace.h>
#include <asm/sysreg.h>

#define xchg(ptr,x) \
	((__typeof__(*(ptr)))__xchg((unsigned long)(x),(ptr),sizeof(*(ptr))))

#define nop() asm volatile("nop")

#define mb()			asm volatile("" : : : "memory")
#define rmb()			mb()
#define wmb()			asm volatile("sync 0" : : : "memory")
#define read_barrier_depends()  do { } while(0)
#define set_mb(var, value)      do { var = value; mb(); } while(0)

/*
 * Help PathFinder and other Nexus-compliant debuggers keep track of
 * the current PID by emitting an Ownership Trace Message each time we
 * switch task.
 */
#ifdef CONFIG_OWNERSHIP_TRACE
#include <asm/ocd.h>
#define finish_arch_switch(prev)			\
	do {						\
		ocd_write(PID, prev->pid);		\
		ocd_write(PID, current->pid);		\
	} while(0)
#endif

/*
 * switch_to(prev, next, last) should switch from task `prev' to task
 * `next'. `prev' will never be the same as `next'.
 *
 * We just delegate everything to the __switch_to assembly function,
 * which is implemented in arch/avr32/kernel/switch_to.S
 *
 * mb() tells GCC not to cache `current' across this call.
 */
struct cpu_context;
struct task_struct;
extern struct task_struct *__switch_to(struct task_struct *,
				       struct cpu_context *,
				       struct cpu_context *);
#define switch_to(prev, next, last)					\
	do {								\
		last = __switch_to(prev, &prev->thread.cpu_context + 1,	\
				   &next->thread.cpu_context);		\
	} while (0)

#ifdef CONFIG_SMP
# error "The AVR32 port does not support SMP"
#else
# define smp_mb()		barrier()
# define smp_rmb()		barrier()
# define smp_wmb()		barrier()
# define smp_read_barrier_depends() do { } while(0)
#endif

#include <linux/irqflags.h>

extern void __xchg_called_with_bad_pointer(void);

static inline unsigned long xchg_u32(u32 val, volatile u32 *m)
{
	u32 ret;

	asm volatile("xchg %[ret], %[m], %[val]"
			: [ret] "=&r"(ret), "=m"(*m)
			: "m"(*m), [m] "r"(m), [val] "r"(val)
			: "memory");
	return ret;
}

static inline unsigned long __xchg(unsigned long x,
				       volatile void *ptr,
				       int size)
{
	switch(size) {
	case 4:
		return xchg_u32(x, ptr);
	default:
		__xchg_called_with_bad_pointer();
		return x;
	}
}

static inline unsigned long __cmpxchg_u32(volatile int *m, unsigned long old,
					  unsigned long new)
{
	__u32 ret;

	asm volatile(
		"1:	ssrf	5\n"
		"	ld.w	%[ret], %[m]\n"
		"	cp.w	%[ret], %[old]\n"
		"	brne	2f\n"
		"	stcond	%[m], %[new]\n"
		"	brne	1b\n"
		"2:\n"
		: [ret] "=&r"(ret), [m] "=m"(*m)
		: "m"(m), [old] "ir"(old), [new] "r"(new)
		: "memory", "cc");
	return ret;
}

extern unsigned long __cmpxchg_u64_unsupported_on_32bit_kernels(
        volatile int * m, unsigned long old, unsigned long new);
#define __cmpxchg_u64 __cmpxchg_u64_unsupported_on_32bit_kernels

/* This function doesn't exist, so you'll get a linker error
   if something tries to do an invalid cmpxchg().  */
extern void __cmpxchg_called_with_bad_pointer(void);

#define __HAVE_ARCH_CMPXCHG 1

static inline unsigned long __cmpxchg(volatile void *ptr, unsigned long old,
				      unsigned long new, int size)
{
	switch (size) {
	case 4:
		return __cmpxchg_u32(ptr, old, new);
	case 8:
		return __cmpxchg_u64(ptr, old, new);
	}

	__cmpxchg_called_with_bad_pointer();
	return old;
}

#define cmpxchg(ptr, old, new)					\
	((typeof(*(ptr)))__cmpxchg((ptr), (unsigned long)(old),	\
				   (unsigned long)(new),	\
				   sizeof(*(ptr))))

#include <asm-generic/cmpxchg-local.h>

static inline unsigned long __cmpxchg_local(volatile void *ptr,
				      unsigned long old,
				      unsigned long new, int size)
{
	switch (size) {
	case 4:
		return __cmpxchg_u32(ptr, old, new);
	default:
		return __cmpxchg_local_generic(ptr, old, new, size);
	}

	return old;
}

#define cmpxchg_local(ptr, old, new)					\
	((typeof(*(ptr)))__cmpxchg_local((ptr), (unsigned long)(old),	\
				   (unsigned long)(new),		\
				   sizeof(*(ptr))))

#define cmpxchg64_local(ptr, o, n) __cmpxchg64_local_generic((ptr), (o), (n))

struct pt_regs;
void NORET_TYPE die(const char *str, struct pt_regs *regs, long err);
void _exception(long signr, struct pt_regs *regs, int code,
		unsigned long addr);

#define arch_align_stack(x)	(x)

#endif /* __ASM_AVR32_SYSTEM_H */
