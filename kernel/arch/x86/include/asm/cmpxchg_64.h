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

#ifndef _ASM_X86_CMPXCHG_64_H
#define _ASM_X86_CMPXCHG_64_H

#include <asm/alternative.h> /* Provides LOCK_PREFIX */

#define xchg(ptr, v) ((__typeof__(*(ptr)))__xchg((unsigned long)(v), \
						 (ptr), sizeof(*(ptr))))

#define __xg(x) ((volatile long *)(x))

static inline void set_64bit(volatile unsigned long *ptr, unsigned long val)
{
	*ptr = val;
}

#define _set_64bit set_64bit

/*
 * Note: no "lock" prefix even on SMP: xchg always implies lock anyway
 * Note 2: xchg has side effect, so that attribute volatile is necessary,
 *	  but generally the primitive is invalid, *ptr is output argument. --ANK
 */
static inline unsigned long __xchg(unsigned long x, volatile void *ptr,
				   int size)
{
	switch (size) {
	case 1:
		asm volatile("xchgb %b0,%1"
			     : "=q" (x)
			     : "m" (*__xg(ptr)), "0" (x)
			     : "memory");
		break;
	case 2:
		asm volatile("xchgw %w0,%1"
			     : "=r" (x)
			     : "m" (*__xg(ptr)), "0" (x)
			     : "memory");
		break;
	case 4:
		asm volatile("xchgl %k0,%1"
			     : "=r" (x)
			     : "m" (*__xg(ptr)), "0" (x)
			     : "memory");
		break;
	case 8:
		asm volatile("xchgq %0,%1"
			     : "=r" (x)
			     : "m" (*__xg(ptr)), "0" (x)
			     : "memory");
		break;
	}
	return x;
}

/*
 * Atomic compare and exchange.  Compare OLD with MEM, if identical,
 * store NEW in MEM.  Return the initial value in MEM.  Success is
 * indicated by comparing RETURN with OLD.
 */

#define __HAVE_ARCH_CMPXCHG 1

static inline unsigned long __cmpxchg(volatile void *ptr, unsigned long old,
				      unsigned long new, int size)
{
	unsigned long prev;
	switch (size) {
	case 1:
		asm volatile(LOCK_PREFIX "cmpxchgb %b1,%2"
			     : "=a"(prev)
			     : "q"(new), "m"(*__xg(ptr)), "0"(old)
			     : "memory");
		return prev;
	case 2:
		asm volatile(LOCK_PREFIX "cmpxchgw %w1,%2"
			     : "=a"(prev)
			     : "r"(new), "m"(*__xg(ptr)), "0"(old)
			     : "memory");
		return prev;
	case 4:
		asm volatile(LOCK_PREFIX "cmpxchgl %k1,%2"
			     : "=a"(prev)
			     : "r"(new), "m"(*__xg(ptr)), "0"(old)
			     : "memory");
		return prev;
	case 8:
		asm volatile(LOCK_PREFIX "cmpxchgq %1,%2"
			     : "=a"(prev)
			     : "r"(new), "m"(*__xg(ptr)), "0"(old)
			     : "memory");
		return prev;
	}
	return old;
}

/*
 * Always use locked operations when touching memory shared with a
 * hypervisor, since the system may be SMP even if the guest kernel
 * isn't.
 */
static inline unsigned long __sync_cmpxchg(volatile void *ptr,
					   unsigned long old,
					   unsigned long new, int size)
{
	unsigned long prev;
	switch (size) {
	case 1:
		asm volatile("lock; cmpxchgb %b1,%2"
			     : "=a"(prev)
			     : "q"(new), "m"(*__xg(ptr)), "0"(old)
			     : "memory");
		return prev;
	case 2:
		asm volatile("lock; cmpxchgw %w1,%2"
			     : "=a"(prev)
			     : "r"(new), "m"(*__xg(ptr)), "0"(old)
			     : "memory");
		return prev;
	case 4:
		asm volatile("lock; cmpxchgl %1,%2"
			     : "=a"(prev)
			     : "r"(new), "m"(*__xg(ptr)), "0"(old)
			     : "memory");
		return prev;
	}
	return old;
}

static inline unsigned long __cmpxchg_local(volatile void *ptr,
					    unsigned long old,
					    unsigned long new, int size)
{
	unsigned long prev;
	switch (size) {
	case 1:
		asm volatile("cmpxchgb %b1,%2"
			     : "=a"(prev)
			     : "q"(new), "m"(*__xg(ptr)), "0"(old)
			     : "memory");
		return prev;
	case 2:
		asm volatile("cmpxchgw %w1,%2"
			     : "=a"(prev)
			     : "r"(new), "m"(*__xg(ptr)), "0"(old)
			     : "memory");
		return prev;
	case 4:
		asm volatile("cmpxchgl %k1,%2"
			     : "=a"(prev)
			     : "r"(new), "m"(*__xg(ptr)), "0"(old)
			     : "memory");
		return prev;
	case 8:
		asm volatile("cmpxchgq %1,%2"
			     : "=a"(prev)
			     : "r"(new), "m"(*__xg(ptr)), "0"(old)
			     : "memory");
		return prev;
	}
	return old;
}

#define cmpxchg(ptr, o, n)						\
	((__typeof__(*(ptr)))__cmpxchg((ptr), (unsigned long)(o),	\
				       (unsigned long)(n), sizeof(*(ptr))))
#define cmpxchg64(ptr, o, n)						\
({									\
	BUILD_BUG_ON(sizeof(*(ptr)) != 8);				\
	cmpxchg((ptr), (o), (n));					\
})
#define cmpxchg_local(ptr, o, n)					\
	((__typeof__(*(ptr)))__cmpxchg_local((ptr), (unsigned long)(o),	\
					     (unsigned long)(n),	\
					     sizeof(*(ptr))))
#define sync_cmpxchg(ptr, o, n)						\
	((__typeof__(*(ptr)))__sync_cmpxchg((ptr), (unsigned long)(o),	\
					    (unsigned long)(n),		\
					    sizeof(*(ptr))))
#define cmpxchg64_local(ptr, o, n)					\
({									\
	BUILD_BUG_ON(sizeof(*(ptr)) != 8);				\
	cmpxchg_local((ptr), (o), (n));					\
})

#endif /* _ASM_X86_CMPXCHG_64_H */
