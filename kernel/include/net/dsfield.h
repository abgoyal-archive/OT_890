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

/* include/net/dsfield.h - Manipulation of the Differentiated Services field */

/* Written 1998-2000 by Werner Almesberger, EPFL ICA */


#ifndef __NET_DSFIELD_H
#define __NET_DSFIELD_H

#include <linux/types.h>
#include <linux/ip.h>
#include <linux/ipv6.h>
#include <asm/byteorder.h>


static inline __u8 ipv4_get_dsfield(const struct iphdr *iph)
{
	return iph->tos;
}


static inline __u8 ipv6_get_dsfield(const struct ipv6hdr *ipv6h)
{
	return ntohs(*(const __be16 *)ipv6h) >> 4;
}


static inline void ipv4_change_dsfield(struct iphdr *iph,__u8 mask,
    __u8 value)
{
        __u32 check = ntohs((__force __be16)iph->check);
	__u8 dsfield;

	dsfield = (iph->tos & mask) | value;
	check += iph->tos;
	if ((check+1) >> 16) check = (check+1) & 0xffff;
	check -= dsfield;
	check += check >> 16; /* adjust carry */
	iph->check = (__force __sum16)htons(check);
	iph->tos = dsfield;
}


static inline void ipv6_change_dsfield(struct ipv6hdr *ipv6h,__u8 mask,
    __u8 value)
{
        __u16 tmp;

	tmp = ntohs(*(__be16 *) ipv6h);
	tmp = (tmp & ((mask << 4) | 0xf00f)) | (value << 4);
	*(__be16 *) ipv6h = htons(tmp);
}


#endif
