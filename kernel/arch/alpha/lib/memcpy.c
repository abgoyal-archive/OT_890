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
 *  linux/arch/alpha/lib/memcpy.c
 *
 *  Copyright (C) 1995  Linus Torvalds
 */

/*
 * This is a reasonably optimized memcpy() routine.
 */

/*
 * Note that the C code is written to be optimized into good assembly. However,
 * at this point gcc is unable to sanely compile "if (n >= 0)", resulting in a
 * explicit compare against 0 (instead of just using the proper "blt reg, xx" or
 * "bge reg, xx"). I hope alpha-gcc will be fixed to notice this eventually..
 */

#include <linux/types.h>

/*
 * This should be done in one go with ldq_u*2/mask/stq_u. Do it
 * with a macro so that we can fix it up later..
 */
#define ALIGN_DEST_TO8_UP(d,s,n) \
	while (d & 7) { \
		if (n <= 0) return; \
		n--; \
		*(char *) d = *(char *) s; \
		d++; s++; \
	}
#define ALIGN_DEST_TO8_DN(d,s,n) \
	while (d & 7) { \
		if (n <= 0) return; \
		n--; \
		d--; s--; \
		*(char *) d = *(char *) s; \
	}

/*
 * This should similarly be done with ldq_u*2/mask/stq. The destination
 * is aligned, but we don't fill in a full quad-word
 */
#define DO_REST_UP(d,s,n) \
	while (n > 0) { \
		n--; \
		*(char *) d = *(char *) s; \
		d++; s++; \
	}
#define DO_REST_DN(d,s,n) \
	while (n > 0) { \
		n--; \
		d--; s--; \
		*(char *) d = *(char *) s; \
	}

/*
 * This should be done with ldq/mask/stq. The source and destination are
 * aligned, but we don't fill in a full quad-word
 */
#define DO_REST_ALIGNED_UP(d,s,n) DO_REST_UP(d,s,n)
#define DO_REST_ALIGNED_DN(d,s,n) DO_REST_DN(d,s,n)

/*
 * This does unaligned memory copies. We want to avoid storing to
 * an unaligned address, as that would do a read-modify-write cycle.
 * We also want to avoid double-reading the unaligned reads.
 *
 * Note the ordering to try to avoid load (and address generation) latencies.
 */
static inline void __memcpy_unaligned_up (unsigned long d, unsigned long s,
					  long n)
{
	ALIGN_DEST_TO8_UP(d,s,n);
	n -= 8;			/* to avoid compare against 8 in the loop */
	if (n >= 0) {
		unsigned long low_word, high_word;
		__asm__("ldq_u %0,%1":"=r" (low_word):"m" (*(unsigned long *) s));
		do {
			unsigned long tmp;
			__asm__("ldq_u %0,%1":"=r" (high_word):"m" (*(unsigned long *)(s+8)));
			n -= 8;
			__asm__("extql %1,%2,%0"
				:"=r" (low_word)
				:"r" (low_word), "r" (s));
			__asm__("extqh %1,%2,%0"
				:"=r" (tmp)
				:"r" (high_word), "r" (s));
			s += 8;
			*(unsigned long *) d = low_word | tmp;
			d += 8;
			low_word = high_word;
		} while (n >= 0);
	}
	n += 8;
	DO_REST_UP(d,s,n);
}

static inline void __memcpy_unaligned_dn (unsigned long d, unsigned long s,
					  long n)
{
	/* I don't understand AXP assembler well enough for this. -Tim */
	s += n;
	d += n;
	while (n--)
		* (char *) --d = * (char *) --s;
}

/*
 * Hmm.. Strange. The __asm__ here is there to make gcc use an integer register
 * for the load-store. I don't know why, but it would seem that using a floating
 * point register for the move seems to slow things down (very small difference,
 * though).
 *
 * Note the ordering to try to avoid load (and address generation) latencies.
 */
static inline void __memcpy_aligned_up (unsigned long d, unsigned long s,
					long n)
{
	ALIGN_DEST_TO8_UP(d,s,n);
	n -= 8;
	while (n >= 0) {
		unsigned long tmp;
		__asm__("ldq %0,%1":"=r" (tmp):"m" (*(unsigned long *) s));
		n -= 8;
		s += 8;
		*(unsigned long *) d = tmp;
		d += 8;
	}
	n += 8;
	DO_REST_ALIGNED_UP(d,s,n);
}
static inline void __memcpy_aligned_dn (unsigned long d, unsigned long s,
					long n)
{
	s += n;
	d += n;
	ALIGN_DEST_TO8_DN(d,s,n);
	n -= 8;
	while (n >= 0) {
		unsigned long tmp;
		s -= 8;
		__asm__("ldq %0,%1":"=r" (tmp):"m" (*(unsigned long *) s));
		n -= 8;
		d -= 8;
		*(unsigned long *) d = tmp;
	}
	n += 8;
	DO_REST_ALIGNED_DN(d,s,n);
}

void * memcpy(void * dest, const void *src, size_t n)
{
	if (!(((unsigned long) dest ^ (unsigned long) src) & 7)) {
		__memcpy_aligned_up ((unsigned long) dest, (unsigned long) src,
				     n);
		return dest;
	}
	__memcpy_unaligned_up ((unsigned long) dest, (unsigned long) src, n);
	return dest;
}

/* For backward modules compatibility, define __memcpy.  */
asm("__memcpy = memcpy; .globl __memcpy");
