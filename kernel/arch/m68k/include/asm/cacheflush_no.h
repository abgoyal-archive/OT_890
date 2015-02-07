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

#ifndef _M68KNOMMU_CACHEFLUSH_H
#define _M68KNOMMU_CACHEFLUSH_H

/*
 * (C) Copyright 2000-2004, Greg Ungerer <gerg@snapgear.com>
 */
#include <linux/mm.h>

#define flush_cache_all()			__flush_cache_all()
#define flush_cache_mm(mm)			do { } while (0)
#define flush_cache_dup_mm(mm)			do { } while (0)
#define flush_cache_range(vma, start, end)	__flush_cache_all()
#define flush_cache_page(vma, vmaddr)		do { } while (0)
#define flush_dcache_range(start,len)		__flush_cache_all()
#define flush_dcache_page(page)			do { } while (0)
#define flush_dcache_mmap_lock(mapping)		do { } while (0)
#define flush_dcache_mmap_unlock(mapping)	do { } while (0)
#define flush_icache_range(start,len)		__flush_cache_all()
#define flush_icache_page(vma,pg)		do { } while (0)
#define flush_icache_user_range(vma,pg,adr,len)	do { } while (0)
#define flush_cache_vmap(start, end)		do { } while (0)
#define flush_cache_vunmap(start, end)		do { } while (0)

#define copy_to_user_page(vma, page, vaddr, dst, src, len) \
	memcpy(dst, src, len)
#define copy_from_user_page(vma, page, vaddr, dst, src, len) \
	memcpy(dst, src, len)

static inline void __flush_cache_all(void)
{
#ifdef CONFIG_M5407
	/*
	 *	Use cpushl to push and invalidate all cache lines.
	 *	Gas doesn't seem to know how to generate the ColdFire
	 *	cpushl instruction... Oh well, bit stuff it for now.
	 */
	__asm__ __volatile__ (
		"nop\n\t"
		"clrl	%%d0\n\t"
		"1:\n\t"
		"movel	%%d0,%%a0\n\t"
		"2:\n\t"
		".word	0xf468\n\t"
		"addl	#0x10,%%a0\n\t"
		"cmpl	#0x00000800,%%a0\n\t"
		"blt	2b\n\t"
		"addql	#1,%%d0\n\t"
		"cmpil	#4,%%d0\n\t"
		"bne	1b\n\t"
		"movel	#0xb6088500,%%d0\n\t"
		"movec	%%d0,%%CACR\n\t"
		: : : "d0", "a0" );
#endif /* CONFIG_M5407 */
#if defined(CONFIG_M523x) || defined(CONFIG_M527x)
	__asm__ __volatile__ (
		"movel	#0x81400100, %%d0\n\t"
		"movec	%%d0, %%CACR\n\t"
		"nop\n\t"
		: : : "d0" );
#endif /* CONFIG_M523x || CONFIG_M527x */
#if defined(CONFIG_M528x)
	__asm__ __volatile__ (
		"movel	#0x81000200, %%d0\n\t"
		"movec	%%d0, %%CACR\n\t"
		"nop\n\t"
		: : : "d0" );
#endif /* CONFIG_M528x */
#if defined(CONFIG_M5206) || defined(CONFIG_M5206e) || defined(CONFIG_M5272)
	__asm__ __volatile__ (
		"movel	#0x81000100, %%d0\n\t"
		"movec	%%d0, %%CACR\n\t"
		"nop\n\t"
		: : : "d0" );
#endif /* CONFIG_M5206 || CONFIG_M5206e || CONFIG_M5272 */
#ifdef CONFIG_M5249
	__asm__ __volatile__ (
		"movel	#0xa1000200, %%d0\n\t"
		"movec	%%d0, %%CACR\n\t"
		"nop\n\t"
		: : : "d0" );
#endif /* CONFIG_M5249 */
#ifdef CONFIG_M532x
	__asm__ __volatile__ (
		"movel	#0x81000200, %%d0\n\t"
		"movec	%%d0, %%CACR\n\t"
		"nop\n\t"
		: : : "d0" );
#endif /* CONFIG_M532x */
}

#endif /* _M68KNOMMU_CACHEFLUSH_H */
