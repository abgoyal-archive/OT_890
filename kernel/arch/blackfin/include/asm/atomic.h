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

#ifndef __ARCH_BLACKFIN_ATOMIC__
#define __ARCH_BLACKFIN_ATOMIC__

#include <linux/types.h>
#include <asm/system.h>	/* local_irq_XXX() */

/*
 * Atomic operations that C can't guarantee us.  Useful for
 * resource counting etc..
 *
 * Generally we do not concern about SMP BFIN systems, so we don't have
 * to deal with that.
 *
 * Tony Kou (tonyko@lineo.ca)   Lineo Inc.   2001
 */

#define ATOMIC_INIT(i)	{ (i) }
#define atomic_set(v, i)	(((v)->counter) = i)

#ifdef CONFIG_SMP

#define atomic_read(v)	__raw_uncached_fetch_asm(&(v)->counter)

asmlinkage int __raw_uncached_fetch_asm(const volatile int *ptr);

asmlinkage int __raw_atomic_update_asm(volatile int *ptr, int value);

asmlinkage int __raw_atomic_clear_asm(volatile int *ptr, int value);

asmlinkage int __raw_atomic_set_asm(volatile int *ptr, int value);

asmlinkage int __raw_atomic_xor_asm(volatile int *ptr, int value);

asmlinkage int __raw_atomic_test_asm(const volatile int *ptr, int value);

static inline void atomic_add(int i, atomic_t *v)
{
	__raw_atomic_update_asm(&v->counter, i);
}

static inline void atomic_sub(int i, atomic_t *v)
{
	__raw_atomic_update_asm(&v->counter, -i);
}

static inline int atomic_add_return(int i, atomic_t *v)
{
	return __raw_atomic_update_asm(&v->counter, i);
}

static inline int atomic_sub_return(int i, atomic_t *v)
{
	return __raw_atomic_update_asm(&v->counter, -i);
}

static inline void atomic_inc(volatile atomic_t *v)
{
	__raw_atomic_update_asm(&v->counter, 1);
}

static inline void atomic_dec(volatile atomic_t *v)
{
	__raw_atomic_update_asm(&v->counter, -1);
}

static inline void atomic_clear_mask(int mask, atomic_t *v)
{
	__raw_atomic_clear_asm(&v->counter, mask);
}

static inline void atomic_set_mask(int mask, atomic_t *v)
{
	__raw_atomic_set_asm(&v->counter, mask);
}

static inline int atomic_test_mask(int mask, atomic_t *v)
{
	return __raw_atomic_test_asm(&v->counter, mask);
}

/* Atomic operations are already serializing */
#define smp_mb__before_atomic_dec()    barrier()
#define smp_mb__after_atomic_dec() barrier()
#define smp_mb__before_atomic_inc()    barrier()
#define smp_mb__after_atomic_inc() barrier()

#else /* !CONFIG_SMP */

#define atomic_read(v)	((v)->counter)

static inline void atomic_add(int i, atomic_t *v)
{
	long flags;

	local_irq_save_hw(flags);
	v->counter += i;
	local_irq_restore_hw(flags);
}

static inline void atomic_sub(int i, atomic_t *v)
{
	long flags;

	local_irq_save_hw(flags);
	v->counter -= i;
	local_irq_restore_hw(flags);

}

static inline int atomic_add_return(int i, atomic_t *v)
{
	int __temp = 0;
	long flags;

	local_irq_save_hw(flags);
	v->counter += i;
	__temp = v->counter;
	local_irq_restore_hw(flags);


	return __temp;
}

static inline int atomic_sub_return(int i, atomic_t *v)
{
	int __temp = 0;
	long flags;

	local_irq_save_hw(flags);
	v->counter -= i;
	__temp = v->counter;
	local_irq_restore_hw(flags);

	return __temp;
}

static inline void atomic_inc(volatile atomic_t *v)
{
	long flags;

	local_irq_save_hw(flags);
	v->counter++;
	local_irq_restore_hw(flags);
}

static inline void atomic_dec(volatile atomic_t *v)
{
	long flags;

	local_irq_save_hw(flags);
	v->counter--;
	local_irq_restore_hw(flags);
}

static inline void atomic_clear_mask(unsigned int mask, atomic_t *v)
{
	long flags;

	local_irq_save_hw(flags);
	v->counter &= ~mask;
	local_irq_restore_hw(flags);
}

static inline void atomic_set_mask(unsigned int mask, atomic_t *v)
{
	long flags;

	local_irq_save_hw(flags);
	v->counter |= mask;
	local_irq_restore_hw(flags);
}

/* Atomic operations are already serializing */
#define smp_mb__before_atomic_dec()    barrier()
#define smp_mb__after_atomic_dec() barrier()
#define smp_mb__before_atomic_inc()    barrier()
#define smp_mb__after_atomic_inc() barrier()

#endif /* !CONFIG_SMP */

#define atomic_add_negative(a, v)	(atomic_add_return((a), (v)) < 0)
#define atomic_dec_return(v) atomic_sub_return(1,(v))
#define atomic_inc_return(v) atomic_add_return(1,(v))

#define atomic_cmpxchg(v, o, n) ((int)cmpxchg(&((v)->counter), (o), (n)))
#define atomic_xchg(v, new) (xchg(&((v)->counter), new))

#define atomic_add_unless(v, a, u)				\
({								\
	int c, old;						\
	c = atomic_read(v);					\
	while (c != (u) && (old = atomic_cmpxchg((v), c, c + (a))) != c) \
		c = old;					\
	c != (u);						\
})
#define atomic_inc_not_zero(v) atomic_add_unless((v), 1, 0)

/*
 * atomic_inc_and_test - increment and test
 * @v: pointer of type atomic_t
 *
 * Atomically increments @v by 1
 * and returns true if the result is zero, or false for all
 * other cases.
 */
#define atomic_inc_and_test(v) (atomic_inc_return(v) == 0)

#define atomic_sub_and_test(i,v) (atomic_sub_return((i), (v)) == 0)
#define atomic_dec_and_test(v) (atomic_sub_return(1, (v)) == 0)

#include <asm-generic/atomic.h>

#endif				/* __ARCH_BLACKFIN_ATOMIC __ */
