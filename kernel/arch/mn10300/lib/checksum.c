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

/* MN10300 Optimised checksumming wrappers
 *
 * Copyright (C) 2007 Matsushita Electric Industrial Co., Ltd.
 * Copyright (C) 2007 Red Hat, Inc. All Rights Reserved.
 * Written by David Howells (dhowells@redhat.com)
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public Licence
 * as published by the Free Software Foundation; either version
 * 2 of the Licence, or (at your option) any later version.
 */
#include <linux/module.h>
#include <linux/errno.h>
#include <asm/byteorder.h>
#include <asm/uaccess.h>
#include <asm/checksum.h>
#include "internal.h"

static inline unsigned short from32to16(__wsum sum)
{
	asm("	add	%1,%0		\n"
	    "	addc	0xffff,%0	\n"
	    : "=r" (sum)
	    : "r" (sum << 16), "0" (sum & 0xffff0000)
	    );
	return sum >> 16;
}

__sum16 ip_fast_csum(const void *iph, unsigned int ihl)
{
	return ~do_csum(iph, ihl * 4);
}
EXPORT_SYMBOL(ip_fast_csum);

__wsum csum_partial(const void *buff, int len, __wsum sum)
{
	__wsum result;

	result = do_csum(buff, len);
	result += sum;
	if (sum > result)
		result++;
	return result;
}
EXPORT_SYMBOL(csum_partial);

__sum16 ip_compute_csum(const void *buff, int len)
{
	return ~from32to16(do_csum(buff, len));
}
EXPORT_SYMBOL(ip_compute_csum);

__wsum csum_partial_copy(const void *src, void *dst, int len, __wsum sum)
{
	copy_from_user(dst, src, len);
	return csum_partial(dst, len, sum);
}
EXPORT_SYMBOL(csum_partial_copy);

__wsum csum_partial_copy_nocheck(const void *src, void *dst,
				 int len, __wsum sum)
{
	sum = csum_partial(src, len, sum);
	memcpy(dst, src, len);
	return sum;
}
EXPORT_SYMBOL(csum_partial_copy_nocheck);

__wsum csum_partial_copy_from_user(const void *src, void *dst,
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
EXPORT_SYMBOL(csum_partial_copy_from_user);

__wsum csum_and_copy_to_user(const void *src, void *dst,
			     int len, __wsum sum,
			     int *err_ptr)
{
	int missing;

	missing = copy_to_user(dst, src, len);
	if (missing) {
		memset(dst + len - missing, 0, missing);
		*err_ptr = -EFAULT;
	}

	return csum_partial(src, len, sum);
}
EXPORT_SYMBOL(csum_and_copy_to_user);
