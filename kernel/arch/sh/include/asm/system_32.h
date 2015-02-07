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

#ifndef __ASM_SH_SYSTEM_32_H
#define __ASM_SH_SYSTEM_32_H

#include <linux/types.h>

struct task_struct *__switch_to(struct task_struct *prev,
				struct task_struct *next);

/*
 *	switch_to() should switch tasks to task nr n, first
 */
#define switch_to(prev, next, last)					\
do {									\
	register u32 *__ts1 __asm__ ("r1") = (u32 *)&prev->thread.sp;	\
	register u32 *__ts2 __asm__ ("r2") = (u32 *)&prev->thread.pc;	\
	register u32 *__ts4 __asm__ ("r4") = (u32 *)prev;		\
	register u32 *__ts5 __asm__ ("r5") = (u32 *)next;		\
	register u32 *__ts6 __asm__ ("r6") = (u32 *)&next->thread.sp;	\
	register u32 __ts7 __asm__ ("r7") = next->thread.pc;		\
	struct task_struct *__last;					\
									\
	__asm__ __volatile__ (						\
		".balign 4\n\t"						\
		"stc.l	gbr, @-r15\n\t"					\
		"sts.l	pr, @-r15\n\t"					\
		"mov.l	r8, @-r15\n\t"					\
		"mov.l	r9, @-r15\n\t"					\
		"mov.l	r10, @-r15\n\t"					\
		"mov.l	r11, @-r15\n\t"					\
		"mov.l	r12, @-r15\n\t"					\
		"mov.l	r13, @-r15\n\t"					\
		"mov.l	r14, @-r15\n\t"					\
		"mov.l	r15, @r1\t! save SP\n\t"			\
		"mov.l	@r6, r15\t! change to new stack\n\t"		\
		"mova	1f, %0\n\t"					\
		"mov.l	%0, @r2\t! save PC\n\t"				\
		"mov.l	2f, %0\n\t"					\
		"jmp	@%0\t! call __switch_to\n\t"			\
		" lds	r7, pr\t!  with return to new PC\n\t"		\
		".balign	4\n"					\
		"2:\n\t"						\
		".long	__switch_to\n"					\
		"1:\n\t"						\
		"mov.l	@r15+, r14\n\t"					\
		"mov.l	@r15+, r13\n\t"					\
		"mov.l	@r15+, r12\n\t"					\
		"mov.l	@r15+, r11\n\t"					\
		"mov.l	@r15+, r10\n\t"					\
		"mov.l	@r15+, r9\n\t"					\
		"mov.l	@r15+, r8\n\t"					\
		"lds.l	@r15+, pr\n\t"					\
		"ldc.l	@r15+, gbr\n\t"					\
		: "=z" (__last)						\
		: "r" (__ts1), "r" (__ts2), "r" (__ts4),		\
		  "r" (__ts5), "r" (__ts6), "r" (__ts7)			\
		: "r3", "t");						\
									\
	last = __last;							\
} while (0)

#define __uses_jump_to_uncached \
	noinline __attribute__ ((__section__ (".uncached.text")))

/*
 * Jump to uncached area.
 * When handling TLB or caches, we need to do it from an uncached area.
 */
#define jump_to_uncached()			\
do {						\
	unsigned long __dummy;			\
						\
	__asm__ __volatile__(			\
		"mova	1f, %0\n\t"		\
		"add	%1, %0\n\t"		\
		"jmp	@%0\n\t"		\
		" nop\n\t"			\
		".balign 4\n"			\
		"1:"				\
		: "=&z" (__dummy)		\
		: "r" (cached_to_uncached));	\
} while (0)

/*
 * Back to cached area.
 */
#define back_to_cached()				\
do {							\
	unsigned long __dummy;				\
	ctrl_barrier();					\
	__asm__ __volatile__(				\
		"mov.l	1f, %0\n\t"			\
		"jmp	@%0\n\t"			\
		" nop\n\t"				\
		".balign 4\n"				\
		"1:	.long 2f\n"			\
		"2:"					\
		: "=&r" (__dummy));			\
} while (0)

#ifdef CONFIG_CPU_HAS_SR_RB
#define lookup_exception_vector()	\
({					\
	unsigned long _vec;		\
					\
	__asm__ __volatile__ (		\
		"stc r2_bank, %0\n\t"	\
		: "=r" (_vec)		\
	);				\
					\
	_vec;				\
})
#else
#define lookup_exception_vector()	\
({					\
	unsigned long _vec;		\
	__asm__ __volatile__ (		\
		"mov r4, %0\n\t"	\
		: "=r" (_vec)		\
	);				\
					\
	_vec;				\
})
#endif

int handle_unaligned_access(opcode_t instruction, struct pt_regs *regs,
			    struct mem_access *ma);

asmlinkage void do_address_error(struct pt_regs *regs,
				 unsigned long writeaccess,
				 unsigned long address);
asmlinkage void do_divide_error(unsigned long r4, unsigned long r5,
				unsigned long r6, unsigned long r7,
				struct pt_regs __regs);
asmlinkage void do_reserved_inst(unsigned long r4, unsigned long r5,
				unsigned long r6, unsigned long r7,
				struct pt_regs __regs);
asmlinkage void do_illegal_slot_inst(unsigned long r4, unsigned long r5,
				unsigned long r6, unsigned long r7,
				struct pt_regs __regs);
asmlinkage void do_exception_error(unsigned long r4, unsigned long r5,
				   unsigned long r6, unsigned long r7,
				   struct pt_regs __regs);

#endif /* __ASM_SH_SYSTEM_32_H */
