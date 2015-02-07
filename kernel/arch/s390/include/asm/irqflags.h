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
 *  include/asm-s390/irqflags.h
 *
 *    Copyright (C) IBM Corp. 2006
 *    Author(s): Heiko Carstens <heiko.carstens@de.ibm.com>
 */

#ifndef __ASM_IRQFLAGS_H
#define __ASM_IRQFLAGS_H

#ifdef __KERNEL__

#if __GNUC__ > 3 || (__GNUC__ == 3 && __GNUC_MINOR__ > 2)

/* store then or system mask. */
#define __raw_local_irq_stosm(__or)					\
({									\
	unsigned long __mask;						\
	asm volatile(							\
		"	stosm	%0,%1"					\
		: "=Q" (__mask) : "i" (__or) : "memory");		\
	__mask;								\
})

/* store then and system mask. */
#define __raw_local_irq_stnsm(__and)					\
({									\
	unsigned long __mask;						\
	asm volatile(							\
		"	stnsm	%0,%1"					\
		: "=Q" (__mask) : "i" (__and) : "memory");		\
	__mask;								\
})

/* set system mask. */
#define __raw_local_irq_ssm(__mask)					\
({									\
	asm volatile("ssm   %0" : : "Q" (__mask) : "memory");		\
})

#else /* __GNUC__ */

/* store then or system mask. */
#define __raw_local_irq_stosm(__or)					\
({									\
	unsigned long __mask;						\
	asm volatile(							\
		"	stosm	0(%1),%2"				\
		: "=m" (__mask)						\
		: "a" (&__mask), "i" (__or) : "memory");		\
	__mask;								\
})

/* store then and system mask. */
#define __raw_local_irq_stnsm(__and)					\
({									\
	unsigned long __mask;						\
	asm volatile(							\
		"	stnsm	0(%1),%2"				\
		: "=m" (__mask)						\
		: "a" (&__mask), "i" (__and) : "memory");		\
	__mask;								\
})

/* set system mask. */
#define __raw_local_irq_ssm(__mask)					\
({									\
	asm volatile(							\
		"	ssm	0(%0)"					\
		: : "a" (&__mask), "m" (__mask) : "memory");		\
})

#endif /* __GNUC__ */

/* interrupt control.. */
static inline unsigned long raw_local_irq_enable(void)
{
	return __raw_local_irq_stosm(0x03);
}

static inline unsigned long raw_local_irq_disable(void)
{
	return __raw_local_irq_stnsm(0xfc);
}

#define raw_local_save_flags(x)						\
do {									\
	typecheck(unsigned long, x);					\
	(x) = __raw_local_irq_stosm(0x00);				\
} while (0)

static inline void raw_local_irq_restore(unsigned long flags)
{
	__raw_local_irq_ssm(flags);
}

static inline int raw_irqs_disabled_flags(unsigned long flags)
{
	return !(flags & (3UL << (BITS_PER_LONG - 8)));
}

/* For spinlocks etc */
#define raw_local_irq_save(x)	((x) = raw_local_irq_disable())

#endif /* __KERNEL__ */
#endif /* __ASM_IRQFLAGS_H */
