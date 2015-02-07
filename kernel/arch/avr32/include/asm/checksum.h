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
 * Copyright (C) 2004-2006 Atmel Corporation
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */
#ifndef __ASM_AVR32_CHECKSUM_H
#define __ASM_AVR32_CHECKSUM_H

/*
 * computes the checksum of a memory block at buff, length len,
 * and adds in "sum" (32-bit)
 *
 * returns a 32-bit number suitable for feeding into itself
 * or csum_tcpudp_magic
 *
 * this function must be called with even lengths, except
 * for the last fragment, which may be odd
 *
 * it's best to have buff aligned on a 32-bit boundary
 */
__wsum csum_partial(const void *buff, int len, __wsum sum);

/*
 * the same as csum_partial, but copies from src while it
 * checksums, and handles user-space pointer exceptions correctly, when needed.
 *
 * here even more important to align src and dst on a 32-bit (or even
 * better 64-bit) boundary
 */
__wsum csum_partial_copy_generic(const void *src, void *dst, int len,
				       __wsum sum, int *src_err_ptr,
				       int *dst_err_ptr);

/*
 *	Note: when you get a NULL pointer exception here this means someone
 *	passed in an incorrect kernel address to one of these functions.
 *
 *	If you use these functions directly please don't forget the
 *	access_ok().
 */
static inline
__wsum csum_partial_copy_nocheck(const void *src, void *dst,
				       int len, __wsum sum)
{
	return csum_partial_copy_generic(src, dst, len, sum, NULL, NULL);
}

static inline
__wsum csum_partial_copy_from_user(const void __user *src, void *dst,
					  int len, __wsum sum, int *err_ptr)
{
	return csum_partial_copy_generic((const void __force *)src, dst, len,
					 sum, err_ptr, NULL);
}

/*
 *	This is a version of ip_compute_csum() optimized for IP headers,
 *	which always checksum on 4 octet boundaries.
 */
static inline __sum16 ip_fast_csum(const void *iph, unsigned int ihl)
{
	unsigned int sum, tmp;

	__asm__ __volatile__(
		"	ld.w	%0, %1++\n"
		"	ld.w	%3, %1++\n"
		"	sub	%2, 4\n"
		"	add	%0, %3\n"
		"	ld.w	%3, %1++\n"
		"	adc	%0, %0, %3\n"
		"	ld.w	%3, %1++\n"
		"	adc	%0, %0, %3\n"
		"	acr	%0\n"
		"1:	ld.w	%3, %1++\n"
		"	add	%0, %3\n"
		"	acr	%0\n"
		"	sub	%2, 1\n"
		"	brne	1b\n"
		"	lsl	%3, %0, 16\n"
		"	andl	%0, 0\n"
		"	mov	%2, 0xffff\n"
		"	add	%0, %3\n"
		"	adc	%0, %0, %2\n"
		"	com	%0\n"
		"	lsr	%0, 16\n"
		: "=r"(sum), "=r"(iph), "=r"(ihl), "=r"(tmp)
		: "1"(iph), "2"(ihl)
		: "memory", "cc");
	return (__force __sum16)sum;
}

/*
 *	Fold a partial checksum
 */

static inline __sum16 csum_fold(__wsum sum)
{
	unsigned int tmp;

	asm("	bfextu	%1, %0, 0, 16\n"
	    "	lsr	%0, 16\n"
	    "	add	%0, %1\n"
	    "	bfextu	%1, %0, 16, 16\n"
	    "	add	%0, %1"
	    : "=&r"(sum), "=&r"(tmp)
	    : "0"(sum));

	return (__force __sum16)~sum;
}

static inline __wsum csum_tcpudp_nofold(__be32 saddr, __be32 daddr,
					       unsigned short len,
					       unsigned short proto,
					       __wsum sum)
{
	asm("	add	%0, %1\n"
	    "	adc	%0, %0, %2\n"
	    "	adc	%0, %0, %3\n"
	    "	acr	%0"
	    : "=r"(sum)
	    : "r"(daddr), "r"(saddr), "r"(len + proto),
	      "0"(sum)
	    : "cc");

	return sum;
}

/*
 * computes the checksum of the TCP/UDP pseudo-header
 * returns a 16-bit checksum, already complemented
 */
static inline __sum16 csum_tcpudp_magic(__be32 saddr, __be32 daddr,
						   unsigned short len,
						   unsigned short proto,
						   __wsum sum)
{
	return csum_fold(csum_tcpudp_nofold(saddr,daddr,len,proto,sum));
}

/*
 * this routine is used for miscellaneous IP-like checksums, mainly
 * in icmp.c
 */

static inline __sum16 ip_compute_csum(const void *buff, int len)
{
    return csum_fold(csum_partial(buff, len, 0));
}

#endif /* __ASM_AVR32_CHECKSUM_H */
