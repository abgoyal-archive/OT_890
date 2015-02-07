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
 * Assembly implementation of the mutex fastpath, based on atomic
 * decrement/increment.
 *
 * started by Ingo Molnar:
 *
 *  Copyright (C) 2004, 2005, 2006 Red Hat, Inc., Ingo Molnar <mingo@redhat.com>
 */
#ifndef _ASM_X86_MUTEX_64_H
#define _ASM_X86_MUTEX_64_H

/**
 * __mutex_fastpath_lock - decrement and call function if negative
 * @v: pointer of type atomic_t
 * @fail_fn: function to call if the result is negative
 *
 * Atomically decrements @v and calls <fail_fn> if the result is negative.
 */
#define __mutex_fastpath_lock(v, fail_fn)			\
do {								\
	unsigned long dummy;					\
								\
	typecheck(atomic_t *, v);				\
	typecheck_fn(void (*)(atomic_t *), fail_fn);		\
								\
	asm volatile(LOCK_PREFIX "   decl (%%rdi)\n"		\
		     "   jns 1f		\n"			\
		     "   call " #fail_fn "\n"			\
		     "1:"					\
		     : "=D" (dummy)				\
		     : "D" (v)					\
		     : "rax", "rsi", "rdx", "rcx",		\
		       "r8", "r9", "r10", "r11", "memory");	\
} while (0)

/**
 *  __mutex_fastpath_lock_retval - try to take the lock by moving the count
 *                                 from 1 to a 0 value
 *  @count: pointer of type atomic_t
 *  @fail_fn: function to call if the original value was not 1
 *
 * Change the count from 1 to a value lower than 1, and call <fail_fn> if
 * it wasn't 1 originally. This function returns 0 if the fastpath succeeds,
 * or anything the slow path function returns
 */
static inline int __mutex_fastpath_lock_retval(atomic_t *count,
					       int (*fail_fn)(atomic_t *))
{
	if (unlikely(atomic_dec_return(count) < 0))
		return fail_fn(count);
	else
		return 0;
}

/**
 * __mutex_fastpath_unlock - increment and call function if nonpositive
 * @v: pointer of type atomic_t
 * @fail_fn: function to call if the result is nonpositive
 *
 * Atomically increments @v and calls <fail_fn> if the result is nonpositive.
 */
#define __mutex_fastpath_unlock(v, fail_fn)			\
do {								\
	unsigned long dummy;					\
								\
	typecheck(atomic_t *, v);				\
	typecheck_fn(void (*)(atomic_t *), fail_fn);		\
								\
	asm volatile(LOCK_PREFIX "   incl (%%rdi)\n"		\
		     "   jg 1f\n"				\
		     "   call " #fail_fn "\n"			\
		     "1:"					\
		     : "=D" (dummy)				\
		     : "D" (v)					\
		     : "rax", "rsi", "rdx", "rcx",		\
		       "r8", "r9", "r10", "r11", "memory");	\
} while (0)

#define __mutex_slowpath_needs_to_unlock()	1

/**
 * __mutex_fastpath_trylock - try to acquire the mutex, without waiting
 *
 *  @count: pointer of type atomic_t
 *  @fail_fn: fallback function
 *
 * Change the count from 1 to 0 and return 1 (success), or return 0 (failure)
 * if it wasn't 1 originally. [the fallback function is never used on
 * x86_64, because all x86_64 CPUs have a CMPXCHG instruction.]
 */
static inline int __mutex_fastpath_trylock(atomic_t *count,
					   int (*fail_fn)(atomic_t *))
{
	if (likely(atomic_cmpxchg(count, 1, 0) == 1))
		return 1;
	else
		return 0;
}

#endif /* _ASM_X86_MUTEX_64_H */
