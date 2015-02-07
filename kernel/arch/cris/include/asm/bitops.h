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

/* asm/bitops.h for Linux/CRIS
 *
 * TODO: asm versions if speed is needed
 *
 * All bit operations return 0 if the bit was cleared before the
 * operation and != 0 if it was not.
 *
 * bit 0 is the LSB of addr; bit 32 is the LSB of (addr+1).
 */

#ifndef _CRIS_BITOPS_H
#define _CRIS_BITOPS_H

/* Currently this is unsuitable for consumption outside the kernel.  */
#ifdef __KERNEL__ 

#ifndef _LINUX_BITOPS_H
#error only <linux/bitops.h> can be included directly
#endif

#include <arch/bitops.h>
#include <asm/system.h>
#include <asm/atomic.h>
#include <linux/compiler.h>

/*
 * set_bit - Atomically set a bit in memory
 * @nr: the bit to set
 * @addr: the address to start counting from
 *
 * This function is atomic and may not be reordered.  See __set_bit()
 * if you do not require the atomic guarantees.
 * Note that @nr may be almost arbitrarily large; this function is not
 * restricted to acting on a single-word quantity.
 */

#define set_bit(nr, addr)    (void)test_and_set_bit(nr, addr)

/*
 * clear_bit - Clears a bit in memory
 * @nr: Bit to clear
 * @addr: Address to start counting from
 *
 * clear_bit() is atomic and may not be reordered.  However, it does
 * not contain a memory barrier, so if it is used for locking purposes,
 * you should call smp_mb__before_clear_bit() and/or smp_mb__after_clear_bit()
 * in order to ensure changes are visible on other processors.
 */

#define clear_bit(nr, addr)  (void)test_and_clear_bit(nr, addr)

/*
 * change_bit - Toggle a bit in memory
 * @nr: Bit to change
 * @addr: Address to start counting from
 *
 * change_bit() is atomic and may not be reordered.
 * Note that @nr may be almost arbitrarily large; this function is not
 * restricted to acting on a single-word quantity.
 */

#define change_bit(nr, addr) (void)test_and_change_bit(nr, addr)

/**
 * test_and_set_bit - Set a bit and return its old value
 * @nr: Bit to set
 * @addr: Address to count from
 *
 * This operation is atomic and cannot be reordered.  
 * It also implies a memory barrier.
 */

static inline int test_and_set_bit(int nr, volatile unsigned long *addr)
{
	unsigned int mask, retval;
	unsigned long flags;
	unsigned int *adr = (unsigned int *)addr;
	
	adr += nr >> 5;
	mask = 1 << (nr & 0x1f);
	cris_atomic_save(addr, flags);
	retval = (mask & *adr) != 0;
	*adr |= mask;
	cris_atomic_restore(addr, flags);
	return retval;
}

/*
 * clear_bit() doesn't provide any barrier for the compiler.
 */
#define smp_mb__before_clear_bit()      barrier()
#define smp_mb__after_clear_bit()       barrier()

/**
 * test_and_clear_bit - Clear a bit and return its old value
 * @nr: Bit to clear
 * @addr: Address to count from
 *
 * This operation is atomic and cannot be reordered.  
 * It also implies a memory barrier.
 */

static inline int test_and_clear_bit(int nr, volatile unsigned long *addr)
{
	unsigned int mask, retval;
	unsigned long flags;
	unsigned int *adr = (unsigned int *)addr;
	
	adr += nr >> 5;
	mask = 1 << (nr & 0x1f);
	cris_atomic_save(addr, flags);
	retval = (mask & *adr) != 0;
	*adr &= ~mask;
	cris_atomic_restore(addr, flags);
	return retval;
}

/**
 * test_and_change_bit - Change a bit and return its old value
 * @nr: Bit to change
 * @addr: Address to count from
 *
 * This operation is atomic and cannot be reordered.  
 * It also implies a memory barrier.
 */

static inline int test_and_change_bit(int nr, volatile unsigned long *addr)
{
	unsigned int mask, retval;
	unsigned long flags;
	unsigned int *adr = (unsigned int *)addr;
	adr += nr >> 5;
	mask = 1 << (nr & 0x1f);
	cris_atomic_save(addr, flags);
	retval = (mask & *adr) != 0;
	*adr ^= mask;
	cris_atomic_restore(addr, flags);
	return retval;
}

#include <asm-generic/bitops/non-atomic.h>

/*
 * Since we define it "external", it collides with the built-in
 * definition, which doesn't have the same semantics.  We don't want to
 * use -fno-builtin, so just hide the name ffs.
 */
#define ffs kernel_ffs

#include <asm-generic/bitops/fls.h>
#include <asm-generic/bitops/__fls.h>
#include <asm-generic/bitops/fls64.h>
#include <asm-generic/bitops/hweight.h>
#include <asm-generic/bitops/find.h>
#include <asm-generic/bitops/lock.h>

#include <asm-generic/bitops/ext2-non-atomic.h>

#define ext2_set_bit_atomic(l,n,a)   test_and_set_bit(n,a)
#define ext2_clear_bit_atomic(l,n,a) test_and_clear_bit(n,a)

#include <asm-generic/bitops/minix.h>
#include <asm-generic/bitops/sched.h>

#endif /* __KERNEL__ */

#endif /* _CRIS_BITOPS_H */
