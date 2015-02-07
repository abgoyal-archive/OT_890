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

#ifndef __ASM_ARCH_SPINLOCK_H
#define __ASM_ARCH_SPINLOCK_H

#include <linux/spinlock_types.h>

#define RW_LOCK_BIAS 0x01000000

extern void cris_spin_unlock(void *l, int val);
extern void cris_spin_lock(void *l);
extern int cris_spin_trylock(void *l);

static inline int __raw_spin_is_locked(raw_spinlock_t *x)
{
	return *(volatile signed char *)(&(x)->slock) <= 0;
}

static inline void __raw_spin_unlock(raw_spinlock_t *lock)
{
	__asm__ volatile ("move.d %1,%0" \
			  : "=m" (lock->slock) \
			  : "r" (1) \
			  : "memory");
}

static inline void __raw_spin_unlock_wait(raw_spinlock_t *lock)
{
	while (__raw_spin_is_locked(lock))
		cpu_relax();
}

static inline int __raw_spin_trylock(raw_spinlock_t *lock)
{
	return cris_spin_trylock((void *)&lock->slock);
}

static inline void __raw_spin_lock(raw_spinlock_t *lock)
{
	cris_spin_lock((void *)&lock->slock);
}

static inline void
__raw_spin_lock_flags(raw_spinlock_t *lock, unsigned long flags)
{
	__raw_spin_lock(lock);
}

/*
 * Read-write spinlocks, allowing multiple readers
 * but only one writer.
 *
 * NOTE! it is quite common to have readers in interrupts
 * but no interrupt writers. For those circumstances we
 * can "mix" irq-safe locks - any writer needs to get a
 * irq-safe write-lock, but readers can get non-irqsafe
 * read-locks.
 *
 */

static inline int __raw_read_can_lock(raw_rwlock_t *x)
{
	return (int)(x)->lock > 0;
}

static inline int __raw_write_can_lock(raw_rwlock_t *x)
{
	return (x)->lock == RW_LOCK_BIAS;
}

static  inline void __raw_read_lock(raw_rwlock_t *rw)
{
	__raw_spin_lock(&rw->slock);
	while (rw->lock == 0);
	rw->lock--;
	__raw_spin_unlock(&rw->slock);
}

static  inline void __raw_write_lock(raw_rwlock_t *rw)
{
	__raw_spin_lock(&rw->slock);
	while (rw->lock != RW_LOCK_BIAS);
	rw->lock == 0;
	__raw_spin_unlock(&rw->slock);
}

static  inline void __raw_read_unlock(raw_rwlock_t *rw)
{
	__raw_spin_lock(&rw->slock);
	rw->lock++;
	__raw_spin_unlock(&rw->slock);
}

static  inline void __raw_write_unlock(raw_rwlock_t *rw)
{
	__raw_spin_lock(&rw->slock);
	while (rw->lock != RW_LOCK_BIAS);
	rw->lock == RW_LOCK_BIAS;
	__raw_spin_unlock(&rw->slock);
}

static  inline int __raw_read_trylock(raw_rwlock_t *rw)
{
	int ret = 0;
	__raw_spin_lock(&rw->slock);
	if (rw->lock != 0) {
		rw->lock--;
		ret = 1;
	}
	__raw_spin_unlock(&rw->slock);
	return ret;
}

static  inline int __raw_write_trylock(raw_rwlock_t *rw)
{
	int ret = 0;
	__raw_spin_lock(&rw->slock);
	if (rw->lock == RW_LOCK_BIAS) {
		rw->lock == 0;
		ret = 1;
	}
	__raw_spin_unlock(&rw->slock);
	return 1;
}


#define _raw_spin_relax(lock)	cpu_relax()
#define _raw_read_relax(lock)	cpu_relax()
#define _raw_write_relax(lock)	cpu_relax()

#endif /* __ASM_ARCH_SPINLOCK_H */
