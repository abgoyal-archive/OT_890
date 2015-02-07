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
 * Mutexes: blocking mutual exclusion locks
 *
 * started by Ingo Molnar:
 *
 *  Copyright (C) 2004, 2005, 2006 Red Hat, Inc., Ingo Molnar <mingo@redhat.com>
 *
 * This file contains mutex debugging related internal declarations,
 * prototypes and inline functions, for the CONFIG_DEBUG_MUTEXES case.
 * More details are in kernel/mutex-debug.c.
 */

/*
 * This must be called with lock->wait_lock held.
 */
extern void
debug_mutex_set_owner(struct mutex *lock, struct thread_info *new_owner);

static inline void debug_mutex_clear_owner(struct mutex *lock)
{
	lock->owner = NULL;
}

extern void debug_mutex_lock_common(struct mutex *lock,
				    struct mutex_waiter *waiter);
extern void debug_mutex_wake_waiter(struct mutex *lock,
				    struct mutex_waiter *waiter);
extern void debug_mutex_free_waiter(struct mutex_waiter *waiter);
extern void debug_mutex_add_waiter(struct mutex *lock,
				   struct mutex_waiter *waiter,
				   struct thread_info *ti);
extern void mutex_remove_waiter(struct mutex *lock, struct mutex_waiter *waiter,
				struct thread_info *ti);
extern void debug_mutex_unlock(struct mutex *lock);
extern void debug_mutex_init(struct mutex *lock, const char *name,
			     struct lock_class_key *key);

#define spin_lock_mutex(lock, flags)			\
	do {						\
		struct mutex *l = container_of(lock, struct mutex, wait_lock); \
							\
		DEBUG_LOCKS_WARN_ON(in_interrupt());	\
		local_irq_save(flags);			\
		__raw_spin_lock(&(lock)->raw_lock);	\
		DEBUG_LOCKS_WARN_ON(l->magic != l);	\
	} while (0)

#define spin_unlock_mutex(lock, flags)			\
	do {						\
		__raw_spin_unlock(&(lock)->raw_lock);	\
		local_irq_restore(flags);		\
		preempt_check_resched();		\
	} while (0)
