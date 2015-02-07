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
 * ring buffer based initcalls tracer
 *
 * Copyright (C) 2008 Frederic Weisbecker <fweisbec@gmail.com>
 *
 */

#include <linux/init.h>
#include <linux/debugfs.h>
#include <linux/ftrace.h>
#include <linux/kallsyms.h>

#include "trace.h"

static struct trace_array *boot_trace;
static bool pre_initcalls_finished;

/* Tells the boot tracer that the pre_smp_initcalls are finished.
 * So we are ready .
 * It doesn't enable sched events tracing however.
 * You have to call enable_boot_trace to do so.
 */
void start_boot_trace(void)
{
	pre_initcalls_finished = true;
}

void enable_boot_trace(void)
{
	if (pre_initcalls_finished)
		tracing_start_sched_switch_record();
}

void disable_boot_trace(void)
{
	if (pre_initcalls_finished)
		tracing_stop_sched_switch_record();
}

static int boot_trace_init(struct trace_array *tr)
{
	int cpu;
	boot_trace = tr;

	for_each_cpu(cpu, cpu_possible_mask)
		tracing_reset(tr, cpu);

	tracing_sched_switch_assign_trace(tr);
	return 0;
}

static enum print_line_t
initcall_call_print_line(struct trace_iterator *iter)
{
	struct trace_entry *entry = iter->ent;
	struct trace_seq *s = &iter->seq;
	struct trace_boot_call *field;
	struct boot_trace_call *call;
	u64 ts;
	unsigned long nsec_rem;
	int ret;

	trace_assign_type(field, entry);
	call = &field->boot_call;
	ts = iter->ts;
	nsec_rem = do_div(ts, 1000000000);

	ret = trace_seq_printf(s, "[%5ld.%09ld] calling  %s @ %i\n",
			(unsigned long)ts, nsec_rem, call->func, call->caller);

	if (!ret)
		return TRACE_TYPE_PARTIAL_LINE;
	else
		return TRACE_TYPE_HANDLED;
}

static enum print_line_t
initcall_ret_print_line(struct trace_iterator *iter)
{
	struct trace_entry *entry = iter->ent;
	struct trace_seq *s = &iter->seq;
	struct trace_boot_ret *field;
	struct boot_trace_ret *init_ret;
	u64 ts;
	unsigned long nsec_rem;
	int ret;

	trace_assign_type(field, entry);
	init_ret = &field->boot_ret;
	ts = iter->ts;
	nsec_rem = do_div(ts, 1000000000);

	ret = trace_seq_printf(s, "[%5ld.%09ld] initcall %s "
			"returned %d after %llu msecs\n",
			(unsigned long) ts,
			nsec_rem,
			init_ret->func, init_ret->result, init_ret->duration);

	if (!ret)
		return TRACE_TYPE_PARTIAL_LINE;
	else
		return TRACE_TYPE_HANDLED;
}

static enum print_line_t initcall_print_line(struct trace_iterator *iter)
{
	struct trace_entry *entry = iter->ent;

	switch (entry->type) {
	case TRACE_BOOT_CALL:
		return initcall_call_print_line(iter);
	case TRACE_BOOT_RET:
		return initcall_ret_print_line(iter);
	default:
		return TRACE_TYPE_UNHANDLED;
	}
}

struct tracer boot_tracer __read_mostly =
{
	.name		= "initcall",
	.init		= boot_trace_init,
	.reset		= tracing_reset_online_cpus,
	.print_line	= initcall_print_line,
};

void trace_boot_call(struct boot_trace_call *bt, initcall_t fn)
{
	struct ring_buffer_event *event;
	struct trace_boot_call *entry;
	unsigned long irq_flags;
	struct trace_array *tr = boot_trace;

	if (!pre_initcalls_finished)
		return;

	/* Get its name now since this function could
	 * disappear because it is in the .init section.
	 */
	sprint_symbol(bt->func, (unsigned long)fn);
	preempt_disable();

	event = ring_buffer_lock_reserve(tr->buffer, sizeof(*entry),
					 &irq_flags);
	if (!event)
		goto out;
	entry	= ring_buffer_event_data(event);
	tracing_generic_entry_update(&entry->ent, 0, 0);
	entry->ent.type = TRACE_BOOT_CALL;
	entry->boot_call = *bt;
	ring_buffer_unlock_commit(tr->buffer, event, irq_flags);

	trace_wake_up();

 out:
	preempt_enable();
}

void trace_boot_ret(struct boot_trace_ret *bt, initcall_t fn)
{
	struct ring_buffer_event *event;
	struct trace_boot_ret *entry;
	unsigned long irq_flags;
	struct trace_array *tr = boot_trace;

	if (!pre_initcalls_finished)
		return;

	sprint_symbol(bt->func, (unsigned long)fn);
	preempt_disable();

	event = ring_buffer_lock_reserve(tr->buffer, sizeof(*entry),
					 &irq_flags);
	if (!event)
		goto out;
	entry	= ring_buffer_event_data(event);
	tracing_generic_entry_update(&entry->ent, 0, 0);
	entry->ent.type = TRACE_BOOT_RET;
	entry->boot_ret = *bt;
	ring_buffer_unlock_commit(tr->buffer, event, irq_flags);

	trace_wake_up();

 out:
	preempt_enable();
}