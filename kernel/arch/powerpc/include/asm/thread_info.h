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

/* thread_info.h: PowerPC low-level thread information
 * adapted from the i386 version by Paul Mackerras
 *
 * Copyright (C) 2002  David Howells (dhowells@redhat.com)
 * - Incorporating suggestions made by Linus Torvalds and Dave Miller
 */

#ifndef _ASM_POWERPC_THREAD_INFO_H
#define _ASM_POWERPC_THREAD_INFO_H

#ifdef __KERNEL__

/* We have 8k stacks on ppc32 and 16k on ppc64 */

#ifdef CONFIG_PPC64
#define THREAD_SHIFT		14
#else
#define THREAD_SHIFT		13
#endif

#define THREAD_SIZE		(1 << THREAD_SHIFT)

#ifndef __ASSEMBLY__
#include <linux/cache.h>
#include <asm/processor.h>
#include <asm/page.h>
#include <linux/stringify.h>

/*
 * low level task data.
 */
struct thread_info {
	struct task_struct *task;		/* main task structure */
	struct exec_domain *exec_domain;	/* execution domain */
	int		cpu;			/* cpu we're on */
	int		preempt_count;		/* 0 => preemptable,
						   <0 => BUG */
	struct restart_block restart_block;
	unsigned long	local_flags;		/* private flags for thread */

	/* low level flags - has atomic operations done on it */
	unsigned long	flags ____cacheline_aligned_in_smp;
};

/*
 * macros/functions for gaining access to the thread information structure
 *
 * preempt_count needs to be 1 initially, until the scheduler is functional.
 */
#define INIT_THREAD_INFO(tsk)			\
{						\
	.task =		&tsk,			\
	.exec_domain =	&default_exec_domain,	\
	.cpu =		0,			\
	.preempt_count = 1,			\
	.restart_block = {			\
		.fn = do_no_restart_syscall,	\
	},					\
	.flags =	0,			\
}

#define init_thread_info	(init_thread_union.thread_info)
#define init_stack		(init_thread_union.stack)

/* thread information allocation */

#if THREAD_SHIFT >= PAGE_SHIFT

#define THREAD_SIZE_ORDER	(THREAD_SHIFT - PAGE_SHIFT)

#else /* THREAD_SHIFT < PAGE_SHIFT */

#define __HAVE_ARCH_THREAD_INFO_ALLOCATOR

extern struct thread_info *alloc_thread_info(struct task_struct *tsk);
extern void free_thread_info(struct thread_info *ti);

#endif /* THREAD_SHIFT < PAGE_SHIFT */

/* how to get the thread information struct from C */
static inline struct thread_info *current_thread_info(void)
{
	register unsigned long sp asm("r1");

	/* gcc4, at least, is smart enough to turn this into a single
	 * rlwinm for ppc32 and clrrdi for ppc64 */
	return (struct thread_info *)(sp & ~(THREAD_SIZE-1));
}

#endif /* __ASSEMBLY__ */

#define PREEMPT_ACTIVE		0x10000000

/*
 * thread information flag bit numbers
 */
#define TIF_SYSCALL_TRACE	0	/* syscall trace active */
#define TIF_SIGPENDING		1	/* signal pending */
#define TIF_NEED_RESCHED	2	/* rescheduling necessary */
#define TIF_POLLING_NRFLAG	3	/* true if poll_idle() is polling
					   TIF_NEED_RESCHED */
#define TIF_32BIT		4	/* 32 bit binary */
#define TIF_PERFMON_WORK	5	/* work for pfm_handle_work() */
#define TIF_PERFMON_CTXSW	6	/* perfmon needs ctxsw calls */
#define TIF_SYSCALL_AUDIT	7	/* syscall auditing active */
#define TIF_SINGLESTEP		8	/* singlestepping active */
#define TIF_MEMDIE		9
#define TIF_SECCOMP		10	/* secure computing */
#define TIF_RESTOREALL		11	/* Restore all regs (implies NOERROR) */
#define TIF_NOERROR		12	/* Force successful syscall return */
#define TIF_NOTIFY_RESUME	13	/* callback before returning to user */
#define TIF_FREEZE		14	/* Freezing for suspend */
#define TIF_RUNLATCH		15	/* Is the runlatch enabled? */
#define TIF_ABI_PENDING		16	/* 32/64 bit switch needed */

/* as above, but as bit values */
#define _TIF_SYSCALL_TRACE	(1<<TIF_SYSCALL_TRACE)
#define _TIF_SIGPENDING		(1<<TIF_SIGPENDING)
#define _TIF_NEED_RESCHED	(1<<TIF_NEED_RESCHED)
#define _TIF_POLLING_NRFLAG	(1<<TIF_POLLING_NRFLAG)
#define _TIF_32BIT		(1<<TIF_32BIT)
#define _TIF_PERFMON_WORK	(1<<TIF_PERFMON_WORK)
#define _TIF_PERFMON_CTXSW	(1<<TIF_PERFMON_CTXSW)
#define _TIF_SYSCALL_AUDIT	(1<<TIF_SYSCALL_AUDIT)
#define _TIF_SINGLESTEP		(1<<TIF_SINGLESTEP)
#define _TIF_SECCOMP		(1<<TIF_SECCOMP)
#define _TIF_RESTOREALL		(1<<TIF_RESTOREALL)
#define _TIF_NOERROR		(1<<TIF_NOERROR)
#define _TIF_NOTIFY_RESUME	(1<<TIF_NOTIFY_RESUME)
#define _TIF_FREEZE		(1<<TIF_FREEZE)
#define _TIF_RUNLATCH		(1<<TIF_RUNLATCH)
#define _TIF_ABI_PENDING	(1<<TIF_ABI_PENDING)
#define _TIF_SYSCALL_T_OR_A	(_TIF_SYSCALL_TRACE|_TIF_SYSCALL_AUDIT|_TIF_SECCOMP)

#define _TIF_USER_WORK_MASK	(_TIF_SIGPENDING | _TIF_NEED_RESCHED | \
				 _TIF_NOTIFY_RESUME)
#define _TIF_PERSYSCALL_MASK	(_TIF_RESTOREALL|_TIF_NOERROR)

/* Bits in local_flags */
/* Don't move TLF_NAPPING without adjusting the code in entry_32.S */
#define TLF_NAPPING		0	/* idle thread enabled NAP mode */
#define TLF_SLEEPING		1	/* suspend code enabled SLEEP mode */
#define TLF_RESTORE_SIGMASK	2	/* Restore signal mask in do_signal */

#define _TLF_NAPPING		(1 << TLF_NAPPING)
#define _TLF_SLEEPING		(1 << TLF_SLEEPING)
#define _TLF_RESTORE_SIGMASK	(1 << TLF_RESTORE_SIGMASK)

#ifndef __ASSEMBLY__
#define HAVE_SET_RESTORE_SIGMASK	1
static inline void set_restore_sigmask(void)
{
	struct thread_info *ti = current_thread_info();
	ti->local_flags |= _TLF_RESTORE_SIGMASK;
	set_bit(TIF_SIGPENDING, &ti->flags);
}
#endif	/* !__ASSEMBLY__ */

#endif /* __KERNEL__ */

#endif /* _ASM_POWERPC_THREAD_INFO_H */
