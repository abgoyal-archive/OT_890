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
 * rwsem.h: R/W semaphores implemented using CAS
 *
 * Written by David S. Miller (davem@redhat.com), 2001.
 * Derived from asm-i386/rwsem.h
 */
#ifndef _SPARC64_RWSEM_H
#define _SPARC64_RWSEM_H

#ifndef _LINUX_RWSEM_H
#error "please don't include asm/rwsem.h directly, use linux/rwsem.h instead"
#endif

#ifdef __KERNEL__

#include <linux/list.h>
#include <linux/spinlock.h>
#include <asm/rwsem-const.h>

struct rwsem_waiter;

struct rw_semaphore {
	signed int count;
	spinlock_t		wait_lock;
	struct list_head	wait_list;
#ifdef CONFIG_DEBUG_LOCK_ALLOC
	struct lockdep_map	dep_map;
#endif
};

#ifdef CONFIG_DEBUG_LOCK_ALLOC
# define __RWSEM_DEP_MAP_INIT(lockname) , .dep_map = { .name = #lockname }
#else
# define __RWSEM_DEP_MAP_INIT(lockname)
#endif

#define __RWSEM_INITIALIZER(name) \
{ RWSEM_UNLOCKED_VALUE, SPIN_LOCK_UNLOCKED, LIST_HEAD_INIT((name).wait_list) \
  __RWSEM_DEP_MAP_INIT(name) }

#define DECLARE_RWSEM(name) \
	struct rw_semaphore name = __RWSEM_INITIALIZER(name)

extern void __init_rwsem(struct rw_semaphore *sem, const char *name,
			 struct lock_class_key *key);

#define init_rwsem(sem)						\
do {								\
	static struct lock_class_key __key;			\
								\
	__init_rwsem((sem), #sem, &__key);			\
} while (0)

extern void __down_read(struct rw_semaphore *sem);
extern int __down_read_trylock(struct rw_semaphore *sem);
extern void __down_write(struct rw_semaphore *sem);
extern int __down_write_trylock(struct rw_semaphore *sem);
extern void __up_read(struct rw_semaphore *sem);
extern void __up_write(struct rw_semaphore *sem);
extern void __downgrade_write(struct rw_semaphore *sem);

static inline void __down_write_nested(struct rw_semaphore *sem, int subclass)
{
	__down_write(sem);
}

static inline int rwsem_atomic_update(int delta, struct rw_semaphore *sem)
{
	return atomic_add_return(delta, (atomic_t *)(&sem->count));
}

static inline void rwsem_atomic_add(int delta, struct rw_semaphore *sem)
{
	atomic_add(delta, (atomic_t *)(&sem->count));
}

static inline int rwsem_is_locked(struct rw_semaphore *sem)
{
	return (sem->count != 0);
}

#endif /* __KERNEL__ */

#endif /* _SPARC64_RWSEM_H */
