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

#ifndef __ASM_SH_STRING_H
#define __ASM_SH_STRING_H

#ifdef __KERNEL__

/*
 * Copyright (C) 1999 Niibe Yutaka
 * But consider these trivial functions to be public domain.
 */

#define __HAVE_ARCH_STRCPY
static inline char *strcpy(char *__dest, const char *__src)
{
	register char *__xdest = __dest;
	unsigned long __dummy;

	__asm__ __volatile__("1:\n\t"
			     "mov.b	@%1+, %2\n\t"
			     "mov.b	%2, @%0\n\t"
			     "cmp/eq	#0, %2\n\t"
			     "bf/s	1b\n\t"
			     " add	#1, %0\n\t"
			     : "=r" (__dest), "=r" (__src), "=&z" (__dummy)
			     : "0" (__dest), "1" (__src)
			     : "memory", "t");

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
		"1:\n"
		"mov.b	@%1+, %2\n\t"
		"mov.b	%2, @%0\n\t"
		"cmp/eq	#0, %2\n\t"
		"bt/s	2f\n\t"
		" cmp/eq	%5,%1\n\t"
		"bf/s	1b\n\t"
		" add	#1, %0\n"
		"2:"
		: "=r" (__dest), "=r" (__src), "=&z" (__dummy)
		: "0" (__dest), "1" (__src), "r" (__src+__n)
		: "memory", "t");

	return __xdest;
}

#define __HAVE_ARCH_STRCMP
static inline int strcmp(const char *__cs, const char *__ct)
{
	register int __res;
	unsigned long __dummy;

	__asm__ __volatile__(
		"mov.b	@%1+, %3\n"
		"1:\n\t"
		"mov.b	@%0+, %2\n\t"
		"cmp/eq #0, %3\n\t"
		"bt	2f\n\t"
		"cmp/eq %2, %3\n\t"
		"bt/s	1b\n\t"
		" mov.b	@%1+, %3\n\t"
		"add	#-2, %1\n\t"
		"mov.b	@%1, %3\n\t"
		"sub	%3, %2\n"
		"2:"
		: "=r" (__cs), "=r" (__ct), "=&r" (__res), "=&z" (__dummy)
		: "0" (__cs), "1" (__ct)
		: "t");

	return __res;
}

#define __HAVE_ARCH_STRNCMP
static inline int strncmp(const char *__cs, const char *__ct, size_t __n)
{
	register int __res;
	unsigned long __dummy;

	if (__n == 0)
		return 0;

	__asm__ __volatile__(
		"mov.b	@%1+, %3\n"
		"1:\n\t"
		"mov.b	@%0+, %2\n\t"
		"cmp/eq %6, %0\n\t"
		"bt/s	2f\n\t"
		" cmp/eq #0, %3\n\t"
		"bt/s	3f\n\t"
		" cmp/eq %3, %2\n\t"
		"bt/s	1b\n\t"
		" mov.b	@%1+, %3\n\t"
		"add	#-2, %1\n\t"
		"mov.b	@%1, %3\n"
		"2:\n\t"
		"sub	%3, %2\n"
		"3:"
		:"=r" (__cs), "=r" (__ct), "=&r" (__res), "=&z" (__dummy)
		: "0" (__cs), "1" (__ct), "r" (__cs+__n)
		: "t");

	return __res;
}

#define __HAVE_ARCH_MEMSET
extern void *memset(void *__s, int __c, size_t __count);

#define __HAVE_ARCH_MEMCPY
extern void *memcpy(void *__to, __const__ void *__from, size_t __n);

#define __HAVE_ARCH_MEMMOVE
extern void *memmove(void *__dest, __const__ void *__src, size_t __n);

#define __HAVE_ARCH_MEMCHR
extern void *memchr(const void *__s, int __c, size_t __n);

#define __HAVE_ARCH_STRLEN
extern size_t strlen(const char *);

#endif /* __KERNEL__ */

#endif /* __ASM_SH_STRING_H */
