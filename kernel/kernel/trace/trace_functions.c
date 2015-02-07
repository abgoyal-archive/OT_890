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
 * ring buffer based function tracer
 *
 * Copyright (C) 2007-2008 Steven Rostedt <srostedt@redhat.com>
 * Copyright (C) 2008 Ingo Molnar <mingo@redhat.com>
 *
 * Based on code from the latency_tracer, that is:
 *
 *  Copyright (C) 2004-2006 Ingo Molnar
 *  Copyright (C) 2004 William Lee Irwin III
 */
#include <linux/debugfs.h>
#include <linux/uaccess.h>
#include <linux/ftrace.h>
#include <linux/fs.h>

#include "trace.h"

static void start_function_trace(struct trace_array *tr)
{
	tr->cpu = get_cpu();
	tracing_reset_online_cpus(tr);
	put_cpu();

	tracing_start_cmdline_record();
	tracing_start_function_trace();
}

static void stop_function_trace(struct trace_array *tr)
{
	tracing_stop_function_trace();
	tracing_stop_cmdline_record();
}

static int function_trace_init(struct trace_array *tr)
{
	start_function_trace(tr);
	return 0;
}

static void function_trace_reset(struct trace_array *tr)
{
	stop_function_trace(tr);
}

static void function_trace_start(struct trace_array *tr)
{
	tracing_reset_online_cpus(tr);
}

static struct tracer function_trace __read_mostly =
{
	.name	     = "function",
	.init	     = function_trace_init,
	.reset	     = function_trace_reset,
	.start	     = function_trace_start,
#ifdef CONFIG_FTRACE_SELFTEST
	.selftest    = trace_selftest_startup_function,
#endif
};

static __init int init_function_trace(void)
{
	return register_tracer(&function_trace);
}

device_initcall(init_function_trace);
