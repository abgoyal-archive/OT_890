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

#ifndef __BFIN_SPINLOCK_H
#define __BFIN_SPINLOCK_H

#include <asm/atomic.h>

asmlinkage int __raw_spin_is_locked_asm(volatile int *ptr);
asmlinkage void __raw_spin_lock_asm(volatile int *ptr);
asmlinkage int __raw_spin_trylock_asm(volatile int *ptr);
asmlinkage void __raw_spin_unlock_asm(volatile int *ptr);
asmlinkage void __raw_read_lock_asm(volatile int *ptr);
asmlinkage int __raw_read_trylock_asm(volatile int *ptr);
asmlinkage void __raw_read_unlock_asm(volatile int *ptr);
asmlinkage void __raw_write_lock_asm(volatile int *ptr);
asmlinkage int __raw_write_trylock_asm(volatile int *ptr);
asmlinkage void __raw_write_unlock_asm(volatile int *ptr);

static inline int __raw_spin_is_locked(raw_spinlock_t *lock)
{
	return __raw_spin_is_locked_asm(&lock->lock);
}

static inline void __raw_spin_lock(raw_spinlock_t *lock)
{
	__raw_spin_lock_asm(&lock->lock);
}

#define __raw_spin_lock_flags(lock, flags) __raw_spin_lock(lock)

static inline int __raw_spin_trylock(raw_spinlock_t *lock)
{
	return __raw_spin_trylock_asm(&lock->lock);
}

static inline void __raw_spin_unlock(raw_spinlock_t *lock)
{
	__raw_spin_unlock_asm(&lock->lock);
}

static inline void __raw_spin_unlock_wait(raw_spinlock_t *lock)
{
	while (__raw_spin_is_locked(lock))
		cpu_relax();
}

static inline int __raw_read_can_lock(raw_rwlock_t *rw)
{
	return __raw_uncached_fetch_asm(&rw->lock) > 0;
}

static inline int __raw_write_can_lock(raw_rwlock_t *rw)
{
	return __raw_uncached_fetch_asm(&rw->lock) == RW_LOCK_BIAS;
}

static inline void __raw_read_lock(raw_rwlock_t *rw)
{
	__raw_read_lock_asm(&rw->lock);
}

static inline int __raw_read_trylock(raw_rwlock_t *rw)
{
	return __raw_read_trylock_asm(&rw->lock);
}

static inline void __raw_read_unlock(raw_rwlock_t *rw)
{
	__raw_read_unlock_asm(&rw->lock);
}

static inline void __raw_write_lock(raw_rwlock_t *rw)
{
	__raw_write_lock_asm(&rw->lock);
}

static inline int __raw_write_trylock(raw_rwlock_t *rw)
{
	return __raw_write_trylock_asm(&rw->lock);
}

static inline void __raw_write_unlock(raw_rwlock_t *rw)
{
	__raw_write_unlock_asm(&rw->lock);
}

#define _raw_spin_relax(lock)  	cpu_relax()
#define _raw_read_relax(lock)	cpu_relax()
#define _raw_write_relax(lock)	cpu_relax()

#endif /*  !__BFIN_SPINLOCK_H */
