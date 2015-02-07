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
 * Copyright (C) 1994 by Waldorf Electronics
 * Copyright (C) 1995 - 2000, 01, 03 by Ralf Baechle
 * Copyright (C) 1999, 2000 Silicon Graphics, Inc.
 * Copyright (C) 2007  Maciej W. Rozycki
 */
#ifndef _ASM_DELAY_H
#define _ASM_DELAY_H

#include <linux/param.h>
#include <linux/smp.h>

#include <asm/compiler.h>
#include <asm/war.h>

static inline void __delay(unsigned long loops)
{
	if (sizeof(long) == 4)
		__asm__ __volatile__ (
		"	.set	noreorder				\n"
		"	.align	3					\n"
		"1:	bnez	%0, 1b					\n"
		"	subu	%0, 1					\n"
		"	.set	reorder					\n"
		: "=r" (loops)
		: "0" (loops));
	else if (sizeof(long) == 8 && !DADDI_WAR)
		__asm__ __volatile__ (
		"	.set	noreorder				\n"
		"	.align	3					\n"
		"1:	bnez	%0, 1b					\n"
		"	dsubu	%0, 1					\n"
		"	.set	reorder					\n"
		: "=r" (loops)
		: "0" (loops));
	else if (sizeof(long) == 8 && DADDI_WAR)
		__asm__ __volatile__ (
		"	.set	noreorder				\n"
		"	.align	3					\n"
		"1:	bnez	%0, 1b					\n"
		"	dsubu	%0, %2					\n"
		"	.set	reorder					\n"
		: "=r" (loops)
		: "0" (loops), "r" (1));
}


/*
 * Division by multiplication: you don't have to worry about
 * loss of precision.
 *
 * Use only for very small delays ( < 1 msec).  Should probably use a
 * lookup table, really, as the multiplications take much too long with
 * short delays.  This is a "reasonable" implementation, though (and the
 * first constant multiplications gets optimized away if the delay is
 * a constant)
 */

static inline void __udelay(unsigned long usecs, unsigned long lpj)
{
	unsigned long hi, lo;

	/*
	 * The rates of 128 is rounded wrongly by the catchall case
	 * for 64-bit.  Excessive precission?  Probably ...
	 */
#if defined(CONFIG_64BIT) && (HZ == 128)
	usecs *= 0x0008637bd05af6c7UL;		/* 2**64 / (1000000 / HZ) */
#elif defined(CONFIG_64BIT)
	usecs *= (0x8000000000000000UL / (500000 / HZ));
#else /* 32-bit junk follows here */
	usecs *= (unsigned long) (((0x8000000000000000ULL / (500000 / HZ)) +
	                           0x80000000ULL) >> 32);
#endif

	if (sizeof(long) == 4)
		__asm__("multu\t%2, %3"
		: "=h" (usecs), "=l" (lo)
		: "r" (usecs), "r" (lpj)
		: GCC_REG_ACCUM);
	else if (sizeof(long) == 8 && !R4000_WAR)
		__asm__("dmultu\t%2, %3"
		: "=h" (usecs), "=l" (lo)
		: "r" (usecs), "r" (lpj)
		: GCC_REG_ACCUM);
	else if (sizeof(long) == 8 && R4000_WAR)
		__asm__("dmultu\t%3, %4\n\tmfhi\t%0"
		: "=r" (usecs), "=h" (hi), "=l" (lo)
		: "r" (usecs), "r" (lpj)
		: GCC_REG_ACCUM);

	__delay(usecs);
}

#define __udelay_val cpu_data[raw_smp_processor_id()].udelay_val

#define udelay(usecs) __udelay((usecs), __udelay_val)

/* make sure "usecs *= ..." in udelay do not overflow. */
#if HZ >= 1000
#define MAX_UDELAY_MS	1
#elif HZ <= 200
#define MAX_UDELAY_MS	5
#else
#define MAX_UDELAY_MS	(1000 / HZ)
#endif

#endif /* _ASM_DELAY_H */
