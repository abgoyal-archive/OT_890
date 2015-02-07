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
 * RT Mutexes: blocking mutual exclusion locks with PI support
 *
 * started by Ingo Molnar and Thomas Gleixner:
 *
 *  Copyright (C) 2004-2006 Red Hat, Inc., Ingo Molnar <mingo@redhat.com>
 *  Copyright (C) 2006, Timesys Corp., Thomas Gleixner <tglx@timesys.com>
 *
 * This file contains the private data structure and API definitions.
 */

#ifndef __KERNEL_RTMUTEX_COMMON_H
#define __KERNEL_RTMUTEX_COMMON_H

#include <linux/rtmutex.h>

/*
 * The rtmutex in kernel tester is independent of rtmutex debugging. We
 * call schedule_rt_mutex_test() instead of schedule() for the tasks which
 * belong to the tester. That way we can delay the wakeup path of those
 * threads to provoke lock stealing and testing of  complex boosting scenarios.
 */
#ifdef CONFIG_RT_MUTEX_TESTER

extern void schedule_rt_mutex_test(struct rt_mutex *lock);

#define schedule_rt_mutex(_lock)				\
  do {								\
	if (!(current->flags & PF_MUTEX_TESTER))		\
		schedule();					\
	else							\
		schedule_rt_mutex_test(_lock);			\
  } while (0)

#else
# define schedule_rt_mutex(_lock)			schedule()
#endif

/*
 * This is the control structure for tasks blocked on a rt_mutex,
 * which is allocated on the kernel stack on of the blocked task.
 *
 * @list_entry:		pi node to enqueue into the mutex waiters list
 * @pi_list_entry:	pi node to enqueue into the mutex owner waiters list
 * @task:		task reference to the blocked task
 */
struct rt_mutex_waiter {
	struct plist_node	list_entry;
	struct plist_node	pi_list_entry;
	struct task_struct	*task;
	struct rt_mutex		*lock;
#ifdef CONFIG_DEBUG_RT_MUTEXES
	unsigned long		ip;
	struct pid		*deadlock_task_pid;
	struct rt_mutex		*deadlock_lock;
#endif
};

/*
 * Various helpers to access the waiters-plist:
 */
static inline int rt_mutex_has_waiters(struct rt_mutex *lock)
{
	return !plist_head_empty(&lock->wait_list);
}

static inline struct rt_mutex_waiter *
rt_mutex_top_waiter(struct rt_mutex *lock)
{
	struct rt_mutex_waiter *w;

	w = plist_first_entry(&lock->wait_list, struct rt_mutex_waiter,
			       list_entry);
	BUG_ON(w->lock != lock);

	return w;
}

static inline int task_has_pi_waiters(struct task_struct *p)
{
	return !plist_head_empty(&p->pi_waiters);
}

static inline struct rt_mutex_waiter *
task_top_pi_waiter(struct task_struct *p)
{
	return plist_first_entry(&p->pi_waiters, struct rt_mutex_waiter,
				  pi_list_entry);
}

/*
 * lock->owner state tracking:
 */
#define RT_MUTEX_OWNER_PENDING	1UL
#define RT_MUTEX_HAS_WAITERS	2UL
#define RT_MUTEX_OWNER_MASKALL	3UL

static inline struct task_struct *rt_mutex_owner(struct rt_mutex *lock)
{
	return (struct task_struct *)
		((unsigned long)lock->owner & ~RT_MUTEX_OWNER_MASKALL);
}

static inline struct task_struct *rt_mutex_real_owner(struct rt_mutex *lock)
{
	return (struct task_struct *)
		((unsigned long)lock->owner & ~RT_MUTEX_HAS_WAITERS);
}

static inline unsigned long rt_mutex_owner_pending(struct rt_mutex *lock)
{
	return (unsigned long)lock->owner & RT_MUTEX_OWNER_PENDING;
}

/*
 * PI-futex support (proxy locking functions, etc.):
 */
extern struct task_struct *rt_mutex_next_owner(struct rt_mutex *lock);
extern void rt_mutex_init_proxy_locked(struct rt_mutex *lock,
				       struct task_struct *proxy_owner);
extern void rt_mutex_proxy_unlock(struct rt_mutex *lock,
				  struct task_struct *proxy_owner);

#ifdef CONFIG_DEBUG_RT_MUTEXES
# include "rtmutex-debug.h"
#else
# include "rtmutex.h"
#endif

#endif
