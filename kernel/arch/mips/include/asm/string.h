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
 * Copyright (c) 1994, 95, 96, 97, 98, 2000, 01 Ralf Baechle
 * Copyright (c) 2000 by Silicon Graphics, Inc.
 * Copyright (c) 2001 MIPS Technologies, Inc.
 */
#ifndef _ASM_STRING_H
#define _ASM_STRING_H


/*
 * Most of the inline functions are rather naive implementations so I just
 * didn't bother updating them for 64-bit ...
 */
#ifdef CONFIG_32BIT

#ifndef IN_STRING_C

#define __HAVE_ARCH_STRCPY
static __inline__ char *strcpy(char *__dest, __const__ char *__src)
{
  char *__xdest = __dest;

  __asm__ __volatile__(
	".set\tnoreorder\n\t"
	".set\tnoat\n"
	"1:\tlbu\t$1,(%1)\n\t"
	"addiu\t%1,1\n\t"
	"sb\t$1,(%0)\n\t"
	"bnez\t$1,1b\n\t"
	"addiu\t%0,1\n\t"
	".set\tat\n\t"
	".set\treorder"
	: "=r" (__dest), "=r" (__src)
        : "0" (__dest), "1" (__src)
	: "memory");

  return __xdest;
}

#define __HAVE_ARCH_STRNCPY
static __inline__ char *strncpy(char *__dest, __const__ char *__src, size_t __n)
{
  char *__xdest = __dest;

  if (__n == 0)
    return __xdest;

  __asm__ __volatile__(
	".set\tnoreorder\n\t"
	".set\tnoat\n"
	"1:\tlbu\t$1,(%1)\n\t"
	"subu\t%2,1\n\t"
	"sb\t$1,(%0)\n\t"
	"beqz\t$1,2f\n\t"
	"addiu\t%0,1\n\t"
	"bnez\t%2,1b\n\t"
	"addiu\t%1,1\n"
	"2:\n\t"
	".set\tat\n\t"
	".set\treorder"
        : "=r" (__dest), "=r" (__src), "=r" (__n)
        : "0" (__dest), "1" (__src), "2" (__n)
        : "memory");

  return __xdest;
}

#define __HAVE_ARCH_STRCMP
static __inline__ int strcmp(__const__ char *__cs, __const__ char *__ct)
{
  int __res;

  __asm__ __volatile__(
	".set\tnoreorder\n\t"
	".set\tnoat\n\t"
	"lbu\t%2,(%0)\n"
	"1:\tlbu\t$1,(%1)\n\t"
	"addiu\t%0,1\n\t"
	"bne\t$1,%2,2f\n\t"
	"addiu\t%1,1\n\t"
	"bnez\t%2,1b\n\t"
	"lbu\t%2,(%0)\n\t"
#if defined(CONFIG_CPU_R3000)
	"nop\n\t"
#endif
	"move\t%2,$1\n"
	"2:\tsubu\t%2,$1\n"
	"3:\t.set\tat\n\t"
	".set\treorder"
	: "=r" (__cs), "=r" (__ct), "=r" (__res)
	: "0" (__cs), "1" (__ct));

  return __res;
}

#endif /* !defined(IN_STRING_C) */

#define __HAVE_ARCH_STRNCMP
static __inline__ int
strncmp(__const__ char *__cs, __const__ char *__ct, size_t __count)
{
	int __res;

	__asm__ __volatile__(
	".set\tnoreorder\n\t"
	".set\tnoat\n"
	"1:\tlbu\t%3,(%0)\n\t"
	"beqz\t%2,2f\n\t"
	"lbu\t$1,(%1)\n\t"
	"subu\t%2,1\n\t"
	"bne\t$1,%3,3f\n\t"
	"addiu\t%0,1\n\t"
	"bnez\t%3,1b\n\t"
	"addiu\t%1,1\n"
	"2:\n\t"
#if defined(CONFIG_CPU_R3000)
	"nop\n\t"
#endif
	"move\t%3,$1\n"
	"3:\tsubu\t%3,$1\n\t"
	".set\tat\n\t"
	".set\treorder"
	: "=r" (__cs), "=r" (__ct), "=r" (__count), "=r" (__res)
	: "0" (__cs), "1" (__ct), "2" (__count));

	return __res;
}
#endif /* CONFIG_32BIT */

#define __HAVE_ARCH_MEMSET
extern void *memset(void *__s, int __c, size_t __count);

#define __HAVE_ARCH_MEMCPY
extern void *memcpy(void *__to, __const__ void *__from, size_t __n);

#define __HAVE_ARCH_MEMMOVE
extern void *memmove(void *__dest, __const__ void *__src, size_t __n);

#endif /* _ASM_STRING_H */
