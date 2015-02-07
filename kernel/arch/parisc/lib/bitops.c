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
 * bitops.c: atomic operations which got too long to be inlined all over
 *      the place.
 * 
 * Copyright 1999 Philipp Rumpf (prumpf@tux.org)
 * Copyright 2000 Grant Grundler (grundler@cup.hp.com)
 */

#include <linux/kernel.h>
#include <linux/spinlock.h>
#include <asm/system.h>
#include <asm/atomic.h>

#ifdef CONFIG_SMP
raw_spinlock_t __atomic_hash[ATOMIC_HASH_SIZE] __lock_aligned = {
	[0 ... (ATOMIC_HASH_SIZE-1)]  = __RAW_SPIN_LOCK_UNLOCKED
};
#endif

#ifdef CONFIG_64BIT
unsigned long __xchg64(unsigned long x, unsigned long *ptr)
{
	unsigned long temp, flags;

	_atomic_spin_lock_irqsave(ptr, flags);
	temp = *ptr;
	*ptr = x;
	_atomic_spin_unlock_irqrestore(ptr, flags);
	return temp;
}
#endif

unsigned long __xchg32(int x, int *ptr)
{
	unsigned long flags;
	long temp;

	_atomic_spin_lock_irqsave(ptr, flags);
	temp = (long) *ptr;	/* XXX - sign extension wanted? */
	*ptr = x;
	_atomic_spin_unlock_irqrestore(ptr, flags);
	return (unsigned long)temp;
}


unsigned long __xchg8(char x, char *ptr)
{
	unsigned long flags;
	long temp;

	_atomic_spin_lock_irqsave(ptr, flags);
	temp = (long) *ptr;	/* XXX - sign extension wanted? */
	*ptr = x;
	_atomic_spin_unlock_irqrestore(ptr, flags);
	return (unsigned long)temp;
}


#ifdef CONFIG_64BIT
unsigned long __cmpxchg_u64(volatile unsigned long *ptr, unsigned long old, unsigned long new)
{
	unsigned long flags;
	unsigned long prev;

	_atomic_spin_lock_irqsave(ptr, flags);
	if ((prev = *ptr) == old)
		*ptr = new;
	_atomic_spin_unlock_irqrestore(ptr, flags);
	return prev;
}
#endif

unsigned long __cmpxchg_u32(volatile unsigned int *ptr, unsigned int old, unsigned int new)
{
	unsigned long flags;
	unsigned int prev;

	_atomic_spin_lock_irqsave(ptr, flags);
	if ((prev = *ptr) == old)
		*ptr = new;
	_atomic_spin_unlock_irqrestore(ptr, flags);
	return (unsigned long)prev;
}
