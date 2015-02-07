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

/* asm/arch/bitops.h for Linux/CRISv10 */

#ifndef _CRIS_ARCH_BITOPS_H
#define _CRIS_ARCH_BITOPS_H

/*
 * Helper functions for the core of the ff[sz] functions, wrapping the
 * syntactically awkward asms.  The asms compute the number of leading
 * zeroes of a bits-in-byte and byte-in-word and word-in-dword-swapped
 * number.  They differ in that the first function also inverts all bits
 * in the input.
 */
static inline unsigned long cris_swapnwbrlz(unsigned long w)
{
	/* Let's just say we return the result in the same register as the
	   input.  Saying we clobber the input but can return the result
	   in another register:
	   !  __asm__ ("swapnwbr %2\n\tlz %2,%0"
	   !	      : "=r,r" (res), "=r,X" (dummy) : "1,0" (w));
	   confuses gcc (sched.c, gcc from cris-dist-1.14).  */

	unsigned long res;
	__asm__ ("swapnwbr %0 \n\t"
		 "lz %0,%0"
		 : "=r" (res) : "0" (w));
	return res;
}

static inline unsigned long cris_swapwbrlz(unsigned long w)
{
	unsigned res;
	__asm__ ("swapwbr %0 \n\t"
		 "lz %0,%0"
		 : "=r" (res)
		 : "0" (w));
	return res;
}

/*
 * ffz = Find First Zero in word. Undefined if no zero exists,
 * so code should check against ~0UL first..
 */
static inline unsigned long ffz(unsigned long w)
{
	return cris_swapnwbrlz(w);
}

/**
 * __ffs - find first bit in word.
 * @word: The word to search
 *
 * Undefined if no bit exists, so code should check against 0 first.
 */
static inline unsigned long __ffs(unsigned long word)
{
	return cris_swapnwbrlz(~word);
}

/**
 * ffs - find first bit set
 * @x: the word to search
 *
 * This is defined the same way as
 * the libc and compiler builtin ffs routines, therefore
 * differs in spirit from the above ffz (man ffs).
 */

static inline unsigned long kernel_ffs(unsigned long w)
{
	return w ? cris_swapwbrlz (w) + 1 : 0;
}

#endif
