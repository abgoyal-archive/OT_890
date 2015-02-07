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

#ifndef _S390_CHECKSUM_H
#define _S390_CHECKSUM_H

/*
 *  include/asm-s390/checksum.h
 *    S390 fast network checksum routines
 *    see also arch/S390/lib/checksum.c
 *
 *  S390 version
 *    Copyright (C) 1999 IBM Deutschland Entwicklung GmbH, IBM Corporation
 *    Author(s): Ulrich Hild        (first version)
 *               Martin Schwidefsky (heavily optimized CKSM version)
 *               D.J. Barrow        (third attempt) 
 */

#include <asm/uaccess.h>

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
static inline __wsum
csum_partial(const void *buff, int len, __wsum sum)
{
	register unsigned long reg2 asm("2") = (unsigned long) buff;
	register unsigned long reg3 asm("3") = (unsigned long) len;

	asm volatile(
		"0:	cksm	%0,%1\n"	/* do checksum on longs */
		"	jo	0b\n"
		: "+d" (sum), "+d" (reg2), "+d" (reg3) : : "cc", "memory");
	return sum;
}

/*
 * the same as csum_partial_copy, but copies from user space.
 *
 * here even more important to align src and dst on a 32-bit (or even
 * better 64-bit) boundary
 *
 * Copy from userspace and compute checksum.  If we catch an exception
 * then zero the rest of the buffer.
 */
static inline __wsum
csum_partial_copy_from_user(const void __user *src, void *dst,
                                          int len, __wsum sum,
                                          int *err_ptr)
{
	int missing;

	missing = copy_from_user(dst, src, len);
	if (missing) {
		memset(dst + len - missing, 0, missing);
		*err_ptr = -EFAULT;
	}
		
	return csum_partial(dst, len, sum);
}


static inline __wsum
csum_partial_copy_nocheck (const void *src, void *dst, int len, __wsum sum)
{
        memcpy(dst,src,len);
	return csum_partial(dst, len, sum);
}

/*
 *      Fold a partial checksum without adding pseudo headers
 */
static inline __sum16 csum_fold(__wsum sum)
{
#ifndef __s390x__
	register_pair rp;

	asm volatile(
		"	slr	%N1,%N1\n"	/* %0 = H L */
		"	lr	%1,%0\n"	/* %0 = H L, %1 = H L 0 0 */
		"	srdl	%1,16\n"	/* %0 = H L, %1 = 0 H L 0 */
		"	alr	%1,%N1\n"	/* %0 = H L, %1 = L H L 0 */
		"	alr	%0,%1\n"	/* %0 = H+L+C L+H */
		"	srl	%0,16\n"	/* %0 = H+L+C */
		: "+&d" (sum), "=d" (rp) : : "cc");
#else /* __s390x__ */
	asm volatile(
		"	sr	3,3\n"		/* %0 = H*65536 + L */
		"	lr	2,%0\n"		/* %0 = H L, 2/3 = H L / 0 0 */
		"	srdl	2,16\n"		/* %0 = H L, 2/3 = 0 H / L 0 */
		"	alr	2,3\n"		/* %0 = H L, 2/3 = L H / L 0 */
		"	alr	%0,2\n"		/* %0 = H+L+C L+H */
		"	srl	%0,16\n"	/* %0 = H+L+C */
		: "+&d" (sum) : : "cc", "2", "3");
#endif /* __s390x__ */
	return (__force __sum16) ~sum;
}

/*
 *	This is a version of ip_compute_csum() optimized for IP headers,
 *	which always checksum on 4 octet boundaries.
 *
 */
static inline __sum16 ip_fast_csum(const void *iph, unsigned int ihl)
{
	return csum_fold(csum_partial(iph, ihl*4, 0));
}

/*
 * computes the checksum of the TCP/UDP pseudo-header
 * returns a 32-bit checksum
 */
static inline __wsum
csum_tcpudp_nofold(__be32 saddr, __be32 daddr,
                   unsigned short len, unsigned short proto,
                   __wsum sum)
{
	__u32 csum = (__force __u32)sum;

	csum += (__force __u32)saddr;
	if (csum < (__force __u32)saddr)
		csum++;

	csum += (__force __u32)daddr;
	if (csum < (__force __u32)daddr)
		csum++;

	csum += len + proto;
	if (csum < len + proto)
		csum++;

	return (__force __wsum)csum;
}

/*
 * computes the checksum of the TCP/UDP pseudo-header
 * returns a 16-bit checksum, already complemented
 */

static inline __sum16
csum_tcpudp_magic(__be32 saddr, __be32 daddr,
                  unsigned short len, unsigned short proto,
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

#endif /* _S390_CHECKSUM_H */


