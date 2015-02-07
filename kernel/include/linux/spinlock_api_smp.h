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

#ifndef __LINUX_SPINLOCK_API_SMP_H
#define __LINUX_SPINLOCK_API_SMP_H

#ifndef __LINUX_SPINLOCK_H
# error "please don't include this file directly"
#endif

/*
 * include/linux/spinlock_api_smp.h
 *
 * spinlock API declarations on SMP (and debug)
 * (implemented in kernel/spinlock.c)
 *
 * portions Copyright 2005, Red Hat, Inc., Ingo Molnar
 * Released under the General Public License (GPL).
 */

int in_lock_functions(unsigned long addr);

#define assert_spin_locked(x)	BUG_ON(!spin_is_locked(x))

void __lockfunc _spin_lock(spinlock_t *lock)		__acquires(lock);
void __lockfunc _spin_lock_nested(spinlock_t *lock, int subclass)
							__acquires(lock);
void __lockfunc _spin_lock_nest_lock(spinlock_t *lock, struct lockdep_map *map)
							__acquires(lock);
void __lockfunc _read_lock(rwlock_t *lock)		__acquires(lock);
void __lockfunc _write_lock(rwlock_t *lock)		__acquires(lock);
void __lockfunc _spin_lock_bh(spinlock_t *lock)		__acquires(lock);
void __lockfunc _read_lock_bh(rwlock_t *lock)		__acquires(lock);
void __lockfunc _write_lock_bh(rwlock_t *lock)		__acquires(lock);
void __lockfunc _spin_lock_irq(spinlock_t *lock)	__acquires(lock);
void __lockfunc _read_lock_irq(rwlock_t *lock)		__acquires(lock);
void __lockfunc _write_lock_irq(rwlock_t *lock)		__acquires(lock);
unsigned long __lockfunc _spin_lock_irqsave(spinlock_t *lock)
							__acquires(lock);
unsigned long __lockfunc _spin_lock_irqsave_nested(spinlock_t *lock, int subclass)
							__acquires(lock);
unsigned long __lockfunc _read_lock_irqsave(rwlock_t *lock)
							__acquires(lock);
unsigned long __lockfunc _write_lock_irqsave(rwlock_t *lock)
							__acquires(lock);
int __lockfunc _spin_trylock(spinlock_t *lock);
int __lockfunc _read_trylock(rwlock_t *lock);
int __lockfunc _write_trylock(rwlock_t *lock);
int __lockfunc _spin_trylock_bh(spinlock_t *lock);
void __lockfunc _spin_unlock(spinlock_t *lock)		__releases(lock);
void __lockfunc _read_unlock(rwlock_t *lock)		__releases(lock);
void __lockfunc _write_unlock(rwlock_t *lock)		__releases(lock);
void __lockfunc _spin_unlock_bh(spinlock_t *lock)	__releases(lock);
void __lockfunc _read_unlock_bh(rwlock_t *lock)		__releases(lock);
void __lockfunc _write_unlock_bh(rwlock_t *lock)	__releases(lock);
void __lockfunc _spin_unlock_irq(spinlock_t *lock)	__releases(lock);
void __lockfunc _read_unlock_irq(rwlock_t *lock)	__releases(lock);
void __lockfunc _write_unlock_irq(rwlock_t *lock)	__releases(lock);
void __lockfunc _spin_unlock_irqrestore(spinlock_t *lock, unsigned long flags)
							__releases(lock);
void __lockfunc _read_unlock_irqrestore(rwlock_t *lock, unsigned long flags)
							__releases(lock);
void __lockfunc _write_unlock_irqrestore(rwlock_t *lock, unsigned long flags)
							__releases(lock);

#endif /* __LINUX_SPINLOCK_API_SMP_H */
