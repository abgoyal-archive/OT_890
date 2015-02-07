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
 * include/asm-sh/cpu-sh4/cacheflush.h
 *
 * Copyright (C) 1999 Niibe Yutaka
 * Copyright (C) 2003 Paul Mundt
 *
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 */
#ifndef __ASM_CPU_SH4_CACHEFLUSH_H
#define __ASM_CPU_SH4_CACHEFLUSH_H

/*
 *  Caches are broken on SH-4 (unless we use write-through
 *  caching; in which case they're only semi-broken),
 *  so we need them.
 */
void flush_cache_all(void);
void flush_dcache_all(void);
void flush_cache_mm(struct mm_struct *mm);
#define flush_cache_dup_mm(mm) flush_cache_mm(mm)
void flush_cache_range(struct vm_area_struct *vma, unsigned long start,
		       unsigned long end);
void flush_cache_page(struct vm_area_struct *vma, unsigned long addr,
		      unsigned long pfn);
void flush_dcache_page(struct page *pg);

#define flush_dcache_mmap_lock(mapping)		do { } while (0)
#define flush_dcache_mmap_unlock(mapping)	do { } while (0)

void flush_icache_range(unsigned long start, unsigned long end);
void flush_icache_user_range(struct vm_area_struct *vma, struct page *page,
			     unsigned long addr, int len);

#define flush_icache_page(vma,pg)		do { } while (0)

/* Initialization of P3 area for copy_user_page */
void p3_cache_init(void);

#define PG_mapped	PG_arch_1

#endif /* __ASM_CPU_SH4_CACHEFLUSH_H */
