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

#ifndef __LINUX_SPINLOCK_UP_H
#define __LINUX_SPINLOCK_UP_H

#ifndef __LINUX_SPINLOCK_H
# error "please don't include this file directly"
#endif

/*
 * include/linux/spinlock_up.h - UP-debug version of spinlocks.
 *
 * portions Copyright 2005, Red Hat, Inc., Ingo Molnar
 * Released under the General Public License (GPL).
 *
 * In the debug case, 1 means unlocked, 0 means locked. (the values
 * are inverted, to catch initialization bugs)
 *
 * No atomicity anywhere, we are on UP.
 */

#ifdef CONFIG_DEBUG_SPINLOCK
#define __raw_spin_is_locked(x)		((x)->slock == 0)

static inline void __raw_spin_lock(raw_spinlock_t *lock)
{
	lock->slock = 0;
}

static inline void
__raw_spin_lock_flags(raw_spinlock_t *lock, unsigned long flags)
{
	local_irq_save(flags);
	lock->slock = 0;
}

static inline int __raw_spin_trylock(raw_spinlock_t *lock)
{
	char oldval = lock->slock;

	lock->slock = 0;

	return oldval > 0;
}

static inline void __raw_spin_unlock(raw_spinlock_t *lock)
{
	lock->slock = 1;
}

/*
 * Read-write spinlocks. No debug version.
 */
#define __raw_read_lock(lock)		do { (void)(lock); } while (0)
#define __raw_write_lock(lock)		do { (void)(lock); } while (0)
#define __raw_read_trylock(lock)	({ (void)(lock); 1; })
#define __raw_write_trylock(lock)	({ (void)(lock); 1; })
#define __raw_read_unlock(lock)		do { (void)(lock); } while (0)
#define __raw_write_unlock(lock)	do { (void)(lock); } while (0)

#else /* DEBUG_SPINLOCK */
#define __raw_spin_is_locked(lock)	((void)(lock), 0)
/* for sched.c and kernel_lock.c: */
# define __raw_spin_lock(lock)		do { (void)(lock); } while (0)
# define __raw_spin_unlock(lock)	do { (void)(lock); } while (0)
# define __raw_spin_trylock(lock)	({ (void)(lock); 1; })
#endif /* DEBUG_SPINLOCK */

#define __raw_spin_is_contended(lock)	(((void)(lock), 0))

#define __raw_read_can_lock(lock)	(((void)(lock), 1))
#define __raw_write_can_lock(lock)	(((void)(lock), 1))

#define __raw_spin_unlock_wait(lock) \
		do { cpu_relax(); } while (__raw_spin_is_locked(lock))

#endif /* __LINUX_SPINLOCK_UP_H */
