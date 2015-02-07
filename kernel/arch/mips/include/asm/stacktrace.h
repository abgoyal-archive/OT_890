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

#ifndef _ASM_STACKTRACE_H
#define _ASM_STACKTRACE_H

#include <asm/ptrace.h>

#ifdef CONFIG_KALLSYMS
extern int raw_show_trace;
extern unsigned long unwind_stack(struct task_struct *task, unsigned long *sp,
				  unsigned long pc, unsigned long *ra);
#else
#define raw_show_trace 1
static inline unsigned long unwind_stack(struct task_struct *task,
	unsigned long *sp, unsigned long pc, unsigned long *ra)
{
	return 0;
}
#endif

static __always_inline void prepare_frametrace(struct pt_regs *regs)
{
#ifndef CONFIG_KALLSYMS
	/*
	 * Remove any garbage that may be in regs (specially func
	 * addresses) to avoid show_raw_backtrace() to report them
	 */
	memset(regs, 0, sizeof(*regs));
#endif
	__asm__ __volatile__(
		".set push\n\t"
		".set noat\n\t"
#ifdef CONFIG_64BIT
		"1: dla $1, 1b\n\t"
		"sd $1, %0\n\t"
		"sd $29, %1\n\t"
		"sd $31, %2\n\t"
#else
		"1: la $1, 1b\n\t"
		"sw $1, %0\n\t"
		"sw $29, %1\n\t"
		"sw $31, %2\n\t"
#endif
		".set pop\n\t"
		: "=m" (regs->cp0_epc),
		"=m" (regs->regs[29]), "=m" (regs->regs[31])
		: : "memory");
}

#endif /* _ASM_STACKTRACE_H */
