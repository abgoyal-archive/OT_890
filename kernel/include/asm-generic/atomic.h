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

#ifndef _ASM_GENERIC_ATOMIC_H
#define _ASM_GENERIC_ATOMIC_H
/*
 * Copyright (C) 2005 Silicon Graphics, Inc.
 *	Christoph Lameter
 *
 * Allows to provide arch independent atomic definitions without the need to
 * edit all arch specific atomic.h files.
 */

#include <asm/types.h>

/*
 * Suppport for atomic_long_t
 *
 * Casts for parameters are avoided for existing atomic functions in order to
 * avoid issues with cast-as-lval under gcc 4.x and other limitations that the
 * macros of a platform may have.
 */

#if BITS_PER_LONG == 64

typedef atomic64_t atomic_long_t;

#define ATOMIC_LONG_INIT(i)	ATOMIC64_INIT(i)

static inline long atomic_long_read(atomic_long_t *l)
{
	atomic64_t *v = (atomic64_t *)l;

	return (long)atomic64_read(v);
}

static inline void atomic_long_set(atomic_long_t *l, long i)
{
	atomic64_t *v = (atomic64_t *)l;

	atomic64_set(v, i);
}

static inline void atomic_long_inc(atomic_long_t *l)
{
	atomic64_t *v = (atomic64_t *)l;

	atomic64_inc(v);
}

static inline void atomic_long_dec(atomic_long_t *l)
{
	atomic64_t *v = (atomic64_t *)l;

	atomic64_dec(v);
}

static inline void atomic_long_add(long i, atomic_long_t *l)
{
	atomic64_t *v = (atomic64_t *)l;

	atomic64_add(i, v);
}

static inline void atomic_long_sub(long i, atomic_long_t *l)
{
	atomic64_t *v = (atomic64_t *)l;

	atomic64_sub(i, v);
}

static inline int atomic_long_sub_and_test(long i, atomic_long_t *l)
{
	atomic64_t *v = (atomic64_t *)l;

	return atomic64_sub_and_test(i, v);
}

static inline int atomic_long_dec_and_test(atomic_long_t *l)
{
	atomic64_t *v = (atomic64_t *)l;

	return atomic64_dec_and_test(v);
}

static inline int atomic_long_inc_and_test(atomic_long_t *l)
{
	atomic64_t *v = (atomic64_t *)l;

	return atomic64_inc_and_test(v);
}

static inline int atomic_long_add_negative(long i, atomic_long_t *l)
{
	atomic64_t *v = (atomic64_t *)l;

	return atomic64_add_negative(i, v);
}

static inline long atomic_long_add_return(long i, atomic_long_t *l)
{
	atomic64_t *v = (atomic64_t *)l;

	return (long)atomic64_add_return(i, v);
}

static inline long atomic_long_sub_return(long i, atomic_long_t *l)
{
	atomic64_t *v = (atomic64_t *)l;

	return (long)atomic64_sub_return(i, v);
}

static inline long atomic_long_inc_return(atomic_long_t *l)
{
	atomic64_t *v = (atomic64_t *)l;

	return (long)atomic64_inc_return(v);
}

static inline long atomic_long_dec_return(atomic_long_t *l)
{
	atomic64_t *v = (atomic64_t *)l;

	return (long)atomic64_dec_return(v);
}

static inline long atomic_long_add_unless(atomic_long_t *l, long a, long u)
{
	atomic64_t *v = (atomic64_t *)l;

	return (long)atomic64_add_unless(v, a, u);
}

#define atomic_long_inc_not_zero(l) atomic64_inc_not_zero((atomic64_t *)(l))

#define atomic_long_cmpxchg(l, old, new) \
	(atomic_cmpxchg((atomic64_t *)(l), (old), (new)))
#define atomic_long_xchg(v, new) \
	(atomic_xchg((atomic64_t *)(l), (new)))

#else  /*  BITS_PER_LONG == 64  */

typedef atomic_t atomic_long_t;

#define ATOMIC_LONG_INIT(i)	ATOMIC_INIT(i)
static inline long atomic_long_read(atomic_long_t *l)
{
	atomic_t *v = (atomic_t *)l;

	return (long)atomic_read(v);
}

static inline void atomic_long_set(atomic_long_t *l, long i)
{
	atomic_t *v = (atomic_t *)l;

	atomic_set(v, i);
}

static inline void atomic_long_inc(atomic_long_t *l)
{
	atomic_t *v = (atomic_t *)l;

	atomic_inc(v);
}

static inline void atomic_long_dec(atomic_long_t *l)
{
	atomic_t *v = (atomic_t *)l;

	atomic_dec(v);
}

static inline void atomic_long_add(long i, atomic_long_t *l)
{
	atomic_t *v = (atomic_t *)l;

	atomic_add(i, v);
}

static inline void atomic_long_sub(long i, atomic_long_t *l)
{
	atomic_t *v = (atomic_t *)l;

	atomic_sub(i, v);
}

static inline int atomic_long_sub_and_test(long i, atomic_long_t *l)
{
	atomic_t *v = (atomic_t *)l;

	return atomic_sub_and_test(i, v);
}

static inline int atomic_long_dec_and_test(atomic_long_t *l)
{
	atomic_t *v = (atomic_t *)l;

	return atomic_dec_and_test(v);
}

static inline int atomic_long_inc_and_test(atomic_long_t *l)
{
	atomic_t *v = (atomic_t *)l;

	return atomic_inc_and_test(v);
}

static inline int atomic_long_add_negative(long i, atomic_long_t *l)
{
	atomic_t *v = (atomic_t *)l;

	return atomic_add_negative(i, v);
}

static inline long atomic_long_add_return(long i, atomic_long_t *l)
{
	atomic_t *v = (atomic_t *)l;

	return (long)atomic_add_return(i, v);
}

static inline long atomic_long_sub_return(long i, atomic_long_t *l)
{
	atomic_t *v = (atomic_t *)l;

	return (long)atomic_sub_return(i, v);
}

static inline long atomic_long_inc_return(atomic_long_t *l)
{
	atomic_t *v = (atomic_t *)l;

	return (long)atomic_inc_return(v);
}

static inline long atomic_long_dec_return(atomic_long_t *l)
{
	atomic_t *v = (atomic_t *)l;

	return (long)atomic_dec_return(v);
}

static inline long atomic_long_add_unless(atomic_long_t *l, long a, long u)
{
	atomic_t *v = (atomic_t *)l;

	return (long)atomic_add_unless(v, a, u);
}

#define atomic_long_inc_not_zero(l) atomic_inc_not_zero((atomic_t *)(l))

#define atomic_long_cmpxchg(l, old, new) \
	(atomic_cmpxchg((atomic_t *)(l), (old), (new)))
#define atomic_long_xchg(v, new) \
	(atomic_xchg((atomic_t *)(v), (new)))

#endif  /*  BITS_PER_LONG == 64  */

#endif  /*  _ASM_GENERIC_ATOMIC_H  */
