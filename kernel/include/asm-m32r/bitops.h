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

#ifndef _ASM_M32R_BITOPS_H
#define _ASM_M32R_BITOPS_H

/*
 *  linux/include/asm-m32r/bitops.h
 *
 *  Copyright 1992, Linus Torvalds.
 *
 *  M32R version:
 *    Copyright (C) 2001, 2002  Hitoshi Yamamoto
 *    Copyright (C) 2004  Hirokazu Takata <takata at linux-m32r.org>
 */

#ifndef _LINUX_BITOPS_H
#error only <linux/bitops.h> can be included directly
#endif

#include <linux/compiler.h>
#include <asm/assembler.h>
#include <asm/system.h>
#include <asm/byteorder.h>
#include <asm/types.h>

/*
 * These have to be done with inline assembly: that way the bit-setting
 * is guaranteed to be atomic. All bit operations return 0 if the bit
 * was cleared before the operation and != 0 if it was not.
 *
 * bit 0 is the LSB of addr; bit 32 is the LSB of (addr+1).
 */

/**
 * set_bit - Atomically set a bit in memory
 * @nr: the bit to set
 * @addr: the address to start counting from
 *
 * This function is atomic and may not be reordered.  See __set_bit()
 * if you do not require the atomic guarantees.
 * Note that @nr may be almost arbitrarily large; this function is not
 * restricted to acting on a single-word quantity.
 */
static __inline__ void set_bit(int nr, volatile void * addr)
{
	__u32 mask;
	volatile __u32 *a = addr;
	unsigned long flags;
	unsigned long tmp;

	a += (nr >> 5);
	mask = (1 << (nr & 0x1F));

	local_irq_save(flags);
	__asm__ __volatile__ (
		DCACHE_CLEAR("%0", "r6", "%1")
		M32R_LOCK" %0, @%1;		\n\t"
		"or	%0, %2;			\n\t"
		M32R_UNLOCK" %0, @%1;		\n\t"
		: "=&r" (tmp)
		: "r" (a), "r" (mask)
		: "memory"
#ifdef CONFIG_CHIP_M32700_TS1
		, "r6"
#endif	/* CONFIG_CHIP_M32700_TS1 */
	);
	local_irq_restore(flags);
}

/**
 * clear_bit - Clears a bit in memory
 * @nr: Bit to clear
 * @addr: Address to start counting from
 *
 * clear_bit() is atomic and may not be reordered.  However, it does
 * not contain a memory barrier, so if it is used for locking purposes,
 * you should call smp_mb__before_clear_bit() and/or smp_mb__after_clear_bit()
 * in order to ensure changes are visible on other processors.
 */
static __inline__ void clear_bit(int nr, volatile void * addr)
{
	__u32 mask;
	volatile __u32 *a = addr;
	unsigned long flags;
	unsigned long tmp;

	a += (nr >> 5);
	mask = (1 << (nr & 0x1F));

	local_irq_save(flags);

	__asm__ __volatile__ (
		DCACHE_CLEAR("%0", "r6", "%1")
		M32R_LOCK" %0, @%1;		\n\t"
		"and	%0, %2;			\n\t"
		M32R_UNLOCK" %0, @%1;		\n\t"
		: "=&r" (tmp)
		: "r" (a), "r" (~mask)
		: "memory"
#ifdef CONFIG_CHIP_M32700_TS1
		, "r6"
#endif	/* CONFIG_CHIP_M32700_TS1 */
	);
	local_irq_restore(flags);
}

#define smp_mb__before_clear_bit()	barrier()
#define smp_mb__after_clear_bit()	barrier()

/**
 * change_bit - Toggle a bit in memory
 * @nr: Bit to clear
 * @addr: Address to start counting from
 *
 * change_bit() is atomic and may not be reordered.
 * Note that @nr may be almost arbitrarily large; this function is not
 * restricted to acting on a single-word quantity.
 */
static __inline__ void change_bit(int nr, volatile void * addr)
{
	__u32  mask;
	volatile __u32  *a = addr;
	unsigned long flags;
	unsigned long tmp;

	a += (nr >> 5);
	mask = (1 << (nr & 0x1F));

	local_irq_save(flags);
	__asm__ __volatile__ (
		DCACHE_CLEAR("%0", "r6", "%1")
		M32R_LOCK" %0, @%1;		\n\t"
		"xor	%0, %2;			\n\t"
		M32R_UNLOCK" %0, @%1;		\n\t"
		: "=&r" (tmp)
		: "r" (a), "r" (mask)
		: "memory"
#ifdef CONFIG_CHIP_M32700_TS1
		, "r6"
#endif	/* CONFIG_CHIP_M32700_TS1 */
	);
	local_irq_restore(flags);
}

/**
 * test_and_set_bit - Set a bit and return its old value
 * @nr: Bit to set
 * @addr: Address to count from
 *
 * This operation is atomic and cannot be reordered.
 * It also implies a memory barrier.
 */
static __inline__ int test_and_set_bit(int nr, volatile void * addr)
{
	__u32 mask, oldbit;
	volatile __u32 *a = addr;
	unsigned long flags;
	unsigned long tmp;

	a += (nr >> 5);
	mask = (1 << (nr & 0x1F));

	local_irq_save(flags);
	__asm__ __volatile__ (
		DCACHE_CLEAR("%0", "%1", "%2")
		M32R_LOCK" %0, @%2;		\n\t"
		"mv	%1, %0;			\n\t"
		"and	%0, %3;			\n\t"
		"or	%1, %3;			\n\t"
		M32R_UNLOCK" %1, @%2;		\n\t"
		: "=&r" (oldbit), "=&r" (tmp)
		: "r" (a), "r" (mask)
		: "memory"
	);
	local_irq_restore(flags);

	return (oldbit != 0);
}

/**
 * test_and_clear_bit - Clear a bit and return its old value
 * @nr: Bit to set
 * @addr: Address to count from
 *
 * This operation is atomic and cannot be reordered.
 * It also implies a memory barrier.
 */
static __inline__ int test_and_clear_bit(int nr, volatile void * addr)
{
	__u32 mask, oldbit;
	volatile __u32 *a = addr;
	unsigned long flags;
	unsigned long tmp;

	a += (nr >> 5);
	mask = (1 << (nr & 0x1F));

	local_irq_save(flags);

	__asm__ __volatile__ (
		DCACHE_CLEAR("%0", "%1", "%3")
		M32R_LOCK" %0, @%3;		\n\t"
		"mv	%1, %0;			\n\t"
		"and	%0, %2;			\n\t"
		"not	%2, %2;			\n\t"
		"and	%1, %2;			\n\t"
		M32R_UNLOCK" %1, @%3;		\n\t"
		: "=&r" (oldbit), "=&r" (tmp), "+r" (mask)
		: "r" (a)
		: "memory"
	);
	local_irq_restore(flags);

	return (oldbit != 0);
}

/**
 * test_and_change_bit - Change a bit and return its old value
 * @nr: Bit to set
 * @addr: Address to count from
 *
 * This operation is atomic and cannot be reordered.
 * It also implies a memory barrier.
 */
static __inline__ int test_and_change_bit(int nr, volatile void * addr)
{
	__u32 mask, oldbit;
	volatile __u32 *a = addr;
	unsigned long flags;
	unsigned long tmp;

	a += (nr >> 5);
	mask = (1 << (nr & 0x1F));

	local_irq_save(flags);
	__asm__ __volatile__ (
		DCACHE_CLEAR("%0", "%1", "%2")
		M32R_LOCK" %0, @%2;		\n\t"
		"mv	%1, %0;			\n\t"
		"and	%0, %3;			\n\t"
		"xor	%1, %3;			\n\t"
		M32R_UNLOCK" %1, @%2;		\n\t"
		: "=&r" (oldbit), "=&r" (tmp)
		: "r" (a), "r" (mask)
		: "memory"
	);
	local_irq_restore(flags);

	return (oldbit != 0);
}

#include <asm-generic/bitops/non-atomic.h>
#include <asm-generic/bitops/ffz.h>
#include <asm-generic/bitops/__ffs.h>
#include <asm-generic/bitops/fls.h>
#include <asm-generic/bitops/__fls.h>
#include <asm-generic/bitops/fls64.h>

#ifdef __KERNEL__

#include <asm-generic/bitops/sched.h>
#include <asm-generic/bitops/find.h>
#include <asm-generic/bitops/ffs.h>
#include <asm-generic/bitops/hweight.h>
#include <asm-generic/bitops/lock.h>

#endif /* __KERNEL__ */

#ifdef __KERNEL__

#include <asm-generic/bitops/ext2-non-atomic.h>
#include <asm-generic/bitops/ext2-atomic.h>
#include <asm-generic/bitops/minix.h>

#endif /* __KERNEL__ */

#endif /* _ASM_M32R_BITOPS_H */
