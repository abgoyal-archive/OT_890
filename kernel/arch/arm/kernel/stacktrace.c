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

#include <linux/module.h>
#include <linux/sched.h>
#include <linux/stacktrace.h>

#include "stacktrace.h"

int walk_stackframe(unsigned long fp, unsigned long low, unsigned long high,
		    int (*fn)(struct stackframe *, void *), void *data)
{
	struct stackframe *frame;

	do {
		/*
		 * Check current frame pointer is within bounds
		 */
		if (fp < (low + 12) || fp + 4 >= high)
			break;

		frame = (struct stackframe *)(fp - 12);

		if (fn(frame, data))
			break;

		/*
		 * Update the low bound - the next frame must always
		 * be at a higher address than the current frame.
		 */
		low = fp + 4;
		fp = frame->fp;
	} while (fp);

	return 0;
}
EXPORT_SYMBOL(walk_stackframe);

#ifdef CONFIG_STACKTRACE
struct stack_trace_data {
	struct stack_trace *trace;
	unsigned int no_sched_functions;
	unsigned int skip;
};

static int save_trace(struct stackframe *frame, void *d)
{
	struct stack_trace_data *data = d;
	struct stack_trace *trace = data->trace;
	unsigned long addr = frame->lr;

	if (data->no_sched_functions && in_sched_functions(addr))
		return 0;
	if (data->skip) {
		data->skip--;
		return 0;
	}

	trace->entries[trace->nr_entries++] = addr;

	return trace->nr_entries >= trace->max_entries;
}

void save_stack_trace_tsk(struct task_struct *tsk, struct stack_trace *trace)
{
	struct stack_trace_data data;
	unsigned long fp, base;

	data.trace = trace;
	data.skip = trace->skip;
	base = (unsigned long)task_stack_page(tsk);

	if (tsk != current) {
#ifdef CONFIG_SMP
		/*
		 * What guarantees do we have here that 'tsk'
		 * is not running on another CPU?
		 */
		BUG();
#else
		data.no_sched_functions = 1;
		fp = thread_saved_fp(tsk);
#endif
	} else {
		data.no_sched_functions = 0;
		asm("mov %0, fp" : "=r" (fp));
	}

	walk_stackframe(fp, base, base + THREAD_SIZE, save_trace, &data);
	if (trace->nr_entries < trace->max_entries)
		trace->entries[trace->nr_entries++] = ULONG_MAX;
}

void save_stack_trace(struct stack_trace *trace)
{
	save_stack_trace_tsk(current, trace);
}
EXPORT_SYMBOL_GPL(save_stack_trace);
#endif
