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

#ifndef _ASM_PARISC_THREAD_INFO_H
#define _ASM_PARISC_THREAD_INFO_H

#ifdef __KERNEL__

#ifndef __ASSEMBLY__
#include <asm/processor.h>

struct thread_info {
	struct task_struct *task;	/* main task structure */
	struct exec_domain *exec_domain;/* execution domain */
	unsigned long flags;		/* thread_info flags (see TIF_*) */
	mm_segment_t addr_limit;	/* user-level address space limit */
	__u32 cpu;			/* current CPU */
	int preempt_count;		/* 0=premptable, <0=BUG; will also serve as bh-counter */
	struct restart_block restart_block;
};

#define INIT_THREAD_INFO(tsk)			\
{						\
	.task		= &tsk,			\
	.exec_domain	= &default_exec_domain,	\
	.flags		= 0,			\
	.cpu		= 0,			\
	.addr_limit	= KERNEL_DS,		\
	.preempt_count	= 1,			\
  	.restart_block	= {			\
		.fn = do_no_restart_syscall	\
	}					\
}

#define init_thread_info        (init_thread_union.thread_info)
#define init_stack              (init_thread_union.stack)

/* thread information allocation */

#define THREAD_SIZE_ORDER            2
/* Be sure to hunt all references to this down when you change the size of
 * the kernel stack */
#define THREAD_SIZE             (PAGE_SIZE << THREAD_SIZE_ORDER)
#define THREAD_SHIFT            (PAGE_SHIFT + THREAD_SIZE_ORDER)

/* how to get the thread information struct from C */
#define current_thread_info()	((struct thread_info *)mfctl(30))

#endif /* !__ASSEMBLY */

#define PREEMPT_ACTIVE_BIT	28
#define PREEMPT_ACTIVE		(1 << PREEMPT_ACTIVE_BIT)

/*
 * thread information flags
 */
#define TIF_SYSCALL_TRACE	0	/* syscall trace active */
#define TIF_SIGPENDING		1	/* signal pending */
#define TIF_NEED_RESCHED	2	/* rescheduling necessary */
#define TIF_POLLING_NRFLAG	3	/* true if poll_idle() is polling TIF_NEED_RESCHED */
#define TIF_32BIT               4       /* 32 bit binary */
#define TIF_MEMDIE		5
#define TIF_RESTORE_SIGMASK	6	/* restore saved signal mask */
#define TIF_FREEZE		7	/* is freezing for suspend */

#define _TIF_SYSCALL_TRACE	(1 << TIF_SYSCALL_TRACE)
#define _TIF_SIGPENDING		(1 << TIF_SIGPENDING)
#define _TIF_NEED_RESCHED	(1 << TIF_NEED_RESCHED)
#define _TIF_POLLING_NRFLAG	(1 << TIF_POLLING_NRFLAG)
#define _TIF_32BIT		(1 << TIF_32BIT)
#define _TIF_RESTORE_SIGMASK	(1 << TIF_RESTORE_SIGMASK)
#define _TIF_FREEZE		(1 << TIF_FREEZE)

#define _TIF_USER_WORK_MASK     (_TIF_SIGPENDING | \
                                 _TIF_NEED_RESCHED | _TIF_RESTORE_SIGMASK)

#endif /* __KERNEL__ */

#endif /* _ASM_PARISC_THREAD_INFO_H */
