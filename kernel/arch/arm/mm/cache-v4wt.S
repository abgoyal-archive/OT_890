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
 *  linux/arch/arm/mm/cache-v4wt.S
 *
 *  Copyright (C) 1997-2002 Russell king
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 *  ARMv4 write through cache operations support.
 *
 *  We assume that the write buffer is not enabled.
 */
#include <linux/linkage.h>
#include <linux/init.h>
#include <asm/page.h>
#include "proc-macros.S"

/*
 * The size of one data cache line.
 */
#define CACHE_DLINESIZE	32

/*
 * The number of data cache segments.
 */
#define CACHE_DSEGMENTS	8

/*
 * The number of lines in a cache segment.
 */
#define CACHE_DENTRIES	64

/*
 * This is the size at which it becomes more efficient to
 * clean the whole cache, rather than using the individual
 * cache line maintainence instructions.
 *
 * *** This needs benchmarking
 */
#define CACHE_DLIMIT	16384

/*
 *	flush_user_cache_all()
 *
 *	Invalidate all cache entries in a particular address
 *	space.
 */
ENTRY(v4wt_flush_user_cache_all)
	/* FALLTHROUGH */
/*
 *	flush_kern_cache_all()
 *
 *	Clean and invalidate the entire cache.
 */
ENTRY(v4wt_flush_kern_cache_all)
	mov	r2, #VM_EXEC
	mov	ip, #0
__flush_whole_cache:
	tst	r2, #VM_EXEC
	mcrne	p15, 0, ip, c7, c5, 0		@ invalidate I cache
	mcr	p15, 0, ip, c7, c6, 0		@ invalidate D cache
	mov	pc, lr

/*
 *	flush_user_cache_range(start, end, flags)
 *
 *	Clean and invalidate a range of cache entries in the specified
 *	address space.
 *
 *	- start - start address (inclusive, page aligned)
 *	- end	- end address (exclusive, page aligned)
 *	- flags	- vma_area_struct flags describing address space
 */
ENTRY(v4wt_flush_user_cache_range)
	sub	r3, r1, r0			@ calculate total size
	cmp	r3, #CACHE_DLIMIT
	bhs	__flush_whole_cache

1:	mcr	p15, 0, r0, c7, c6, 1		@ invalidate D entry
	tst	r2, #VM_EXEC
	mcrne	p15, 0, r0, c7, c5, 1		@ invalidate I entry
	add	r0, r0, #CACHE_DLINESIZE
	cmp	r0, r1
	blo	1b
	mov	pc, lr

/*
 *	coherent_kern_range(start, end)
 *
 *	Ensure coherency between the Icache and the Dcache in the
 *	region described by start.  If you have non-snooping
 *	Harvard caches, you need to implement this function.
 *
 *	- start  - virtual start address
 *	- end	 - virtual end address
 */
ENTRY(v4wt_coherent_kern_range)
	/* FALLTRHOUGH */

/*
 *	coherent_user_range(start, end)
 *
 *	Ensure coherency between the Icache and the Dcache in the
 *	region described by start.  If you have non-snooping
 *	Harvard caches, you need to implement this function.
 *
 *	- start  - virtual start address
 *	- end	 - virtual end address
 */
ENTRY(v4wt_coherent_user_range)
	bic	r0, r0, #CACHE_DLINESIZE - 1
1:	mcr	p15, 0, r0, c7, c5, 1		@ invalidate I entry
	add	r0, r0, #CACHE_DLINESIZE
	cmp	r0, r1
	blo	1b
	mov	pc, lr

/*
 *	flush_kern_dcache_page(void *page)
 *
 *	Ensure no D cache aliasing occurs, either with itself or
 *	the I cache
 *
 *	- addr	- page aligned address
 */
ENTRY(v4wt_flush_kern_dcache_page)
	mov	r2, #0
	mcr	p15, 0, r2, c7, c5, 0		@ invalidate I cache
	add	r1, r0, #PAGE_SZ
	/* fallthrough */

/*
 *	dma_inv_range(start, end)
 *
 *	Invalidate (discard) the specified virtual address range.
 *	May not write back any entries.  If 'start' or 'end'
 *	are not cache line aligned, those lines must be written
 *	back.
 *
 *	- start  - virtual start address
 *	- end	 - virtual end address
 */
ENTRY(v4wt_dma_inv_range)
	bic	r0, r0, #CACHE_DLINESIZE - 1
1:	mcr	p15, 0, r0, c7, c6, 1		@ invalidate D entry
	add	r0, r0, #CACHE_DLINESIZE
	cmp	r0, r1
	blo	1b
	/* FALLTHROUGH */

/*
 *	dma_clean_range(start, end)
 *
 *	Clean the specified virtual address range.
 *
 *	- start  - virtual start address
 *	- end	 - virtual end address
 */
ENTRY(v4wt_dma_clean_range)
	mov	pc, lr

/*
 *	dma_flush_range(start, end)
 *
 *	Clean and invalidate the specified virtual address range.
 *
 *	- start  - virtual start address
 *	- end	 - virtual end address
 */
	.globl	v4wt_dma_flush_range
	.equ	v4wt_dma_flush_range, v4wt_dma_inv_range

	__INITDATA

	.type	v4wt_cache_fns, #object
ENTRY(v4wt_cache_fns)
	.long	v4wt_flush_kern_cache_all
	.long	v4wt_flush_user_cache_all
	.long	v4wt_flush_user_cache_range
	.long	v4wt_coherent_kern_range
	.long	v4wt_coherent_user_range
	.long	v4wt_flush_kern_dcache_page
	.long	v4wt_dma_inv_range
	.long	v4wt_dma_clean_range
	.long	v4wt_dma_flush_range
	.size	v4wt_cache_fns, . - v4wt_cache_fns
