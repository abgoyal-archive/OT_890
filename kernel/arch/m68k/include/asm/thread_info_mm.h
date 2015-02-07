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

#ifndef _ASM_M68K_THREAD_INFO_H
#define _ASM_M68K_THREAD_INFO_H

#include <asm/types.h>
#include <asm/page.h>

struct thread_info {
	struct task_struct	*task;		/* main task structure */
	unsigned long		flags;
	struct exec_domain	*exec_domain;	/* execution domain */
	int			preempt_count;	/* 0 => preemptable, <0 => BUG */
	__u32 cpu; /* should always be 0 on m68k */
	struct restart_block    restart_block;
};

#define PREEMPT_ACTIVE		0x4000000

#define INIT_THREAD_INFO(tsk)			\
{						\
	.task		= &tsk,			\
	.exec_domain	= &default_exec_domain,	\
	.restart_block = {			\
		.fn = do_no_restart_syscall,	\
	},					\
}

/* THREAD_SIZE should be 8k, so handle differently for 4k and 8k machines */
#define THREAD_SIZE_ORDER (13 - PAGE_SHIFT)

#define init_thread_info	(init_task.thread.info)
#define init_stack		(init_thread_union.stack)

#define task_thread_info(tsk)	(&(tsk)->thread.info)
#define task_stack_page(tsk)	((tsk)->stack)
#define current_thread_info()	task_thread_info(current)

#define __HAVE_THREAD_FUNCTIONS

#define setup_thread_stack(p, org) ({			\
	*(struct task_struct **)(p)->stack = (p);	\
	task_thread_info(p)->task = (p);		\
})

#define end_of_stack(p) ((unsigned long *)(p)->stack + 1)

/* entry.S relies on these definitions!
 * bits 0-7 are tested at every exception exit
 * bits 8-15 are also tested at syscall exit
 */
#define TIF_SIGPENDING		6	/* signal pending */
#define TIF_NEED_RESCHED	7	/* rescheduling necessary */
#define TIF_DELAYED_TRACE	14	/* single step a syscall */
#define TIF_SYSCALL_TRACE	15	/* syscall trace active */
#define TIF_MEMDIE		16
#define TIF_FREEZE		17	/* thread is freezing for suspend */

#endif	/* _ASM_M68K_THREAD_INFO_H */
