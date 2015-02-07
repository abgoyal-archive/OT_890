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
 * arch/sh/lib/io.c - SH32 optimized I/O routines
 *
 * Copyright (C) 2000  Stuart Menefy
 * Copyright (C) 2005  Paul Mundt
 *
 * Provide real functions which expand to whatever the header file defined.
 * Also definitions of machine independent IO functions.
 *
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 */
#include <linux/module.h>
#include <linux/io.h>

void __raw_readsl(const void __iomem *addr, void *datap, int len)
{
	u32 *data;

	for (data = datap; (len != 0) && (((u32)data & 0x1f) != 0); len--)
		*data++ = __raw_readl(addr);

	if (likely(len >= (0x20 >> 2))) {
		int tmp2, tmp3, tmp4, tmp5, tmp6;

		__asm__ __volatile__(
			"1:			\n\t"
			"mov.l	@%7, r0		\n\t"
			"mov.l	@%7, %2		\n\t"
#ifdef CONFIG_CPU_SH4
			"movca.l r0, @%0	\n\t"
#else
			"mov.l	r0, @%0		\n\t"
#endif
			"mov.l	@%7, %3		\n\t"
			"mov.l	@%7, %4		\n\t"
			"mov.l	@%7, %5		\n\t"
			"mov.l	@%7, %6		\n\t"
			"mov.l	@%7, r7		\n\t"
			"mov.l	@%7, r0		\n\t"
			"mov.l	%2, @(0x04,%0)	\n\t"
			"mov	#0x20>>2, %2	\n\t"
			"mov.l	%3, @(0x08,%0)	\n\t"
			"sub	%2, %1		\n\t"
			"mov.l	%4, @(0x0c,%0)	\n\t"
			"cmp/hi	%1, %2		! T if 32 > len	\n\t"
			"mov.l	%5, @(0x10,%0)	\n\t"
			"mov.l	%6, @(0x14,%0)	\n\t"
			"mov.l	r7, @(0x18,%0)	\n\t"
			"mov.l	r0, @(0x1c,%0)	\n\t"
			"bf.s	1b		\n\t"
			" add	#0x20, %0	\n\t"
			: "=&r" (data), "=&r" (len),
			  "=&r" (tmp2), "=&r" (tmp3), "=&r" (tmp4),
			  "=&r" (tmp5), "=&r" (tmp6)
			: "r"(addr), "0" (data), "1" (len)
			: "r0", "r7", "t", "memory");
	}

	for (; len != 0; len--)
		*data++ = __raw_readl(addr);
}
EXPORT_SYMBOL(__raw_readsl);

void __raw_writesl(void __iomem *addr, const void *data, int len)
{
	if (likely(len != 0)) {
		int tmp1;

		__asm__ __volatile__ (
			"1:				\n\t"
			"mov.l	@%0+, %1	\n\t"
			"dt		%3		\n\t"
			"bf.s		1b		\n\t"
			" mov.l	%1, @%4		\n\t"
			: "=&r" (data), "=&r" (tmp1)
			: "0" (data), "r" (len), "r"(addr)
			: "t", "memory");
	}
}
EXPORT_SYMBOL(__raw_writesl);
