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
 * include/asm-xtensa/string.h
 *
 * These trivial string functions are considered part of the public domain.
 *
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 *
 * Copyright (C) 2001 - 2005 Tensilica Inc.
 */

/* We should optimize these. See arch/xtensa/lib/strncpy_user.S */

#ifndef _XTENSA_STRING_H
#define _XTENSA_STRING_H

#define __HAVE_ARCH_STRCPY
static inline char *strcpy(char *__dest, const char *__src)
{
	register char *__xdest = __dest;
	unsigned long __dummy;

	__asm__ __volatile__("1:\n\t"
		"l8ui	%2, %1, 0\n\t"
		"s8i	%2, %0, 0\n\t"
		"addi	%1, %1, 1\n\t"
		"addi	%0, %0, 1\n\t"
		"bnez	%2, 1b\n\t"
		: "=r" (__dest), "=r" (__src), "=&r" (__dummy)
		: "0" (__dest), "1" (__src)
		: "memory");

	return __xdest;
}

#define __HAVE_ARCH_STRNCPY
static inline char *strncpy(char *__dest, const char *__src, size_t __n)
{
	register char *__xdest = __dest;
	unsigned long __dummy;

	if (__n == 0)
		return __xdest;

	__asm__ __volatile__(
		"1:\n\t"
		"l8ui	%2, %1, 0\n\t"
		"s8i	%2, %0, 0\n\t"
		"addi	%1, %1, 1\n\t"
		"addi	%0, %0, 1\n\t"
		"beqz	%2, 2f\n\t"
		"bne	%1, %5, 1b\n"
		"2:"
		: "=r" (__dest), "=r" (__src), "=&r" (__dummy)
		: "0" (__dest), "1" (__src), "r" (__src+__n)
		: "memory");

	return __xdest;
}

#define __HAVE_ARCH_STRCMP
static inline int strcmp(const char *__cs, const char *__ct)
{
	register int __res;
	unsigned long __dummy;

	__asm__ __volatile__(
		"1:\n\t"
		"l8ui	%3, %1, 0\n\t"
		"addi	%1, %1, 1\n\t"
		"l8ui	%2, %0, 0\n\t"
		"addi	%0, %0, 1\n\t"
		"beqz	%2, 2f\n\t"
		"beq	%2, %3, 1b\n"
		"2:\n\t"
		"sub	%2, %3, %2"
		: "=r" (__cs), "=r" (__ct), "=&r" (__res), "=&r" (__dummy)
		: "0" (__cs), "1" (__ct));

	return __res;
}

#define __HAVE_ARCH_STRNCMP
static inline int strncmp(const char *__cs, const char *__ct, size_t __n)
{
	register int __res;
	unsigned long __dummy;

	__asm__ __volatile__(
		"mov	%2, %3\n"
		"1:\n\t"
		"beq	%0, %6, 2f\n\t"
		"l8ui	%3, %1, 0\n\t"
		"addi	%1, %1, 1\n\t"
		"l8ui	%2, %0, 0\n\t"
		"addi	%0, %0, 1\n\t"
		"beqz	%2, 2f\n\t"
		"beqz	%3, 2f\n\t"
		"beq	%2, %3, 1b\n"
		"2:\n\t"
		"sub	%2, %3, %2"
		: "=r" (__cs), "=r" (__ct), "=&r" (__res), "=&r" (__dummy)
		: "0" (__cs), "1" (__ct), "r" (__cs+__n));

	return __res;
}

#define __HAVE_ARCH_MEMSET
extern void *memset(void *__s, int __c, size_t __count);

#define __HAVE_ARCH_MEMCPY
extern void *memcpy(void *__to, __const__ void *__from, size_t __n);

#define __HAVE_ARCH_MEMMOVE
extern void *memmove(void *__dest, __const__ void *__src, size_t __n);

/* Don't build bcopy at all ...  */
#define __HAVE_ARCH_BCOPY

#define __HAVE_ARCH_MEMSCAN
#define memscan memchr

#endif	/* _XTENSA_STRING_H */
