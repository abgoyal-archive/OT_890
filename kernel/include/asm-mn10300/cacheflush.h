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

/* MN10300 Cache flushing
 *
 * Copyright (C) 2007 Red Hat, Inc. All Rights Reserved.
 * Written by David Howells (dhowells@redhat.com)
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public Licence
 * as published by the Free Software Foundation; either version
 * 2 of the Licence, or (at your option) any later version.
 */
#ifndef _ASM_CACHEFLUSH_H
#define _ASM_CACHEFLUSH_H

#ifndef __ASSEMBLY__

/* Keep includes the same across arches.  */
#include <linux/mm.h>

/*
 * virtually-indexed cache managment (our cache is physically indexed)
 */
#define flush_cache_all()			do {} while (0)
#define flush_cache_mm(mm)			do {} while (0)
#define flush_cache_dup_mm(mm)			do {} while (0)
#define flush_cache_range(mm, start, end)	do {} while (0)
#define flush_cache_page(vma, vmaddr, pfn)	do {} while (0)
#define flush_cache_vmap(start, end)		do {} while (0)
#define flush_cache_vunmap(start, end)		do {} while (0)
#define flush_dcache_page(page)			do {} while (0)
#define flush_dcache_mmap_lock(mapping)		do {} while (0)
#define flush_dcache_mmap_unlock(mapping)	do {} while (0)

/*
 * physically-indexed cache managment
 */
#ifndef CONFIG_MN10300_CACHE_DISABLED

extern void flush_icache_range(unsigned long start, unsigned long end);
extern void flush_icache_page(struct vm_area_struct *vma, struct page *pg);

#else

#define flush_icache_range(start, end)		do {} while (0)
#define flush_icache_page(vma, pg)		do {} while (0)

#endif

#define flush_icache_user_range(vma, pg, adr, len) \
	flush_icache_range(adr, adr + len)

#define copy_to_user_page(vma, page, vaddr, dst, src, len) \
	do {					\
		memcpy(dst, src, len);		\
		flush_icache_page(vma, page);	\
	} while (0)

#define copy_from_user_page(vma, page, vaddr, dst, src, len) \
	memcpy(dst, src, len)

/*
 * primitive routines
 */
#ifndef CONFIG_MN10300_CACHE_DISABLED
extern void mn10300_icache_inv(void);
extern void mn10300_dcache_inv(void);
extern void mn10300_dcache_inv_page(unsigned start);
extern void mn10300_dcache_inv_range(unsigned start, unsigned end);
extern void mn10300_dcache_inv_range2(unsigned start, unsigned size);
#ifdef CONFIG_MN10300_CACHE_WBACK
extern void mn10300_dcache_flush(void);
extern void mn10300_dcache_flush_page(unsigned start);
extern void mn10300_dcache_flush_range(unsigned start, unsigned end);
extern void mn10300_dcache_flush_range2(unsigned start, unsigned size);
extern void mn10300_dcache_flush_inv(void);
extern void mn10300_dcache_flush_inv_page(unsigned start);
extern void mn10300_dcache_flush_inv_range(unsigned start, unsigned end);
extern void mn10300_dcache_flush_inv_range2(unsigned start, unsigned size);
#else
#define mn10300_dcache_flush()				do {} while (0)
#define mn10300_dcache_flush_page(start)		do {} while (0)
#define mn10300_dcache_flush_range(start, end)		do {} while (0)
#define mn10300_dcache_flush_range2(start, size)	do {} while (0)
#define mn10300_dcache_flush_inv()			mn10300_dcache_inv()
#define mn10300_dcache_flush_inv_page(start) \
	mn10300_dcache_inv_page((start))
#define mn10300_dcache_flush_inv_range(start, end) \
	mn10300_dcache_inv_range((start), (end))
#define mn10300_dcache_flush_inv_range2(start, size) \
	mn10300_dcache_inv_range2((start), (size))
#endif /* CONFIG_MN10300_CACHE_WBACK */
#else
#define mn10300_icache_inv()				do {} while (0)
#define mn10300_dcache_inv()				do {} while (0)
#define mn10300_dcache_inv_page(start)			do {} while (0)
#define mn10300_dcache_inv_range(start, end)		do {} while (0)
#define mn10300_dcache_inv_range2(start, size)		do {} while (0)
#define mn10300_dcache_flush()				do {} while (0)
#define mn10300_dcache_flush_inv_page(start)		do {} while (0)
#define mn10300_dcache_flush_inv()			do {} while (0)
#define mn10300_dcache_flush_inv_range(start, end)	do {} while (0)
#define mn10300_dcache_flush_inv_range2(start, size)	do {} while (0)
#define mn10300_dcache_flush_page(start)		do {} while (0)
#define mn10300_dcache_flush_range(start, end)		do {} while (0)
#define mn10300_dcache_flush_range2(start, size)	do {} while (0)
#endif /* CONFIG_MN10300_CACHE_DISABLED */

/*
 * internal debugging function
 */
#ifdef CONFIG_DEBUG_PAGEALLOC
extern void kernel_map_pages(struct page *page, int numpages, int enable);
#endif

#endif /* __ASSEMBLY__ */

#endif /* _ASM_CACHEFLUSH_H */
