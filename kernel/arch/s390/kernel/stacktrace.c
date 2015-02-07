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
 * arch/s390/kernel/stacktrace.c
 *
 * Stack trace management functions
 *
 *  Copyright (C) IBM Corp. 2006
 *  Author(s): Heiko Carstens <heiko.carstens@de.ibm.com>
 */

#include <linux/sched.h>
#include <linux/stacktrace.h>
#include <linux/kallsyms.h>
#include <linux/module.h>

static unsigned long save_context_stack(struct stack_trace *trace,
					unsigned long sp,
					unsigned long low,
					unsigned long high,
					int savesched)
{
	struct stack_frame *sf;
	struct pt_regs *regs;
	unsigned long addr;

	while(1) {
		sp &= PSW_ADDR_INSN;
		if (sp < low || sp > high)
			return sp;
		sf = (struct stack_frame *)sp;
		while(1) {
			addr = sf->gprs[8] & PSW_ADDR_INSN;
			if (!trace->skip)
				trace->entries[trace->nr_entries++] = addr;
			else
				trace->skip--;
			if (trace->nr_entries >= trace->max_entries)
				return sp;
			low = sp;
			sp = sf->back_chain & PSW_ADDR_INSN;
			if (!sp)
				break;
			if (sp <= low || sp > high - sizeof(*sf))
				return sp;
			sf = (struct stack_frame *)sp;
		}
		/* Zero backchain detected, check for interrupt frame. */
		sp = (unsigned long)(sf + 1);
		if (sp <= low || sp > high - sizeof(*regs))
			return sp;
		regs = (struct pt_regs *)sp;
		addr = regs->psw.addr & PSW_ADDR_INSN;
		if (savesched || !in_sched_functions(addr)) {
			if (!trace->skip)
				trace->entries[trace->nr_entries++] = addr;
			else
				trace->skip--;
		}
		if (trace->nr_entries >= trace->max_entries)
			return sp;
		low = sp;
		sp = regs->gprs[15];
	}
}

void save_stack_trace(struct stack_trace *trace)
{
	register unsigned long sp asm ("15");
	unsigned long orig_sp, new_sp;

	orig_sp = sp & PSW_ADDR_INSN;
	new_sp = save_context_stack(trace, orig_sp,
				    S390_lowcore.panic_stack - PAGE_SIZE,
				    S390_lowcore.panic_stack, 1);
	if (new_sp != orig_sp)
		return;
	new_sp = save_context_stack(trace, new_sp,
				    S390_lowcore.async_stack - ASYNC_SIZE,
				    S390_lowcore.async_stack, 1);
	if (new_sp != orig_sp)
		return;
	save_context_stack(trace, new_sp,
			   S390_lowcore.thread_info,
			   S390_lowcore.thread_info + THREAD_SIZE, 1);
}
EXPORT_SYMBOL_GPL(save_stack_trace);

void save_stack_trace_tsk(struct task_struct *tsk, struct stack_trace *trace)
{
	unsigned long sp, low, high;

	sp = tsk->thread.ksp & PSW_ADDR_INSN;
	low = (unsigned long) task_stack_page(tsk);
	high = (unsigned long) task_pt_regs(tsk);
	save_context_stack(trace, sp, low, high, 0);
	if (trace->nr_entries < trace->max_entries)
		trace->entries[trace->nr_entries++] = ULONG_MAX;
}
EXPORT_SYMBOL_GPL(save_stack_trace_tsk);
