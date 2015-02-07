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

/* uaccess.c: userspace access functions
 *
 * Copyright (C) 2004 Red Hat, Inc. All Rights Reserved.
 * Written by David Howells (dhowells@redhat.com)
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version
 * 2 of the License, or (at your option) any later version.
 */

#include <linux/mm.h>
#include <linux/module.h>
#include <asm/uaccess.h>

/*****************************************************************************/
/*
 * copy a null terminated string from userspace
 */
long strncpy_from_user(char *dst, const char __user *src, long count)
{
	unsigned long max;
	char *p, ch;
	long err = -EFAULT;

	if (count < 0)
		BUG();

	p = dst;

#ifndef CONFIG_MMU
	if ((unsigned long) src < memory_start)
		goto error;
#endif

	if ((unsigned long) src >= get_addr_limit())
		goto error;

	max = get_addr_limit() - (unsigned long) src;
	if ((unsigned long) count > max) {
		memset(dst + max, 0, count - max);
		count = max;
	}

	err = 0;
	for (; count > 0; count--, p++, src++) {
		__get_user_asm(err, ch, src, "ub", "=r");
		if (err < 0)
			goto error;
		if (!ch)
			break;
		*p = ch;
	}

	err = p - dst; /* return length excluding NUL */

 error:
	if (count > 0)
		memset(p, 0, count); /* clear remainder of buffer [security] */

	return err;

} /* end strncpy_from_user() */

EXPORT_SYMBOL(strncpy_from_user);

/*****************************************************************************/
/*
 * Return the size of a string (including the ending 0)
 *
 * Return 0 on exception, a value greater than N if too long
 */
long strnlen_user(const char __user *src, long count)
{
	const char __user *p;
	long err = 0;
	char ch;

	if (count < 0)
		BUG();

#ifndef CONFIG_MMU
	if ((unsigned long) src < memory_start)
		return 0;
#endif

	if ((unsigned long) src >= get_addr_limit())
		return 0;

	for (p = src; count > 0; count--, p++) {
		__get_user_asm(err, ch, p, "ub", "=r");
		if (err < 0)
			return 0;
		if (!ch)
			break;
	}

	return p - src + 1; /* return length including NUL */

} /* end strnlen_user() */

EXPORT_SYMBOL(strnlen_user);
