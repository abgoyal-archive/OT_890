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
 * INET		An implementation of the TCP/IP protocol suite for the LINUX
 *		operating system.  INET is implemented using the  BSD Socket
 *		interface as the means of communication with the user level.
 *
 *		Checksumming functions for IPv6
 *
 * Authors:	Jorge Cwik, <jorge@laser.satlink.net>
 *		Arnt Gulbrandsen, <agulbra@nvg.unit.no>
 *		Borrows very liberally from tcp.c and ip.c, see those
 *		files for more names.
 *
 *		This program is free software; you can redistribute it and/or
 *		modify it under the terms of the GNU General Public License
 *		as published by the Free Software Foundation; either version
 *		2 of the License, or (at your option) any later version.
 */

/*
 *	Fixes:
 *
 *	Ralf Baechle			:	generic ipv6 checksum
 *	<ralf@waldorf-gmbh.de>
 */

#ifndef _CHECKSUM_IPV6_H
#define _CHECKSUM_IPV6_H

#include <asm/types.h>
#include <asm/byteorder.h>
#include <net/ip.h>
#include <asm/checksum.h>
#include <linux/in6.h>

#ifndef _HAVE_ARCH_IPV6_CSUM

static __inline__ __sum16 csum_ipv6_magic(const struct in6_addr *saddr,
					  const struct in6_addr *daddr,
					  __u32 len, unsigned short proto,
					  __wsum csum)
{

	int carry;
	__u32 ulen;
	__u32 uproto;
	__u32 sum = (__force u32)csum;

	sum += (__force u32)saddr->s6_addr32[0];
	carry = (sum < (__force u32)saddr->s6_addr32[0]);
	sum += carry;

	sum += (__force u32)saddr->s6_addr32[1];
	carry = (sum < (__force u32)saddr->s6_addr32[1]);
	sum += carry;

	sum += (__force u32)saddr->s6_addr32[2];
	carry = (sum < (__force u32)saddr->s6_addr32[2]);
	sum += carry;

	sum += (__force u32)saddr->s6_addr32[3];
	carry = (sum < (__force u32)saddr->s6_addr32[3]);
	sum += carry;

	sum += (__force u32)daddr->s6_addr32[0];
	carry = (sum < (__force u32)daddr->s6_addr32[0]);
	sum += carry;

	sum += (__force u32)daddr->s6_addr32[1];
	carry = (sum < (__force u32)daddr->s6_addr32[1]);
	sum += carry;

	sum += (__force u32)daddr->s6_addr32[2];
	carry = (sum < (__force u32)daddr->s6_addr32[2]);
	sum += carry;

	sum += (__force u32)daddr->s6_addr32[3];
	carry = (sum < (__force u32)daddr->s6_addr32[3]);
	sum += carry;

	ulen = (__force u32)htonl((__u32) len);
	sum += ulen;
	carry = (sum < ulen);
	sum += carry;

	uproto = (__force u32)htonl(proto);
	sum += uproto;
	carry = (sum < uproto);
	sum += carry;

	return csum_fold((__force __wsum)sum);
}

#endif
#endif
