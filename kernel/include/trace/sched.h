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

#ifndef _TRACE_SCHED_H
#define _TRACE_SCHED_H

#include <linux/sched.h>
#include <linux/tracepoint.h>

DECLARE_TRACE(sched_kthread_stop,
	TPPROTO(struct task_struct *t),
		TPARGS(t));

DECLARE_TRACE(sched_kthread_stop_ret,
	TPPROTO(int ret),
		TPARGS(ret));

DECLARE_TRACE(sched_wait_task,
	TPPROTO(struct rq *rq, struct task_struct *p),
		TPARGS(rq, p));

DECLARE_TRACE(sched_wakeup,
	TPPROTO(struct rq *rq, struct task_struct *p, int success),
		TPARGS(rq, p, success));

DECLARE_TRACE(sched_wakeup_new,
	TPPROTO(struct rq *rq, struct task_struct *p, int success),
		TPARGS(rq, p, success));

DECLARE_TRACE(sched_switch,
	TPPROTO(struct rq *rq, struct task_struct *prev,
		struct task_struct *next),
		TPARGS(rq, prev, next));

DECLARE_TRACE(sched_migrate_task,
	TPPROTO(struct task_struct *p, int orig_cpu, int dest_cpu),
		TPARGS(p, orig_cpu, dest_cpu));

DECLARE_TRACE(sched_process_free,
	TPPROTO(struct task_struct *p),
		TPARGS(p));

DECLARE_TRACE(sched_process_exit,
	TPPROTO(struct task_struct *p),
		TPARGS(p));

DECLARE_TRACE(sched_process_wait,
	TPPROTO(struct pid *pid),
		TPARGS(pid));

DECLARE_TRACE(sched_process_fork,
	TPPROTO(struct task_struct *parent, struct task_struct *child),
		TPARGS(parent, child));

DECLARE_TRACE(sched_signal_send,
	TPPROTO(int sig, struct task_struct *p),
		TPARGS(sig, p));

#endif
