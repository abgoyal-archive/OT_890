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
 * Network checksum routines
 *
 * Copyright (C) 1999, 2003 Hewlett-Packard Co
 *	Stephane Eranian <eranian@hpl.hp.com>
 *
 * Most of the code coming from arch/alpha/lib/checksum.c
 *
 * This file contains network checksum routines that are better done
 * in an architecture-specific manner due to speed..
 */

#include <linux/module.h>
#include <linux/string.h>

#include <asm/byteorder.h>

static inline unsigned short
from64to16 (unsigned long x)
{
	/* add up 32-bit words for 33 bits */
	x = (x & 0xffffffff) + (x >> 32);
	/* add up 16-bit and 17-bit words for 17+c bits */
	x = (x & 0xffff) + (x >> 16);
	/* add up 16-bit and 2-bit for 16+c bit */
	x = (x & 0xffff) + (x >> 16);
	/* add up carry.. */
	x = (x & 0xffff) + (x >> 16);
	return x;
}

/*
 * computes the checksum of the TCP/UDP pseudo-header
 * returns a 16-bit checksum, already complemented.
 */
__sum16
csum_tcpudp_magic (__be32 saddr, __be32 daddr, unsigned short len,
		   unsigned short proto, __wsum sum)
{
	return (__force __sum16)~from64to16(
		(__force u64)saddr + (__force u64)daddr +
		(__force u64)sum + ((len + proto) << 8));
}

EXPORT_SYMBOL(csum_tcpudp_magic);

__wsum
csum_tcpudp_nofold (__be32 saddr, __be32 daddr, unsigned short len,
		    unsigned short proto, __wsum sum)
{
	unsigned long result;

	result = (__force u64)saddr + (__force u64)daddr +
		 (__force u64)sum + ((len + proto) << 8);

	/* Fold down to 32-bits so we don't lose in the typedef-less network stack.  */
	/* 64 to 33 */
	result = (result & 0xffffffff) + (result >> 32);
	/* 33 to 32 */
	result = (result & 0xffffffff) + (result >> 32);
	return (__force __wsum)result;
}
EXPORT_SYMBOL(csum_tcpudp_nofold);

extern unsigned long do_csum (const unsigned char *, long);

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
__wsum csum_partial(const void *buff, int len, __wsum sum)
{
	u64 result = do_csum(buff, len);

	/* add in old sum, and carry.. */
	result += (__force u32)sum;
	/* 32+c bits -> 32 bits */
	result = (result & 0xffffffff) + (result >> 32);
	return (__force __wsum)result;
}

EXPORT_SYMBOL(csum_partial);

/*
 * this routine is used for miscellaneous IP-like checksums, mainly
 * in icmp.c
 */
__sum16 ip_compute_csum (const void *buff, int len)
{
	return (__force __sum16)~do_csum(buff,len);
}

EXPORT_SYMBOL(ip_compute_csum);
