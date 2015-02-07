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
 * Copyright (c) 2000-2005 Silicon Graphics, Inc.
 * All Rights Reserved.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it would be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write the Free Software Foundation,
 * Inc.,  51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */
#include <linux/mm.h>
#include <linux/vmalloc.h>
#include <linux/highmem.h>
#include <linux/swap.h>
#include <linux/blkdev.h>
#include <linux/backing-dev.h>
#include "time.h"
#include "kmem.h"

#define MAX_VMALLOCS	6
#define MAX_SLAB_SIZE	0x20000

void *
kmem_alloc(size_t size, unsigned int __nocast flags)
{
	int	retries = 0;
	gfp_t	lflags = kmem_flags_convert(flags);
	void	*ptr;

#ifdef DEBUG
	if (unlikely(!(flags & KM_LARGE) && (size > PAGE_SIZE))) {
		printk(KERN_WARNING "Large %s attempt, size=%ld\n",
			__func__, (long)size);
		dump_stack();
	}
#endif

	do {
		if (size < MAX_SLAB_SIZE || retries > MAX_VMALLOCS)
			ptr = kmalloc(size, lflags);
		else
			ptr = __vmalloc(size, lflags, PAGE_KERNEL);
		if (ptr || (flags & (KM_MAYFAIL|KM_NOSLEEP)))
			return ptr;
		if (!(++retries % 100))
			printk(KERN_ERR "XFS: possible memory allocation "
					"deadlock in %s (mode:0x%x)\n",
					__func__, lflags);
		congestion_wait(WRITE, HZ/50);
	} while (1);
}

void *
kmem_zalloc(size_t size, unsigned int __nocast flags)
{
	void	*ptr;

	ptr = kmem_alloc(size, flags);
	if (ptr)
		memset((char *)ptr, 0, (int)size);
	return ptr;
}

void *
kmem_zalloc_greedy(size_t *size, size_t minsize, size_t maxsize,
		   unsigned int __nocast flags)
{
	void		*ptr;
	size_t		kmsize = maxsize;
	unsigned int	kmflags = (flags & ~KM_SLEEP) | KM_NOSLEEP;

	while (!(ptr = kmem_zalloc(kmsize, kmflags))) {
		if ((kmsize <= minsize) && (flags & KM_NOSLEEP))
			break;
		if ((kmsize >>= 1) <= minsize) {
			kmsize = minsize;
			kmflags = flags;
		}
	}
	if (ptr)
		*size = kmsize;
	return ptr;
}

void
kmem_free(const void *ptr)
{
	if (!is_vmalloc_addr(ptr)) {
		kfree(ptr);
	} else {
		vfree(ptr);
	}
}

void *
kmem_realloc(const void *ptr, size_t newsize, size_t oldsize,
	     unsigned int __nocast flags)
{
	void	*new;

	new = kmem_alloc(newsize, flags);
	if (ptr) {
		if (new)
			memcpy(new, ptr,
				((oldsize < newsize) ? oldsize : newsize));
		kmem_free(ptr);
	}
	return new;
}

void *
kmem_zone_alloc(kmem_zone_t *zone, unsigned int __nocast flags)
{
	int	retries = 0;
	gfp_t	lflags = kmem_flags_convert(flags);
	void	*ptr;

	do {
		ptr = kmem_cache_alloc(zone, lflags);
		if (ptr || (flags & (KM_MAYFAIL|KM_NOSLEEP)))
			return ptr;
		if (!(++retries % 100))
			printk(KERN_ERR "XFS: possible memory allocation "
					"deadlock in %s (mode:0x%x)\n",
					__func__, lflags);
		congestion_wait(WRITE, HZ/50);
	} while (1);
}

void *
kmem_zone_zalloc(kmem_zone_t *zone, unsigned int __nocast flags)
{
	void	*ptr;

	ptr = kmem_zone_alloc(zone, flags);
	if (ptr)
		memset((char *)ptr, 0, kmem_cache_size(zone));
	return ptr;
}
