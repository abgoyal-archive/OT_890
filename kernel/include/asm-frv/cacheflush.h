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

/* cacheflush.h: FRV cache flushing routines
 *
 * Copyright (C) 2004 Red Hat, Inc. All Rights Reserved.
 * Written by David Howells (dhowells@redhat.com)
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version
 * 2 of the License, or (at your option) any later version.
 */

#ifndef _ASM_CACHEFLUSH_H
#define _ASM_CACHEFLUSH_H

/* Keep includes the same across arches.  */
#include <linux/mm.h>

/*
 * virtually-indexed cache management (our cache is physically indexed)
 */
#define flush_cache_all()			do {} while(0)
#define flush_cache_mm(mm)			do {} while(0)
#define flush_cache_dup_mm(mm)			do {} while(0)
#define flush_cache_range(mm, start, end)	do {} while(0)
#define flush_cache_page(vma, vmaddr, pfn)	do {} while(0)
#define flush_cache_vmap(start, end)		do {} while(0)
#define flush_cache_vunmap(start, end)		do {} while(0)
#define flush_dcache_mmap_lock(mapping)		do {} while(0)
#define flush_dcache_mmap_unlock(mapping)	do {} while(0)

/*
 * physically-indexed cache management
 * - see arch/frv/lib/cache.S
 */
extern void frv_dcache_writeback(unsigned long start, unsigned long size);
extern void frv_cache_invalidate(unsigned long start, unsigned long size);
extern void frv_icache_invalidate(unsigned long start, unsigned long size);
extern void frv_cache_wback_inv(unsigned long start, unsigned long size);

static inline void __flush_cache_all(void)
{
	asm volatile("	dcef	@(gr0,gr0),#1	\n"
		     "	icei	@(gr0,gr0),#1	\n"
		     "	membar			\n"
		     : : : "memory"
		     );
}

/* dcache/icache coherency... */
#ifdef CONFIG_MMU
extern void flush_dcache_page(struct page *page);
#else
static inline void flush_dcache_page(struct page *page)
{
	unsigned long addr = page_to_phys(page);
	frv_dcache_writeback(addr, addr + PAGE_SIZE);
}
#endif

static inline void flush_page_to_ram(struct page *page)
{
	flush_dcache_page(page);
}

static inline void flush_icache(void)
{
	__flush_cache_all();
}

static inline void flush_icache_range(unsigned long start, unsigned long end)
{
	frv_cache_wback_inv(start, end);
}

#ifdef CONFIG_MMU
extern void flush_icache_user_range(struct vm_area_struct *vma, struct page *page,
				    unsigned long start, unsigned long len);
#else
static inline void flush_icache_user_range(struct vm_area_struct *vma, struct page *page,
					   unsigned long start, unsigned long len)
{
	frv_cache_wback_inv(start, start + len);
}
#endif

static inline void flush_icache_page(struct vm_area_struct *vma, struct page *page)
{
	flush_icache_user_range(vma, page, page_to_phys(page), PAGE_SIZE);
}

/*
 * permit ptrace to access another process's address space through the icache
 * and the dcache
 */
#define copy_to_user_page(vma, page, vaddr, dst, src, len)	\
do {								\
	memcpy((dst), (src), (len));				\
	flush_icache_user_range((vma), (page), (vaddr), (len));	\
} while(0)

#define copy_from_user_page(vma, page, vaddr, dst, src, len)	\
	memcpy((dst), (src), (len))

#endif /* _ASM_CACHEFLUSH_H */
