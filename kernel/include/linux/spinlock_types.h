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

#ifndef __LINUX_SPINLOCK_TYPES_H
#define __LINUX_SPINLOCK_TYPES_H

/*
 * include/linux/spinlock_types.h - generic spinlock type definitions
 *                                  and initializers
 *
 * portions Copyright 2005, Red Hat, Inc., Ingo Molnar
 * Released under the General Public License (GPL).
 */

#if defined(CONFIG_SMP)
# include <asm/spinlock_types.h>
#else
# include <linux/spinlock_types_up.h>
#endif

#include <linux/lockdep.h>

typedef struct {
	raw_spinlock_t raw_lock;
#ifdef CONFIG_GENERIC_LOCKBREAK
	unsigned int break_lock;
#endif
#ifdef CONFIG_DEBUG_SPINLOCK
	unsigned int magic, owner_cpu;
	void *owner;
#endif
#ifdef CONFIG_DEBUG_LOCK_ALLOC
	struct lockdep_map dep_map;
#endif
} spinlock_t;

#define SPINLOCK_MAGIC		0xdead4ead

typedef struct {
	raw_rwlock_t raw_lock;
#ifdef CONFIG_GENERIC_LOCKBREAK
	unsigned int break_lock;
#endif
#ifdef CONFIG_DEBUG_SPINLOCK
	unsigned int magic, owner_cpu;
	void *owner;
#endif
#ifdef CONFIG_DEBUG_LOCK_ALLOC
	struct lockdep_map dep_map;
#endif
} rwlock_t;

#define RWLOCK_MAGIC		0xdeaf1eed

#define SPINLOCK_OWNER_INIT	((void *)-1L)

#ifdef CONFIG_DEBUG_LOCK_ALLOC
# define SPIN_DEP_MAP_INIT(lockname)	.dep_map = { .name = #lockname }
#else
# define SPIN_DEP_MAP_INIT(lockname)
#endif

#ifdef CONFIG_DEBUG_LOCK_ALLOC
# define RW_DEP_MAP_INIT(lockname)	.dep_map = { .name = #lockname }
#else
# define RW_DEP_MAP_INIT(lockname)
#endif

#ifdef CONFIG_DEBUG_SPINLOCK
# define __SPIN_LOCK_UNLOCKED(lockname)					\
	(spinlock_t)	{	.raw_lock = __RAW_SPIN_LOCK_UNLOCKED,	\
				.magic = SPINLOCK_MAGIC,		\
				.owner = SPINLOCK_OWNER_INIT,		\
				.owner_cpu = -1,			\
				SPIN_DEP_MAP_INIT(lockname) }
#define __RW_LOCK_UNLOCKED(lockname)					\
	(rwlock_t)	{	.raw_lock = __RAW_RW_LOCK_UNLOCKED,	\
				.magic = RWLOCK_MAGIC,			\
				.owner = SPINLOCK_OWNER_INIT,		\
				.owner_cpu = -1,			\
				RW_DEP_MAP_INIT(lockname) }
#else
# define __SPIN_LOCK_UNLOCKED(lockname) \
	(spinlock_t)	{	.raw_lock = __RAW_SPIN_LOCK_UNLOCKED,	\
				SPIN_DEP_MAP_INIT(lockname) }
#define __RW_LOCK_UNLOCKED(lockname) \
	(rwlock_t)	{	.raw_lock = __RAW_RW_LOCK_UNLOCKED,	\
				RW_DEP_MAP_INIT(lockname) }
#endif

/*
 * SPIN_LOCK_UNLOCKED and RW_LOCK_UNLOCKED defeat lockdep state tracking and
 * are hence deprecated.
 * Please use DEFINE_SPINLOCK()/DEFINE_RWLOCK() or
 * __SPIN_LOCK_UNLOCKED()/__RW_LOCK_UNLOCKED() as appropriate.
 */
#define SPIN_LOCK_UNLOCKED	__SPIN_LOCK_UNLOCKED(old_style_spin_init)
#define RW_LOCK_UNLOCKED	__RW_LOCK_UNLOCKED(old_style_rw_init)

#define DEFINE_SPINLOCK(x)	spinlock_t x = __SPIN_LOCK_UNLOCKED(x)
#define DEFINE_RWLOCK(x)	rwlock_t x = __RW_LOCK_UNLOCKED(x)

#endif /* __LINUX_SPINLOCK_TYPES_H */
