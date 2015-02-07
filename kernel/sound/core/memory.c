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
 *  Copyright (c) by Jaroslav Kysela <perex@perex.cz>
 * 
 *  Misc memory accessors
 *
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation; either version 2 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program; if not, write to the Free Software
 *   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
 *
 */

#include <asm/io.h>
#include <asm/uaccess.h>
#include <sound/core.h>

/**
 * copy_to_user_fromio - copy data from mmio-space to user-space
 * @dst: the destination pointer on user-space
 * @src: the source pointer on mmio
 * @count: the data size to copy in bytes
 *
 * Copies the data from mmio-space to user-space.
 *
 * Returns zero if successful, or non-zero on failure.
 */
int copy_to_user_fromio(void __user *dst, const volatile void __iomem *src, size_t count)
{
#if defined(__i386__) || defined(CONFIG_SPARC32)
	return copy_to_user(dst, (const void __force*)src, count) ? -EFAULT : 0;
#else
	char buf[256];
	while (count) {
		size_t c = count;
		if (c > sizeof(buf))
			c = sizeof(buf);
		memcpy_fromio(buf, (void __iomem *)src, c);
		if (copy_to_user(dst, buf, c))
			return -EFAULT;
		count -= c;
		dst += c;
		src += c;
	}
	return 0;
#endif
}

EXPORT_SYMBOL(copy_to_user_fromio);

/**
 * copy_from_user_toio - copy data from user-space to mmio-space
 * @dst: the destination pointer on mmio-space
 * @src: the source pointer on user-space
 * @count: the data size to copy in bytes
 *
 * Copies the data from user-space to mmio-space.
 *
 * Returns zero if successful, or non-zero on failure.
 */
int copy_from_user_toio(volatile void __iomem *dst, const void __user *src, size_t count)
{
#if defined(__i386__) || defined(CONFIG_SPARC32)
	return copy_from_user((void __force *)dst, src, count) ? -EFAULT : 0;
#else
	char buf[256];
	while (count) {
		size_t c = count;
		if (c > sizeof(buf))
			c = sizeof(buf);
		if (copy_from_user(buf, src, c))
			return -EFAULT;
		memcpy_toio(dst, buf, c);
		count -= c;
		dst += c;
		src += c;
	}
	return 0;
#endif
}

EXPORT_SYMBOL(copy_from_user_toio);
