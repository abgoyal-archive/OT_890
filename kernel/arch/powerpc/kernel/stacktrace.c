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
 * Stack trace utility
 *
 * Copyright 2008 Christoph Hellwig, IBM Corp.
 *
 *
 *      This program is free software; you can redistribute it and/or
 *      modify it under the terms of the GNU General Public License
 *      as published by the Free Software Foundation; either version
 *      2 of the License, or (at your option) any later version.
 */

#include <linux/module.h>
#include <linux/sched.h>
#include <linux/stacktrace.h>
#include <asm/ptrace.h>
#include <asm/processor.h>

/*
 * Save stack-backtrace addresses into a stack_trace buffer.
 */
static void save_context_stack(struct stack_trace *trace, unsigned long sp,
			struct task_struct *tsk, int savesched)
{
	for (;;) {
		unsigned long *stack = (unsigned long *) sp;
		unsigned long newsp, ip;

		if (!validate_sp(sp, tsk, STACK_FRAME_OVERHEAD))
			return;

		newsp = stack[0];
		ip = stack[STACK_FRAME_LR_SAVE];

		if (savesched || !in_sched_functions(ip)) {
			if (!trace->skip)
				trace->entries[trace->nr_entries++] = ip;
			else
				trace->skip--;
		}

		if (trace->nr_entries >= trace->max_entries)
			return;

		sp = newsp;
	}
}

void save_stack_trace(struct stack_trace *trace)
{
	unsigned long sp;

	asm("mr %0,1" : "=r" (sp));

	save_context_stack(trace, sp, current, 1);
}
EXPORT_SYMBOL_GPL(save_stack_trace);

void save_stack_trace_tsk(struct task_struct *tsk, struct stack_trace *trace)
{
	save_context_stack(trace, tsk->thread.ksp, tsk, 0);
}
EXPORT_SYMBOL_GPL(save_stack_trace_tsk);
