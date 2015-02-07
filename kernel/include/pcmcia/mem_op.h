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
 * mem_op.h
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * The initial developer of the original code is David A. Hinds
 * <dahinds@users.sourceforge.net>.  Portions created by David A. Hinds
 * are Copyright (C) 1999 David A. Hinds.  All Rights Reserved.
 *
 * (C) 1999             David A. Hinds
 */

#ifndef _LINUX_MEM_OP_H
#define _LINUX_MEM_OP_H

#include <asm/uaccess.h>
#include <asm/io.h>

/*
   If UNSAFE_MEMCPY is defined, we use the (optimized) system routines
   to copy between a card and kernel memory.  These routines do 32-bit
   operations which may not work with all PCMCIA controllers.  The
   safe versions defined here will do only 8-bit and 16-bit accesses.
*/

#ifdef UNSAFE_MEMCPY

#define copy_from_pc memcpy_fromio
#define copy_to_pc memcpy_toio

static inline void copy_pc_to_user(void *to, const void *from, size_t n)
{
    size_t odd = (n & 3);
    n -= odd;
    while (n) {
	put_user(__raw_readl(from), (int *)to);
	(char *)from += 4; (char *)to += 4; n -= 4;
    }
    while (odd--)
	put_user(readb((char *)from++), (char *)to++);
}

static inline void copy_user_to_pc(void *to, const void *from, size_t n)
{
    int l;
    char c;
    size_t odd = (n & 3);
    n -= odd;
    while (n) {
	get_user(l, (int *)from);
	__raw_writel(l, to);
	(char *)to += 4; (char *)from += 4; n -= 4;
    }
    while (odd--) {
	get_user(c, (char *)from++);
	writeb(c, (char *)to++);
    }
}

#else /* UNSAFE_MEMCPY */

static inline void copy_from_pc(void *to, void __iomem *from, size_t n)
{
	__u16 *t = to;
	__u16 __iomem *f = from;
	size_t odd = (n & 1);
	for (n >>= 1; n; n--)
		*t++ = __raw_readw(f++);
	if (odd)
		*(__u8 *)t = readb(f);
}

static inline void copy_to_pc(void __iomem *to, const void *from, size_t n)
{
	__u16 __iomem *t = to;
	const __u16 *f = from;
	size_t odd = (n & 1);
	for (n >>= 1; n ; n--)
		__raw_writew(*f++, t++);
	if (odd)
		writeb(*(__u8 *)f, t);
}

static inline void copy_pc_to_user(void __user *to, void __iomem *from, size_t n)
{
	__u16 __user *t = to;
	__u16 __iomem *f = from;
	size_t odd = (n & 1);
	for (n >>= 1; n ; n--)
		put_user(__raw_readw(f++), t++);
	if (odd)
		put_user(readb(f), (char __user *)t);
}

static inline void copy_user_to_pc(void __iomem *to, void __user *from, size_t n)
{
	__u16 __user *f = from;
	__u16 __iomem *t = to;
	short s;
	char c;
	size_t odd = (n & 1);
	for (n >>= 1; n; n--) {
		get_user(s, f++);
		__raw_writew(s, t++);
	}
	if (odd) {
		get_user(c, (char __user *)f);
		writeb(c, t);
	}
}

#endif /* UNSAFE_MEMCPY */

#endif /* _LINUX_MEM_OP_H */
