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
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 *
 * Copyright (C) 2003, 06, 07 by Ralf Baechle (ralf@linux-mips.org)
 */
#ifndef __ASM_CMPXCHG_H
#define __ASM_CMPXCHG_H

#include <linux/irqflags.h>

#define __HAVE_ARCH_CMPXCHG 1

#define __cmpxchg_asm(ld, st, m, old, new)				\
({									\
	__typeof(*(m)) __ret;						\
									\
	if (cpu_has_llsc && R10000_LLSC_WAR) {				\
		__asm__ __volatile__(					\
		"	.set	push				\n"	\
		"	.set	noat				\n"	\
		"	.set	mips3				\n"	\
		"1:	" ld "	%0, %2		# __cmpxchg_asm	\n"	\
		"	bne	%0, %z3, 2f			\n"	\
		"	.set	mips0				\n"	\
		"	move	$1, %z4				\n"	\
		"	.set	mips3				\n"	\
		"	" st "	$1, %1				\n"	\
		"	beqzl	$1, 1b				\n"	\
		"2:						\n"	\
		"	.set	pop				\n"	\
		: "=&r" (__ret), "=R" (*m)				\
		: "R" (*m), "Jr" (old), "Jr" (new)			\
		: "memory");						\
	} else if (cpu_has_llsc) {					\
		__asm__ __volatile__(					\
		"	.set	push				\n"	\
		"	.set	noat				\n"	\
		"	.set	mips3				\n"	\
		"1:	" ld "	%0, %2		# __cmpxchg_asm	\n"	\
		"	bne	%0, %z3, 2f			\n"	\
		"	.set	mips0				\n"	\
		"	move	$1, %z4				\n"	\
		"	.set	mips3				\n"	\
		"	" st "	$1, %1				\n"	\
		"	beqz	$1, 3f				\n"	\
		"2:						\n"	\
		"	.subsection 2				\n"	\
		"3:	b	1b				\n"	\
		"	.previous				\n"	\
		"	.set	pop				\n"	\
		: "=&r" (__ret), "=R" (*m)				\
		: "R" (*m), "Jr" (old), "Jr" (new)			\
		: "memory");						\
	} else {							\
		unsigned long __flags;					\
									\
		raw_local_irq_save(__flags);				\
		__ret = *m;						\
		if (__ret == old)					\
			*m = new;					\
		raw_local_irq_restore(__flags);				\
	}								\
									\
	__ret;								\
})

/*
 * This function doesn't exist, so you'll get a linker error
 * if something tries to do an invalid cmpxchg().
 */
extern void __cmpxchg_called_with_bad_pointer(void);

#define __cmpxchg(ptr, old, new, barrier)				\
({									\
	__typeof__(ptr) __ptr = (ptr);					\
	__typeof__(*(ptr)) __old = (old);				\
	__typeof__(*(ptr)) __new = (new);				\
	__typeof__(*(ptr)) __res = 0;					\
									\
	barrier;							\
									\
	switch (sizeof(*(__ptr))) {					\
	case 4:								\
		__res = __cmpxchg_asm("ll", "sc", __ptr, __old, __new);	\
		break;							\
	case 8:								\
		if (sizeof(long) == 8) {				\
			__res = __cmpxchg_asm("lld", "scd", __ptr,	\
					   __old, __new);		\
			break;						\
		}							\
	default:							\
		__cmpxchg_called_with_bad_pointer();			\
		break;							\
	}								\
									\
	barrier;							\
									\
	__res;								\
})

#define cmpxchg(ptr, old, new)		__cmpxchg(ptr, old, new, smp_llsc_mb())
#define cmpxchg_local(ptr, old, new)	__cmpxchg(ptr, old, new, )

#define cmpxchg64(ptr, o, n)						\
  ({									\
	BUILD_BUG_ON(sizeof(*(ptr)) != 8);				\
	cmpxchg((ptr), (o), (n));					\
  })

#ifdef CONFIG_64BIT
#define cmpxchg64_local(ptr, o, n)					\
  ({									\
	BUILD_BUG_ON(sizeof(*(ptr)) != 8);				\
	cmpxchg_local((ptr), (o), (n));					\
  })
#else
#include <asm-generic/cmpxchg-local.h>
#define cmpxchg64_local(ptr, o, n) __cmpxchg64_local_generic((ptr), (o), (n))
#endif

#endif /* __ASM_CMPXCHG_H */
