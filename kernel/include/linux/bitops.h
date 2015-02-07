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

#ifndef _LINUX_BITOPS_H
#define _LINUX_BITOPS_H
#include <asm/types.h>

#ifdef	__KERNEL__
#define BIT(nr)			(1UL << (nr))
#define BIT_MASK(nr)		(1UL << ((nr) % BITS_PER_LONG))
#define BIT_WORD(nr)		((nr) / BITS_PER_LONG)
#define BITS_PER_BYTE		8
#define BITS_TO_LONGS(nr)	DIV_ROUND_UP(nr, BITS_PER_BYTE * sizeof(long))
#endif

/*
 * Include this here because some architectures need generic_ffs/fls in
 * scope
 */
#include <asm/bitops.h>

#define for_each_bit(bit, addr, size) \
	for ((bit) = find_first_bit((addr), (size)); \
	     (bit) < (size); \
	     (bit) = find_next_bit((addr), (size), (bit) + 1))


static __inline__ int get_bitmask_order(unsigned int count)
{
	int order;
	
	order = fls(count);
	return order;	/* We could be slightly more clever with -1 here... */
}

static __inline__ int get_count_order(unsigned int count)
{
	int order;
	
	order = fls(count) - 1;
	if (count & (count - 1))
		order++;
	return order;
}

static inline unsigned long hweight_long(unsigned long w)
{
	return sizeof(w) == 4 ? hweight32(w) : hweight64(w);
}

/**
 * rol32 - rotate a 32-bit value left
 * @word: value to rotate
 * @shift: bits to roll
 */
static inline __u32 rol32(__u32 word, unsigned int shift)
{
	return (word << shift) | (word >> (32 - shift));
}

/**
 * ror32 - rotate a 32-bit value right
 * @word: value to rotate
 * @shift: bits to roll
 */
static inline __u32 ror32(__u32 word, unsigned int shift)
{
	return (word >> shift) | (word << (32 - shift));
}

/**
 * rol16 - rotate a 16-bit value left
 * @word: value to rotate
 * @shift: bits to roll
 */
static inline __u16 rol16(__u16 word, unsigned int shift)
{
	return (word << shift) | (word >> (16 - shift));
}

/**
 * ror16 - rotate a 16-bit value right
 * @word: value to rotate
 * @shift: bits to roll
 */
static inline __u16 ror16(__u16 word, unsigned int shift)
{
	return (word >> shift) | (word << (16 - shift));
}

/**
 * rol8 - rotate an 8-bit value left
 * @word: value to rotate
 * @shift: bits to roll
 */
static inline __u8 rol8(__u8 word, unsigned int shift)
{
	return (word << shift) | (word >> (8 - shift));
}

/**
 * ror8 - rotate an 8-bit value right
 * @word: value to rotate
 * @shift: bits to roll
 */
static inline __u8 ror8(__u8 word, unsigned int shift)
{
	return (word >> shift) | (word << (8 - shift));
}

static inline unsigned fls_long(unsigned long l)
{
	if (sizeof(l) == 4)
		return fls(l);
	return fls64(l);
}

#ifdef __KERNEL__
#ifdef CONFIG_GENERIC_FIND_FIRST_BIT

/**
 * find_first_bit - find the first set bit in a memory region
 * @addr: The address to start the search at
 * @size: The maximum size to search
 *
 * Returns the bit number of the first set bit.
 */
extern unsigned long find_first_bit(const unsigned long *addr,
				    unsigned long size);

/**
 * find_first_zero_bit - find the first cleared bit in a memory region
 * @addr: The address to start the search at
 * @size: The maximum size to search
 *
 * Returns the bit number of the first cleared bit.
 */
extern unsigned long find_first_zero_bit(const unsigned long *addr,
					 unsigned long size);
#endif /* CONFIG_GENERIC_FIND_FIRST_BIT */

#ifdef CONFIG_GENERIC_FIND_LAST_BIT
/**
 * find_last_bit - find the last set bit in a memory region
 * @addr: The address to start the search at
 * @size: The maximum size to search
 *
 * Returns the bit number of the first set bit, or size.
 */
extern unsigned long find_last_bit(const unsigned long *addr,
				   unsigned long size);
#endif /* CONFIG_GENERIC_FIND_LAST_BIT */

#ifdef CONFIG_GENERIC_FIND_NEXT_BIT

/**
 * find_next_bit - find the next set bit in a memory region
 * @addr: The address to base the search on
 * @offset: The bitnumber to start searching at
 * @size: The bitmap size in bits
 */
extern unsigned long find_next_bit(const unsigned long *addr,
				   unsigned long size, unsigned long offset);

/**
 * find_next_zero_bit - find the next cleared bit in a memory region
 * @addr: The address to base the search on
 * @offset: The bitnumber to start searching at
 * @size: The bitmap size in bits
 */

extern unsigned long find_next_zero_bit(const unsigned long *addr,
					unsigned long size,
					unsigned long offset);

#endif /* CONFIG_GENERIC_FIND_NEXT_BIT */
#endif /* __KERNEL__ */
#endif
