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

#ifndef __ARCH_M68K_ATOMIC__
#define __ARCH_M68K_ATOMIC__

#include <linux/types.h>
#include <asm/system.h>

/*
 * Atomic operations that C can't guarantee us.  Useful for
 * resource counting etc..
 */

/*
 * We do not have SMP m68k systems, so we don't have to deal with that.
 */

#define ATOMIC_INIT(i)	{ (i) }

#define atomic_read(v)		((v)->counter)
#define atomic_set(v, i)	(((v)->counter) = i)

static inline void atomic_add(int i, atomic_t *v)
{
	__asm__ __volatile__("addl %1,%0" : "+m" (*v) : "id" (i));
}

static inline void atomic_sub(int i, atomic_t *v)
{
	__asm__ __volatile__("subl %1,%0" : "+m" (*v) : "id" (i));
}

static inline void atomic_inc(atomic_t *v)
{
	__asm__ __volatile__("addql #1,%0" : "+m" (*v));
}

static inline void atomic_dec(atomic_t *v)
{
	__asm__ __volatile__("subql #1,%0" : "+m" (*v));
}

static inline int atomic_dec_and_test(atomic_t *v)
{
	char c;
	__asm__ __volatile__("subql #1,%1; seq %0" : "=d" (c), "+m" (*v));
	return c != 0;
}

static inline int atomic_inc_and_test(atomic_t *v)
{
	char c;
	__asm__ __volatile__("addql #1,%1; seq %0" : "=d" (c), "+m" (*v));
	return c != 0;
}

#ifdef CONFIG_RMW_INSNS

static inline int atomic_add_return(int i, atomic_t *v)
{
	int t, tmp;

	__asm__ __volatile__(
			"1:	movel %2,%1\n"
			"	addl %3,%1\n"
			"	casl %2,%1,%0\n"
			"	jne 1b"
			: "+m" (*v), "=&d" (t), "=&d" (tmp)
			: "g" (i), "2" (atomic_read(v)));
	return t;
}

static inline int atomic_sub_return(int i, atomic_t *v)
{
	int t, tmp;

	__asm__ __volatile__(
			"1:	movel %2,%1\n"
			"	subl %3,%1\n"
			"	casl %2,%1,%0\n"
			"	jne 1b"
			: "+m" (*v), "=&d" (t), "=&d" (tmp)
			: "g" (i), "2" (atomic_read(v)));
	return t;
}

#define atomic_cmpxchg(v, o, n) ((int)cmpxchg(&((v)->counter), (o), (n)))
#define atomic_xchg(v, new) (xchg(&((v)->counter), new))

#else /* !CONFIG_RMW_INSNS */

static inline int atomic_add_return(int i, atomic_t * v)
{
	unsigned long flags;
	int t;

	local_irq_save(flags);
	t = atomic_read(v);
	t += i;
	atomic_set(v, t);
	local_irq_restore(flags);

	return t;
}

static inline int atomic_sub_return(int i, atomic_t * v)
{
	unsigned long flags;
	int t;

	local_irq_save(flags);
	t = atomic_read(v);
	t -= i;
	atomic_set(v, t);
	local_irq_restore(flags);

	return t;
}

static inline int atomic_cmpxchg(atomic_t *v, int old, int new)
{
	unsigned long flags;
	int prev;

	local_irq_save(flags);
	prev = atomic_read(v);
	if (prev == old)
		atomic_set(v, new);
	local_irq_restore(flags);
	return prev;
}

static inline int atomic_xchg(atomic_t *v, int new)
{
	unsigned long flags;
	int prev;

	local_irq_save(flags);
	prev = atomic_read(v);
	atomic_set(v, new);
	local_irq_restore(flags);
	return prev;
}

#endif /* !CONFIG_RMW_INSNS */

#define atomic_dec_return(v)	atomic_sub_return(1, (v))
#define atomic_inc_return(v)	atomic_add_return(1, (v))

static inline int atomic_sub_and_test(int i, atomic_t *v)
{
	char c;
	__asm__ __volatile__("subl %2,%1; seq %0" : "=d" (c), "+m" (*v): "g" (i));
	return c != 0;
}

static inline int atomic_add_negative(int i, atomic_t *v)
{
	char c;
	__asm__ __volatile__("addl %2,%1; smi %0" : "=d" (c), "+m" (*v): "g" (i));
	return c != 0;
}

static inline void atomic_clear_mask(unsigned long mask, unsigned long *v)
{
	__asm__ __volatile__("andl %1,%0" : "+m" (*v) : "id" (~(mask)));
}

static inline void atomic_set_mask(unsigned long mask, unsigned long *v)
{
	__asm__ __volatile__("orl %1,%0" : "+m" (*v) : "id" (mask));
}

static __inline__ int atomic_add_unless(atomic_t *v, int a, int u)
{
	int c, old;
	c = atomic_read(v);
	for (;;) {
		if (unlikely(c == (u)))
			break;
		old = atomic_cmpxchg((v), c, c + (a));
		if (likely(old == c))
			break;
		c = old;
	}
	return c != (u);
}

#define atomic_inc_not_zero(v) atomic_add_unless((v), 1, 0)

/* Atomic operations are already serializing */
#define smp_mb__before_atomic_dec()	barrier()
#define smp_mb__after_atomic_dec()	barrier()
#define smp_mb__before_atomic_inc()	barrier()
#define smp_mb__after_atomic_inc()	barrier()

#include <asm-generic/atomic.h>
#endif /* __ARCH_M68K_ATOMIC __ */
