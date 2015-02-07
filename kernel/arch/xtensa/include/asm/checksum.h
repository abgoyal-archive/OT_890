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
 * include/asm-xtensa/checksum.h
 *
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 *
 * Copyright (C) 2001 - 2005 Tensilica Inc.
 */

#ifndef _XTENSA_CHECKSUM_H
#define _XTENSA_CHECKSUM_H

#include <linux/in6.h>
#include <variant/core.h>

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
asmlinkage __wsum csum_partial(const void *buff, int len, __wsum sum);

/*
 * the same as csum_partial, but copies from src while it
 * checksums, and handles user-space pointer exceptions correctly, when needed.
 *
 * here even more important to align src and dst on a 32-bit (or even
 * better 64-bit) boundary
 */

asmlinkage __wsum csum_partial_copy_generic(const void *src, void *dst, int len, __wsum sum,
						   int *src_err_ptr, int *dst_err_ptr);

/*
 *	Note: when you get a NULL pointer exception here this means someone
 *	passed in an incorrect kernel address to one of these functions.
 *
 *	If you use these functions directly please don't forget the access_ok().
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
	return csum_partial_copy_generic((__force const void *)src, dst,
					len, sum, err_ptr, NULL);
}

/*
 *	Fold a partial checksum
 */

static __inline__ __sum16 csum_fold(__wsum sum)
{
	unsigned int __dummy;
	__asm__("extui	%1, %0, 16, 16\n\t"
		"extui	%0 ,%0, 0, 16\n\t"
		"add	%0, %0, %1\n\t"
		"slli	%1, %0, 16\n\t"
		"add	%0, %0, %1\n\t"
		"extui	%0, %0, 16, 16\n\t"
		"neg	%0, %0\n\t"
		"addi	%0, %0, -1\n\t"
		"extui	%0, %0, 0, 16\n\t"
		: "=r" (sum), "=&r" (__dummy)
		: "0" (sum));
	return (__force __sum16)sum;
}

/*
 *	This is a version of ip_compute_csum() optimized for IP headers,
 *	which always checksum on 4 octet boundaries.
 */
static __inline__ __sum16 ip_fast_csum(const void *iph, unsigned int ihl)
{
	unsigned int sum, tmp, endaddr;

	__asm__ __volatile__(
		"sub		%0, %0, %0\n\t"
#if XCHAL_HAVE_LOOPS
		"loopgtz	%2, 2f\n\t"
#else
		"beqz		%2, 2f\n\t"
		"slli		%4, %2, 2\n\t"
		"add		%4, %4, %1\n\t"
		"0:\t"
#endif
		"l32i		%3, %1, 0\n\t"
		"add		%0, %0, %3\n\t"
		"bgeu		%0, %3, 1f\n\t"
		"addi		%0, %0, 1\n\t"
		"1:\t"
		"addi		%1, %1, 4\n\t"
#if !XCHAL_HAVE_LOOPS
		"blt		%1, %4, 0b\n\t"
#endif
		"2:\t"
	/* Since the input registers which are loaded with iph and ihl
	   are modified, we must also specify them as outputs, or gcc
	   will assume they contain their original values. */
		: "=r" (sum), "=r" (iph), "=r" (ihl), "=&r" (tmp), "=&r" (endaddr)
		: "1" (iph), "2" (ihl));

	return	csum_fold(sum);
}

static __inline__ __wsum csum_tcpudp_nofold(__be32 saddr, __be32 daddr,
						   unsigned short len,
						   unsigned short proto,
						   __wsum sum)
{

#ifdef __XTENSA_EL__
	unsigned long len_proto = (len + proto) << 8;
#elif defined(__XTENSA_EB__)
	unsigned long len_proto = len + proto;
#else
# error processor byte order undefined!
#endif
	__asm__("add	%0, %0, %1\n\t"
		"bgeu	%0, %1, 1f\n\t"
		"addi	%0, %0, 1\n\t"
		"1:\t"
		"add	%0, %0, %2\n\t"
		"bgeu	%0, %2, 1f\n\t"
		"addi	%0, %0, 1\n\t"
		"1:\t"
		"add	%0, %0, %3\n\t"
		"bgeu	%0, %3, 1f\n\t"
		"addi	%0, %0, 1\n\t"
		"1:\t"
		: "=r" (sum), "=r" (len_proto)
		: "r" (daddr), "r" (saddr), "1" (len_proto), "0" (sum));
	return sum;
}

/*
 * computes the checksum of the TCP/UDP pseudo-header
 * returns a 16-bit checksum, already complemented
 */
static __inline__ __sum16 csum_tcpudp_magic(__be32 saddr, __be32 daddr,
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

static __inline__ __sum16 ip_compute_csum(const void *buff, int len)
{
    return csum_fold (csum_partial(buff, len, 0));
}

#define _HAVE_ARCH_IPV6_CSUM
static __inline__ __sum16 csum_ipv6_magic(const struct in6_addr *saddr,
					  const struct in6_addr *daddr,
					  __u32 len, unsigned short proto,
					  __wsum sum)
{
	unsigned int __dummy;
	__asm__("l32i	%1, %2, 0\n\t"
		"add	%0, %0, %1\n\t"
		"bgeu	%0, %1, 1f\n\t"
		"addi	%0, %0, 1\n\t"
		"1:\t"
		"l32i	%1, %2, 4\n\t"
		"add	%0, %0, %1\n\t"
		"bgeu	%0, %1, 1f\n\t"
		"addi	%0, %0, 1\n\t"
		"1:\t"
		"l32i	%1, %2, 8\n\t"
		"add	%0, %0, %1\n\t"
		"bgeu	%0, %1, 1f\n\t"
		"addi	%0, %0, 1\n\t"
		"1:\t"
		"l32i	%1, %2, 12\n\t"
		"add	%0, %0, %1\n\t"
		"bgeu	%0, %1, 1f\n\t"
		"addi	%0, %0, 1\n\t"
		"1:\t"
		"l32i	%1, %3, 0\n\t"
		"add	%0, %0, %1\n\t"
		"bgeu	%0, %1, 1f\n\t"
		"addi	%0, %0, 1\n\t"
		"1:\t"
		"l32i	%1, %3, 4\n\t"
		"add	%0, %0, %1\n\t"
		"bgeu	%0, %1, 1f\n\t"
		"addi	%0, %0, 1\n\t"
		"1:\t"
		"l32i	%1, %3, 8\n\t"
		"add	%0, %0, %1\n\t"
		"bgeu	%0, %1, 1f\n\t"
		"addi	%0, %0, 1\n\t"
		"1:\t"
		"l32i	%1, %3, 12\n\t"
		"add	%0, %0, %1\n\t"
		"bgeu	%0, %1, 1f\n\t"
		"addi	%0, %0, 1\n\t"
		"1:\t"
		"add	%0, %0, %4\n\t"
		"bgeu	%0, %4, 1f\n\t"
		"addi	%0, %0, 1\n\t"
		"1:\t"
		"add	%0, %0, %5\n\t"
		"bgeu	%0, %5, 1f\n\t"
		"addi	%0, %0, 1\n\t"
		"1:\t"
		: "=r" (sum), "=&r" (__dummy)
		: "r" (saddr), "r" (daddr),
		  "r" (htonl(len)), "r" (htonl(proto)), "0" (sum));

	return csum_fold(sum);
}

/*
 *	Copy and checksum to user
 */
#define HAVE_CSUM_COPY_USER
static __inline__ __wsum csum_and_copy_to_user(const void *src, void __user *dst,
				    int len, __wsum sum, int *err_ptr)
{
	if (access_ok(VERIFY_WRITE, dst, len))
		return csum_partial_copy_generic(src, dst, len, sum, NULL, err_ptr);

	if (len)
		*err_ptr = -EFAULT;

	return (__force __wsum)-1; /* invalid checksum */
}
#endif
