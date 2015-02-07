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

/* $Id$ */

#ifndef __ASM_CRIS_ATOMIC__
#define __ASM_CRIS_ATOMIC__

#include <linux/compiler.h>
#include <linux/types.h>
#include <asm/system.h>
#include <arch/atomic.h>

/*
 * Atomic operations that C can't guarantee us.  Useful for
 * resource counting etc..
 */

#define ATOMIC_INIT(i)  { (i) }

#define atomic_read(v) ((v)->counter)
#define atomic_set(v,i) (((v)->counter) = (i))

/* These should be written in asm but we do it in C for now. */

static inline void atomic_add(int i, volatile atomic_t *v)
{
	unsigned long flags;
	cris_atomic_save(v, flags);
	v->counter += i;
	cris_atomic_restore(v, flags);
}

static inline void atomic_sub(int i, volatile atomic_t *v)
{
	unsigned long flags;
	cris_atomic_save(v, flags);
	v->counter -= i;
	cris_atomic_restore(v, flags);
}

static inline int atomic_add_return(int i, volatile atomic_t *v)
{
	unsigned long flags;
	int retval;
	cris_atomic_save(v, flags);
	retval = (v->counter += i);
	cris_atomic_restore(v, flags);
	return retval;
}

#define atomic_add_negative(a, v)	(atomic_add_return((a), (v)) < 0)

static inline int atomic_sub_return(int i, volatile atomic_t *v)
{
	unsigned long flags;
	int retval;
	cris_atomic_save(v, flags);
	retval = (v->counter -= i);
	cris_atomic_restore(v, flags);
	return retval;
}

static inline int atomic_sub_and_test(int i, volatile atomic_t *v)
{
	int retval;
	unsigned long flags;
	cris_atomic_save(v, flags);
	retval = (v->counter -= i) == 0;
	cris_atomic_restore(v, flags);
	return retval;
}

static inline void atomic_inc(volatile atomic_t *v)
{
	unsigned long flags;
	cris_atomic_save(v, flags);
	(v->counter)++;
	cris_atomic_restore(v, flags);
}

static inline void atomic_dec(volatile atomic_t *v)
{
	unsigned long flags;
	cris_atomic_save(v, flags);
	(v->counter)--;
	cris_atomic_restore(v, flags);
}

static inline int atomic_inc_return(volatile atomic_t *v)
{
	unsigned long flags;
	int retval;
	cris_atomic_save(v, flags);
	retval = ++(v->counter);
	cris_atomic_restore(v, flags);
	return retval;
}

static inline int atomic_dec_return(volatile atomic_t *v)
{
	unsigned long flags;
	int retval;
	cris_atomic_save(v, flags);
	retval = --(v->counter);
	cris_atomic_restore(v, flags);
	return retval;
}
static inline int atomic_dec_and_test(volatile atomic_t *v)
{
	int retval;
	unsigned long flags;
	cris_atomic_save(v, flags);
	retval = --(v->counter) == 0;
	cris_atomic_restore(v, flags);
	return retval;
}

static inline int atomic_inc_and_test(volatile atomic_t *v)
{
	int retval;
	unsigned long flags;
	cris_atomic_save(v, flags);
	retval = ++(v->counter) == 0;
	cris_atomic_restore(v, flags);
	return retval;
}

static inline int atomic_cmpxchg(atomic_t *v, int old, int new)
{
	int ret;
	unsigned long flags;

	cris_atomic_save(v, flags);
	ret = v->counter;
	if (likely(ret == old))
		v->counter = new;
	cris_atomic_restore(v, flags);
	return ret;
}

#define atomic_xchg(v, new) (xchg(&((v)->counter), new))

static inline int atomic_add_unless(atomic_t *v, int a, int u)
{
	int ret;
	unsigned long flags;

	cris_atomic_save(v, flags);
	ret = v->counter;
	if (ret != u)
		v->counter += a;
	cris_atomic_restore(v, flags);
	return ret != u;
}
#define atomic_inc_not_zero(v) atomic_add_unless((v), 1, 0)

/* Atomic operations are already serializing */
#define smp_mb__before_atomic_dec()    barrier()
#define smp_mb__after_atomic_dec()     barrier()
#define smp_mb__before_atomic_inc()    barrier()
#define smp_mb__after_atomic_inc()     barrier()

#include <asm-generic/atomic.h>
#endif
