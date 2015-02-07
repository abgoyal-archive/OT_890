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
 * Network Checksum & Copy routine
 *
 * Copyright (C) 1999, 2003-2004 Hewlett-Packard Co
 *	Stephane Eranian <eranian@hpl.hp.com>
 *
 * Most of the code has been imported from Linux/Alpha
 */

#include <linux/module.h>
#include <linux/types.h>
#include <linux/string.h>

#include <asm/uaccess.h>

/*
 * XXX Fixme: those 2 inlines are meant for debugging and will go away
 */
static inline unsigned
short from64to16(unsigned long x)
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

static inline
unsigned long do_csum_c(const unsigned char * buff, int len, unsigned int psum)
{
	int odd, count;
	unsigned long result = (unsigned long)psum;

	if (len <= 0)
		goto out;
	odd = 1 & (unsigned long) buff;
	if (odd) {
		result = *buff << 8;
		len--;
		buff++;
	}
	count = len >> 1;		/* nr of 16-bit words.. */
	if (count) {
		if (2 & (unsigned long) buff) {
			result += *(unsigned short *) buff;
			count--;
			len -= 2;
			buff += 2;
		}
		count >>= 1;		/* nr of 32-bit words.. */
		if (count) {
			if (4 & (unsigned long) buff) {
				result += *(unsigned int *) buff;
				count--;
				len -= 4;
				buff += 4;
			}
			count >>= 1;	/* nr of 64-bit words.. */
			if (count) {
				unsigned long carry = 0;
				do {
					unsigned long w = *(unsigned long *) buff;
					count--;
					buff += 8;
					result += carry;
					result += w;
					carry = (w > result);
				} while (count);
				result += carry;
				result = (result & 0xffffffff) + (result >> 32);
			}
			if (len & 4) {
				result += *(unsigned int *) buff;
				buff += 4;
			}
		}
		if (len & 2) {
			result += *(unsigned short *) buff;
			buff += 2;
		}
	}
	if (len & 1)
		result += *buff;

	result = from64to16(result);

	if (odd)
		result = ((result >> 8) & 0xff) | ((result & 0xff) << 8);

out:
	return result;
}

/*
 * XXX Fixme
 *
 * This is very ugly but temporary. THIS NEEDS SERIOUS ENHANCEMENTS.
 * But it's very tricky to get right even in C.
 */
extern unsigned long do_csum(const unsigned char *, long);

__wsum
csum_partial_copy_from_user(const void __user *src, void *dst,
				int len, __wsum psum, int *errp)
{
	unsigned long result;

	/* XXX Fixme
	 * for now we separate the copy from checksum for obvious
	 * alignment difficulties. Look at the Alpha code and you'll be
	 * scared.
	 */

	if (__copy_from_user(dst, src, len) != 0 && errp)
		*errp = -EFAULT;

	result = do_csum(dst, len);

	/* add in old sum, and carry.. */
	result += (__force u32)psum;
	/* 32+c bits -> 32 bits */
	result = (result & 0xffffffff) + (result >> 32);
	return (__force __wsum)result;
}

EXPORT_SYMBOL(csum_partial_copy_from_user);

__wsum
csum_partial_copy_nocheck(const void *src, void *dst, int len, __wsum sum)
{
	return csum_partial_copy_from_user((__force const void __user *)src,
					   dst, len, sum, NULL);
}

EXPORT_SYMBOL(csum_partial_copy_nocheck);
