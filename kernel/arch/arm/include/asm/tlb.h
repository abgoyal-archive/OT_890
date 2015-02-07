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
 *  arch/arm/include/asm/tlb.h
 *
 *  Copyright (C) 2002 Russell King
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 *  Experimentation shows that on a StrongARM, it appears to be faster
 *  to use the "invalidate whole tlb" rather than "invalidate single
 *  tlb" for this.
 *
 *  This appears true for both the process fork+exit case, as well as
 *  the munmap-large-area case.
 */
#ifndef __ASMARM_TLB_H
#define __ASMARM_TLB_H

#include <asm/cacheflush.h>
#include <asm/tlbflush.h>

#ifndef CONFIG_MMU

#include <linux/pagemap.h>
#include <asm-generic/tlb.h>

#else /* !CONFIG_MMU */

#include <asm/pgalloc.h>

/*
 * TLB handling.  This allows us to remove pages from the page
 * tables, and efficiently handle the TLB issues.
 */
struct mmu_gather {
	struct mm_struct	*mm;
	unsigned int		fullmm;
};

DECLARE_PER_CPU(struct mmu_gather, mmu_gathers);

static inline struct mmu_gather *
tlb_gather_mmu(struct mm_struct *mm, unsigned int full_mm_flush)
{
	struct mmu_gather *tlb = &get_cpu_var(mmu_gathers);

	tlb->mm = mm;
	tlb->fullmm = full_mm_flush;

	return tlb;
}

static inline void
tlb_finish_mmu(struct mmu_gather *tlb, unsigned long start, unsigned long end)
{
	if (tlb->fullmm)
		flush_tlb_mm(tlb->mm);

	/* keep the page table cache within bounds */
	check_pgt_cache();

	put_cpu_var(mmu_gathers);
}

#define tlb_remove_tlb_entry(tlb,ptep,address)	do { } while (0)

/*
 * In the case of tlb vma handling, we can optimise these away in the
 * case where we're doing a full MM flush.  When we're doing a munmap,
 * the vmas are adjusted to only cover the region to be torn down.
 */
static inline void
tlb_start_vma(struct mmu_gather *tlb, struct vm_area_struct *vma)
{
	if (!tlb->fullmm)
		flush_cache_range(vma, vma->vm_start, vma->vm_end);
}

static inline void
tlb_end_vma(struct mmu_gather *tlb, struct vm_area_struct *vma)
{
	if (!tlb->fullmm)
		flush_tlb_range(vma, vma->vm_start, vma->vm_end);
}

#define tlb_remove_page(tlb,page)	free_page_and_swap_cache(page)
#define pte_free_tlb(tlb, ptep)		pte_free((tlb)->mm, ptep)
#define pmd_free_tlb(tlb, pmdp)		pmd_free((tlb)->mm, pmdp)

#define tlb_migrate_finish(mm)		do { } while (0)

#endif /* CONFIG_MMU */
#endif
