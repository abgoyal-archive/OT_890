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

#ifndef _M68K_STRING_H_
#define _M68K_STRING_H_

#include <linux/types.h>
#include <linux/compiler.h>

static inline size_t __kernel_strlen(const char *s)
{
	const char *sc;

	for (sc = s; *sc++; )
		;
	return sc - s - 1;
}

static inline char *__kernel_strcpy(char *dest, const char *src)
{
	char *xdest = dest;

	asm volatile ("\n"
		"1:	move.b	(%1)+,(%0)+\n"
		"	jne	1b"
		: "+a" (dest), "+a" (src)
		: : "memory");
	return xdest;
}

#ifndef __IN_STRING_C

#define __HAVE_ARCH_STRLEN
#define strlen(s)	(__builtin_constant_p(s) ?	\
			 __builtin_strlen(s) :		\
			 __kernel_strlen(s))

#define __HAVE_ARCH_STRNLEN
static inline size_t strnlen(const char *s, size_t count)
{
	const char *sc = s;

	asm volatile ("\n"
		"1:     subq.l  #1,%1\n"
		"       jcs     2f\n"
		"       tst.b   (%0)+\n"
		"       jne     1b\n"
		"       subq.l  #1,%0\n"
		"2:"
		: "+a" (sc), "+d" (count));
	return sc - s;
}

#define __HAVE_ARCH_STRCPY
#if __GNUC__ >= 4
#define strcpy(d, s)	(__builtin_constant_p(s) &&	\
			 __builtin_strlen(s) <= 32 ?	\
			 __builtin_strcpy(d, s) :	\
			 __kernel_strcpy(d, s))
#else
#define strcpy(d, s)	__kernel_strcpy(d, s)
#endif

#define __HAVE_ARCH_STRNCPY
static inline char *strncpy(char *dest, const char *src, size_t n)
{
	char *xdest = dest;

	asm volatile ("\n"
		"	jra	2f\n"
		"1:	move.b	(%1),(%0)+\n"
		"	jeq	2f\n"
		"	addq.l	#1,%1\n"
		"2:	subq.l	#1,%2\n"
		"	jcc	1b\n"
		: "+a" (dest), "+a" (src), "+d" (n)
		: : "memory");
	return xdest;
}

#define __HAVE_ARCH_STRCAT
#define strcat(d, s)	({			\
	char *__d = (d);			\
	strcpy(__d + strlen(__d), (s));		\
})

#define __HAVE_ARCH_STRCHR
static inline char *strchr(const char *s, int c)
{
	char sc, ch = c;

	for (; (sc = *s++) != ch; ) {
		if (!sc)
			return NULL;
	}
	return (char *)s - 1;
}

#define __HAVE_ARCH_STRCMP
static inline int strcmp(const char *cs, const char *ct)
{
	char res;

	asm ("\n"
		"1:	move.b	(%0)+,%2\n"	/* get *cs */
		"	cmp.b	(%1)+,%2\n"	/* compare a byte */
		"	jne	2f\n"		/* not equal, break out */
		"	tst.b	%2\n"		/* at end of cs? */
		"	jne	1b\n"		/* no, keep going */
		"	jra	3f\n"		/* strings are equal */
		"2:	sub.b	-(%1),%2\n"	/* *cs - *ct */
		"3:"
		: "+a" (cs), "+a" (ct), "=d" (res));
	return res;
}

#define __HAVE_ARCH_MEMSET
extern void *memset(void *, int, __kernel_size_t);
#define memset(d, c, n) __builtin_memset(d, c, n)

#define __HAVE_ARCH_MEMCPY
extern void *memcpy(void *, const void *, __kernel_size_t);
#define memcpy(d, s, n) __builtin_memcpy(d, s, n)

#define __HAVE_ARCH_MEMMOVE
extern void *memmove(void *, const void *, __kernel_size_t);

#define __HAVE_ARCH_MEMCMP
extern int memcmp(const void *, const void *, __kernel_size_t);
#define memcmp(d, s, n) __builtin_memcmp(d, s, n)

#endif

#endif /* _M68K_STRING_H_ */
