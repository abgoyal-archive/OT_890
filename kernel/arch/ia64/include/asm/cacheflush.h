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

#ifndef _ASM_IA64_CACHEFLUSH_H
#define _ASM_IA64_CACHEFLUSH_H

/*
 * Copyright (C) 2002 Hewlett-Packard Co
 *	David Mosberger-Tang <davidm@hpl.hp.com>
 */

#include <linux/page-flags.h>
#include <linux/bitops.h>

#include <asm/page.h>

/*
 * Cache flushing routines.  This is the kind of stuff that can be very expensive, so try
 * to avoid them whenever possible.
 */

#define flush_cache_all()			do { } while (0)
#define flush_cache_mm(mm)			do { } while (0)
#define flush_cache_dup_mm(mm)			do { } while (0)
#define flush_cache_range(vma, start, end)	do { } while (0)
#define flush_cache_page(vma, vmaddr, pfn)	do { } while (0)
#define flush_icache_page(vma,page)		do { } while (0)
#define flush_cache_vmap(start, end)		do { } while (0)
#define flush_cache_vunmap(start, end)		do { } while (0)

#define flush_dcache_page(page)			\
do {						\
	clear_bit(PG_arch_1, &(page)->flags);	\
} while (0)

#define flush_dcache_mmap_lock(mapping)		do { } while (0)
#define flush_dcache_mmap_unlock(mapping)	do { } while (0)

extern void flush_icache_range (unsigned long start, unsigned long end);
extern void clflush_cache_range(void *addr, int size);


#define flush_icache_user_range(vma, page, user_addr, len)					\
do {												\
	unsigned long _addr = (unsigned long) page_address(page) + ((user_addr) & ~PAGE_MASK);	\
	flush_icache_range(_addr, _addr + (len));						\
} while (0)

#define copy_to_user_page(vma, page, vaddr, dst, src, len) \
do { memcpy(dst, src, len); \
     flush_icache_user_range(vma, page, vaddr, len); \
} while (0)
#define copy_from_user_page(vma, page, vaddr, dst, src, len) \
	memcpy(dst, src, len)

#endif /* _ASM_IA64_CACHEFLUSH_H */
