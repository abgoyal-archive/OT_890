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
 * include/asm-sh/spinlock.h
 *
 * Copyright (C) 2002, 2003 Paul Mundt
 * Copyright (C) 2006, 2007 Akio Idehara
 *
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 */
#ifndef __ASM_SH_SPINLOCK_H
#define __ASM_SH_SPINLOCK_H

/*
 * The only locking implemented here uses SH-4A opcodes. For others,
 * split this out as per atomic-*.h.
 */
#ifndef CONFIG_CPU_SH4A
#error "Need movli.l/movco.l for spinlocks"
#endif

/*
 * Your basic SMP spinlocks, allowing only a single CPU anywhere
 */

#define __raw_spin_is_locked(x)		((x)->lock <= 0)
#define __raw_spin_lock_flags(lock, flags) __raw_spin_lock(lock)
#define __raw_spin_unlock_wait(x) \
	do { cpu_relax(); } while ((x)->lock)

/*
 * Simple spin lock operations.  There are two variants, one clears IRQ's
 * on the local processor, one does not.
 *
 * We make no fairness assumptions.  They have a cost.
 */
static inline void __raw_spin_lock(raw_spinlock_t *lock)
{
	unsigned long tmp;
	unsigned long oldval;

	__asm__ __volatile__ (
		"1:						\n\t"
		"movli.l	@%2, %0	! __raw_spin_lock	\n\t"
		"mov		%0, %1				\n\t"
		"mov		#0, %0				\n\t"
		"movco.l	%0, @%2				\n\t"
		"bf		1b				\n\t"
		"cmp/pl		%1				\n\t"
		"bf		1b				\n\t"
		: "=&z" (tmp), "=&r" (oldval)
		: "r" (&lock->lock)
		: "t", "memory"
	);
}

static inline void __raw_spin_unlock(raw_spinlock_t *lock)
{
	unsigned long tmp;

	__asm__ __volatile__ (
		"mov		#1, %0 ! __raw_spin_unlock	\n\t"
		"mov.l		%0, @%1				\n\t"
		: "=&z" (tmp)
		: "r" (&lock->lock)
		: "t", "memory"
	);
}

static inline int __raw_spin_trylock(raw_spinlock_t *lock)
{
	unsigned long tmp, oldval;

	__asm__ __volatile__ (
		"1:						\n\t"
		"movli.l	@%2, %0	! __raw_spin_trylock	\n\t"
		"mov		%0, %1				\n\t"
		"mov		#0, %0				\n\t"
		"movco.l	%0, @%2				\n\t"
		"bf		1b				\n\t"
		"synco						\n\t"
		: "=&z" (tmp), "=&r" (oldval)
		: "r" (&lock->lock)
		: "t", "memory"
	);

	return oldval;
}

/*
 * Read-write spinlocks, allowing multiple readers but only one writer.
 *
 * NOTE! it is quite common to have readers in interrupts but no interrupt
 * writers. For those circumstances we can "mix" irq-safe locks - any writer
 * needs to get a irq-safe write-lock, but readers can get non-irqsafe
 * read-locks.
 */

/**
 * read_can_lock - would read_trylock() succeed?
 * @lock: the rwlock in question.
 */
#define __raw_read_can_lock(x)	((x)->lock > 0)

/**
 * write_can_lock - would write_trylock() succeed?
 * @lock: the rwlock in question.
 */
#define __raw_write_can_lock(x)	((x)->lock == RW_LOCK_BIAS)

static inline void __raw_read_lock(raw_rwlock_t *rw)
{
	unsigned long tmp;

	__asm__ __volatile__ (
		"1:						\n\t"
		"movli.l	@%1, %0	! __raw_read_lock	\n\t"
		"cmp/pl		%0				\n\t"
		"bf		1b				\n\t"
		"add		#-1, %0				\n\t"
		"movco.l	%0, @%1				\n\t"
		"bf		1b				\n\t"
		: "=&z" (tmp)
		: "r" (&rw->lock)
		: "t", "memory"
	);
}

static inline void __raw_read_unlock(raw_rwlock_t *rw)
{
	unsigned long tmp;

	__asm__ __volatile__ (
		"1:						\n\t"
		"movli.l	@%1, %0	! __raw_read_unlock	\n\t"
		"add		#1, %0				\n\t"
		"movco.l	%0, @%1				\n\t"
		"bf		1b				\n\t"
		: "=&z" (tmp)
		: "r" (&rw->lock)
		: "t", "memory"
	);
}

static inline void __raw_write_lock(raw_rwlock_t *rw)
{
	unsigned long tmp;

	__asm__ __volatile__ (
		"1:						\n\t"
		"movli.l	@%1, %0	! __raw_write_lock	\n\t"
		"cmp/hs		%2, %0				\n\t"
		"bf		1b				\n\t"
		"sub		%2, %0				\n\t"
		"movco.l	%0, @%1				\n\t"
		"bf		1b				\n\t"
		: "=&z" (tmp)
		: "r" (&rw->lock), "r" (RW_LOCK_BIAS)
		: "t", "memory"
	);
}

static inline void __raw_write_unlock(raw_rwlock_t *rw)
{
	__asm__ __volatile__ (
		"mov.l		%1, @%0 ! __raw_write_unlock	\n\t"
		:
		: "r" (&rw->lock), "r" (RW_LOCK_BIAS)
		: "t", "memory"
	);
}

static inline int __raw_read_trylock(raw_rwlock_t *rw)
{
	unsigned long tmp, oldval;

	__asm__ __volatile__ (
		"1:						\n\t"
		"movli.l	@%2, %0	! __raw_read_trylock	\n\t"
		"mov		%0, %1				\n\t"
		"cmp/pl		%0				\n\t"
		"bf		2f				\n\t"
		"add		#-1, %0				\n\t"
		"movco.l	%0, @%2				\n\t"
		"bf		1b				\n\t"
		"2:						\n\t"
		"synco						\n\t"
		: "=&z" (tmp), "=&r" (oldval)
		: "r" (&rw->lock)
		: "t", "memory"
	);

	return (oldval > 0);
}

static inline int __raw_write_trylock(raw_rwlock_t *rw)
{
	unsigned long tmp, oldval;

	__asm__ __volatile__ (
		"1:						\n\t"
		"movli.l	@%2, %0	! __raw_write_trylock	\n\t"
		"mov		%0, %1				\n\t"
		"cmp/hs		%3, %0				\n\t"
		"bf		2f				\n\t"
		"sub		%3, %0				\n\t"
		"2:						\n\t"
		"movco.l	%0, @%2				\n\t"
		"bf		1b				\n\t"
		"synco						\n\t"
		: "=&z" (tmp), "=&r" (oldval)
		: "r" (&rw->lock), "r" (RW_LOCK_BIAS)
		: "t", "memory"
	);

	return (oldval > (RW_LOCK_BIAS - 1));
}

#define _raw_spin_relax(lock)	cpu_relax()
#define _raw_read_relax(lock)	cpu_relax()
#define _raw_write_relax(lock)	cpu_relax()

#endif /* __ASM_SH_SPINLOCK_H */
