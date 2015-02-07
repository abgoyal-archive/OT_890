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

#ifndef _LINUX_TRACE_BOOT_H
#define _LINUX_TRACE_BOOT_H

#include <linux/module.h>
#include <linux/kallsyms.h>
#include <linux/init.h>

/*
 * Structure which defines the trace of an initcall
 * while it is called.
 * You don't have to fill the func field since it is
 * only used internally by the tracer.
 */
struct boot_trace_call {
	pid_t			caller;
	char			func[KSYM_SYMBOL_LEN];
};

/*
 * Structure which defines the trace of an initcall
 * while it returns.
 */
struct boot_trace_ret {
	char			func[KSYM_SYMBOL_LEN];
	int				result;
	unsigned long long	duration;		/* nsecs */
};

#ifdef CONFIG_BOOT_TRACER
/* Append the traces on the ring-buffer */
extern void trace_boot_call(struct boot_trace_call *bt, initcall_t fn);
extern void trace_boot_ret(struct boot_trace_ret *bt, initcall_t fn);

/* Tells the tracer that smp_pre_initcall is finished.
 * So we can start the tracing
 */
extern void start_boot_trace(void);

/* Resume the tracing of other necessary events
 * such as sched switches
 */
extern void enable_boot_trace(void);

/* Suspend this tracing. Actually, only sched_switches tracing have
 * to be suspended. Initcalls doesn't need it.)
 */
extern void disable_boot_trace(void);
#else
static inline
void trace_boot_call(struct boot_trace_call *bt, initcall_t fn) { }

static inline
void trace_boot_ret(struct boot_trace_ret *bt, initcall_t fn) { }

static inline void start_boot_trace(void) { }
static inline void enable_boot_trace(void) { }
static inline void disable_boot_trace(void) { }
#endif /* CONFIG_BOOT_TRACER */

#endif /* __LINUX_TRACE_BOOT_H */
