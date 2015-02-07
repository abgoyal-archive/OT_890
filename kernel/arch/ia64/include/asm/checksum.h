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

#ifndef _ASM_IA64_CHECKSUM_H
#define _ASM_IA64_CHECKSUM_H

/*
 * Modified 1998, 1999
 *	David Mosberger-Tang <davidm@hpl.hp.com>, Hewlett-Packard Co
 */

/*
 * This is a version of ip_compute_csum() optimized for IP headers,
 * which always checksum on 4 octet boundaries.
 */
extern __sum16 ip_fast_csum(const void *iph, unsigned int ihl);

/*
 * Computes the checksum of the TCP/UDP pseudo-header returns a 16-bit
 * checksum, already complemented
 */
extern __sum16 csum_tcpudp_magic (__be32 saddr, __be32 daddr,
					     unsigned short len,
					     unsigned short proto,
					     __wsum sum);

extern __wsum csum_tcpudp_nofold (__be32 saddr, __be32 daddr,
					unsigned short len,
					unsigned short proto,
					__wsum sum);

/*
 * Computes the checksum of a memory block at buff, length len,
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
extern __wsum csum_partial(const void *buff, int len, __wsum sum);

/*
 * Same as csum_partial, but copies from src while it checksums.
 *
 * Here it is even more important to align src and dst on a 32-bit (or
 * even better 64-bit) boundary.
 */
extern __wsum csum_partial_copy_from_user(const void __user *src, void *dst,
						 int len, __wsum sum,
						 int *errp);

extern __wsum csum_partial_copy_nocheck(const void *src, void *dst,
					       int len, __wsum sum);

/*
 * This routine is used for miscellaneous IP-like checksums, mainly in
 * icmp.c
 */
extern __sum16 ip_compute_csum(const void *buff, int len);

/*
 * Fold a partial checksum without adding pseudo headers.
 */
static inline __sum16 csum_fold(__wsum csum)
{
	u32 sum = (__force u32)csum;
	sum = (sum & 0xffff) + (sum >> 16);
	sum = (sum & 0xffff) + (sum >> 16);
	return (__force __sum16)~sum;
}

#define _HAVE_ARCH_IPV6_CSUM	1
struct in6_addr;
extern __sum16 csum_ipv6_magic(const struct in6_addr *saddr,
	const struct in6_addr *daddr, __u32 len, unsigned short proto,
	__wsum csum);

#endif /* _ASM_IA64_CHECKSUM_H */
