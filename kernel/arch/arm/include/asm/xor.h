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
 *  arch/arm/include/asm/xor.h
 *
 *  Copyright (C) 2001 Russell King
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */
#include <asm-generic/xor.h>

#define __XOR(a1, a2) a1 ^= a2

#define GET_BLOCK_2(dst) \
	__asm__("ldmia	%0, {%1, %2}" \
		: "=r" (dst), "=r" (a1), "=r" (a2) \
		: "0" (dst))

#define GET_BLOCK_4(dst) \
	__asm__("ldmia	%0, {%1, %2, %3, %4}" \
		: "=r" (dst), "=r" (a1), "=r" (a2), "=r" (a3), "=r" (a4) \
		: "0" (dst))

#define XOR_BLOCK_2(src) \
	__asm__("ldmia	%0!, {%1, %2}" \
		: "=r" (src), "=r" (b1), "=r" (b2) \
		: "0" (src)); \
	__XOR(a1, b1); __XOR(a2, b2);

#define XOR_BLOCK_4(src) \
	__asm__("ldmia	%0!, {%1, %2, %3, %4}" \
		: "=r" (src), "=r" (b1), "=r" (b2), "=r" (b3), "=r" (b4) \
		: "0" (src)); \
	__XOR(a1, b1); __XOR(a2, b2); __XOR(a3, b3); __XOR(a4, b4)

#define PUT_BLOCK_2(dst) \
	__asm__ __volatile__("stmia	%0!, {%2, %3}" \
		: "=r" (dst) \
		: "0" (dst), "r" (a1), "r" (a2))

#define PUT_BLOCK_4(dst) \
	__asm__ __volatile__("stmia	%0!, {%2, %3, %4, %5}" \
		: "=r" (dst) \
		: "0" (dst), "r" (a1), "r" (a2), "r" (a3), "r" (a4))

static void
xor_arm4regs_2(unsigned long bytes, unsigned long *p1, unsigned long *p2)
{
	unsigned int lines = bytes / sizeof(unsigned long) / 4;
	register unsigned int a1 __asm__("r4");
	register unsigned int a2 __asm__("r5");
	register unsigned int a3 __asm__("r6");
	register unsigned int a4 __asm__("r7");
	register unsigned int b1 __asm__("r8");
	register unsigned int b2 __asm__("r9");
	register unsigned int b3 __asm__("ip");
	register unsigned int b4 __asm__("lr");

	do {
		GET_BLOCK_4(p1);
		XOR_BLOCK_4(p2);
		PUT_BLOCK_4(p1);
	} while (--lines);
}

static void
xor_arm4regs_3(unsigned long bytes, unsigned long *p1, unsigned long *p2,
		unsigned long *p3)
{
	unsigned int lines = bytes / sizeof(unsigned long) / 4;
	register unsigned int a1 __asm__("r4");
	register unsigned int a2 __asm__("r5");
	register unsigned int a3 __asm__("r6");
	register unsigned int a4 __asm__("r7");
	register unsigned int b1 __asm__("r8");
	register unsigned int b2 __asm__("r9");
	register unsigned int b3 __asm__("ip");
	register unsigned int b4 __asm__("lr");

	do {
		GET_BLOCK_4(p1);
		XOR_BLOCK_4(p2);
		XOR_BLOCK_4(p3);
		PUT_BLOCK_4(p1);
	} while (--lines);
}

static void
xor_arm4regs_4(unsigned long bytes, unsigned long *p1, unsigned long *p2,
		unsigned long *p3, unsigned long *p4)
{
	unsigned int lines = bytes / sizeof(unsigned long) / 2;
	register unsigned int a1 __asm__("r8");
	register unsigned int a2 __asm__("r9");
	register unsigned int b1 __asm__("ip");
	register unsigned int b2 __asm__("lr");

	do {
		GET_BLOCK_2(p1);
		XOR_BLOCK_2(p2);
		XOR_BLOCK_2(p3);
		XOR_BLOCK_2(p4);
		PUT_BLOCK_2(p1);
	} while (--lines);
}

static void
xor_arm4regs_5(unsigned long bytes, unsigned long *p1, unsigned long *p2,
		unsigned long *p3, unsigned long *p4, unsigned long *p5)
{
	unsigned int lines = bytes / sizeof(unsigned long) / 2;
	register unsigned int a1 __asm__("r8");
	register unsigned int a2 __asm__("r9");
	register unsigned int b1 __asm__("ip");
	register unsigned int b2 __asm__("lr");

	do {
		GET_BLOCK_2(p1);
		XOR_BLOCK_2(p2);
		XOR_BLOCK_2(p3);
		XOR_BLOCK_2(p4);
		XOR_BLOCK_2(p5);
		PUT_BLOCK_2(p1);
	} while (--lines);
}

static struct xor_block_template xor_block_arm4regs = {
	.name	= "arm4regs",
	.do_2	= xor_arm4regs_2,
	.do_3	= xor_arm4regs_3,
	.do_4	= xor_arm4regs_4,
	.do_5	= xor_arm4regs_5,
};

#undef XOR_TRY_TEMPLATES
#define XOR_TRY_TEMPLATES			\
	do {					\
		xor_speed(&xor_block_arm4regs);	\
		xor_speed(&xor_block_8regs);	\
		xor_speed(&xor_block_32regs);	\
	} while (0)
