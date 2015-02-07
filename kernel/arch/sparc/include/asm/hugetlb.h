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

#ifndef _ASM_SPARC64_HUGETLB_H
#define _ASM_SPARC64_HUGETLB_H

#include <asm/page.h>


void set_huge_pte_at(struct mm_struct *mm, unsigned long addr,
		     pte_t *ptep, pte_t pte);

pte_t huge_ptep_get_and_clear(struct mm_struct *mm, unsigned long addr,
			      pte_t *ptep);

void hugetlb_prefault_arch_hook(struct mm_struct *mm);

static inline int is_hugepage_only_range(struct mm_struct *mm,
					 unsigned long addr,
					 unsigned long len) {
	return 0;
}

/*
 * If the arch doesn't supply something else, assume that hugepage
 * size aligned regions are ok without further preparation.
 */
static inline int prepare_hugepage_range(struct file *file,
			unsigned long addr, unsigned long len)
{
	if (len & ~HPAGE_MASK)
		return -EINVAL;
	if (addr & ~HPAGE_MASK)
		return -EINVAL;
	return 0;
}

static inline void hugetlb_free_pgd_range(struct mmu_gather *tlb,
					  unsigned long addr, unsigned long end,
					  unsigned long floor,
					  unsigned long ceiling)
{
	free_pgd_range(tlb, addr, end, floor, ceiling);
}

static inline void huge_ptep_clear_flush(struct vm_area_struct *vma,
					 unsigned long addr, pte_t *ptep)
{
}

static inline int huge_pte_none(pte_t pte)
{
	return pte_none(pte);
}

static inline pte_t huge_pte_wrprotect(pte_t pte)
{
	return pte_wrprotect(pte);
}

static inline void huge_ptep_set_wrprotect(struct mm_struct *mm,
					   unsigned long addr, pte_t *ptep)
{
	ptep_set_wrprotect(mm, addr, ptep);
}

static inline int huge_ptep_set_access_flags(struct vm_area_struct *vma,
					     unsigned long addr, pte_t *ptep,
					     pte_t pte, int dirty)
{
	return ptep_set_access_flags(vma, addr, ptep, pte, dirty);
}

static inline pte_t huge_ptep_get(pte_t *ptep)
{
	return *ptep;
}

static inline int arch_prepare_hugepage(struct page *page)
{
	return 0;
}

static inline void arch_release_hugepage(struct page *page)
{
}

#endif /* _ASM_SPARC64_HUGETLB_H */
