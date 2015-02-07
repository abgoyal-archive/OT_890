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
 * arch/mips/kernel/stacktrace.c
 *
 * Stack trace management functions
 *
 *  Copyright (C) 2006 Atsushi Nemoto <anemo@mba.ocn.ne.jp>
 */
#include <linux/sched.h>
#include <linux/stacktrace.h>
#include <linux/module.h>
#include <asm/stacktrace.h>

/*
 * Save stack-backtrace addresses into a stack_trace buffer:
 */
static void save_raw_context_stack(struct stack_trace *trace,
	unsigned long reg29)
{
	unsigned long *sp = (unsigned long *)reg29;
	unsigned long addr;

	while (!kstack_end(sp)) {
		addr = *sp++;
		if (__kernel_text_address(addr)) {
			if (trace->skip > 0)
				trace->skip--;
			else
				trace->entries[trace->nr_entries++] = addr;
			if (trace->nr_entries >= trace->max_entries)
				break;
		}
	}
}

static void save_context_stack(struct stack_trace *trace,
	struct task_struct *tsk, struct pt_regs *regs)
{
	unsigned long sp = regs->regs[29];
#ifdef CONFIG_KALLSYMS
	unsigned long ra = regs->regs[31];
	unsigned long pc = regs->cp0_epc;

	if (raw_show_trace || !__kernel_text_address(pc)) {
		unsigned long stack_page =
			(unsigned long)task_stack_page(tsk);
		if (stack_page && sp >= stack_page &&
		    sp <= stack_page + THREAD_SIZE - 32)
			save_raw_context_stack(trace, sp);
		return;
	}
	do {
		if (trace->skip > 0)
			trace->skip--;
		else
			trace->entries[trace->nr_entries++] = pc;
		if (trace->nr_entries >= trace->max_entries)
			break;
		pc = unwind_stack(tsk, &sp, pc, &ra);
	} while (pc);
#else
	save_raw_context_stack(trace, sp);
#endif
}

/*
 * Save stack-backtrace addresses into a stack_trace buffer.
 */
void save_stack_trace(struct stack_trace *trace)
{
	save_stack_trace_tsk(current, trace);
}
EXPORT_SYMBOL_GPL(save_stack_trace);

void save_stack_trace_tsk(struct task_struct *tsk, struct stack_trace *trace)
{
	struct pt_regs dummyregs;
	struct pt_regs *regs = &dummyregs;

	WARN_ON(trace->nr_entries || !trace->max_entries);

	if (tsk != current) {
		regs->regs[29] = tsk->thread.reg29;
		regs->regs[31] = 0;
		regs->cp0_epc = tsk->thread.reg31;
	} else
		prepare_frametrace(regs);
	save_context_stack(trace, tsk, regs);
}
EXPORT_SYMBOL_GPL(save_stack_trace_tsk);
