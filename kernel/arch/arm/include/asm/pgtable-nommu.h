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
 *  arch/arm/include/asm/pgtable-nommu.h
 *
 *  Copyright (C) 1995-2002 Russell King
 *  Copyright (C) 2004  Hyok S. Choi
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */
#ifndef _ASMARM_PGTABLE_NOMMU_H
#define _ASMARM_PGTABLE_NOMMU_H

#ifndef __ASSEMBLY__

#include <linux/slab.h>
#include <asm/processor.h>
#include <asm/page.h>

/*
 * Trivial page table functions.
 */
#define pgd_present(pgd)	(1)
#define pgd_none(pgd)		(0)
#define pgd_bad(pgd)		(0)
#define pgd_clear(pgdp)
#define kern_addr_valid(addr)	(1)
#define	pmd_offset(a, b)	((void *)0)
/* FIXME */
/*
 * PMD_SHIFT determines the size of the area a second-level page table can map
 * PGDIR_SHIFT determines what a third-level page table entry can map
 */
#define PGDIR_SHIFT		21

#define PGDIR_SIZE		(1UL << PGDIR_SHIFT)
#define PGDIR_MASK		(~(PGDIR_SIZE-1))
/* FIXME */

#define PAGE_NONE	__pgprot(0)
#define PAGE_SHARED	__pgprot(0)
#define PAGE_COPY	__pgprot(0)
#define PAGE_READONLY	__pgprot(0)
#define PAGE_KERNEL	__pgprot(0)

#define swapper_pg_dir ((pgd_t *) 0)

#define __swp_type(x)		(0)
#define __swp_offset(x)		(0)
#define __swp_entry(typ,off)	((swp_entry_t) { ((typ) | ((off) << 7)) })
#define __pte_to_swp_entry(pte)	((swp_entry_t) { pte_val(pte) })
#define __swp_entry_to_pte(x)	((pte_t) { (x).val })


typedef pte_t *pte_addr_t;

static inline int pte_file(pte_t pte) { return 0; }

/*
 * ZERO_PAGE is a global shared page that is always zero: used
 * for zero-mapped memory areas etc..
 */
#define ZERO_PAGE(vaddr)	(virt_to_page(0))

/*
 * Mark the prot value as uncacheable and unbufferable.
 */
#define pgprot_noncached(prot)	__pgprot(0)
#define pgprot_writecombine(prot) __pgprot(0)


/*
 * These would be in other places but having them here reduces the diffs.
 */
extern unsigned int kobjsize(const void *objp);

/*
 * No page table caches to initialise.
 */
#define pgtable_cache_init()	do { } while (0)
#define io_remap_page_range	remap_page_range
#define io_remap_pfn_range	remap_pfn_range


/*
 * All 32bit addresses are effectively valid for vmalloc...
 * Sort of meaningless for non-VM targets.
 */
#define	VMALLOC_START	0
#define	VMALLOC_END	0xffffffff

#define FIRST_USER_ADDRESS      (0)

#include <asm-generic/pgtable.h>

#else 

/*
 * dummy tlb and user structures.
 */
#define v3_tlb_fns	(0)
#define v4_tlb_fns	(0)
#define v4wb_tlb_fns	(0)
#define v4wbi_tlb_fns	(0)
#define v6wbi_tlb_fns	(0)
#define v7wbi_tlb_fns	(0)

#define v3_user_fns	(0)
#define v4_user_fns	(0)
#define v4_mc_user_fns	(0)
#define v4wb_user_fns	(0)
#define v4wt_user_fns	(0)
#define v6_user_fns	(0)
#define xscale_mc_user_fns (0)

#endif /*__ASSEMBLY__*/

#endif /* _ASMARM_PGTABLE_H */
