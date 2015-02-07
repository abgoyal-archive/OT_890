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

#ifndef _MOTOROLA_PGALLOC_H
#define _MOTOROLA_PGALLOC_H

#include <asm/tlb.h>
#include <asm/tlbflush.h>

extern pmd_t *get_pointer_table(void);
extern int free_pointer_table(pmd_t *);

static inline pte_t *pte_alloc_one_kernel(struct mm_struct *mm, unsigned long address)
{
	pte_t *pte;

	pte = (pte_t *)__get_free_page(GFP_KERNEL|__GFP_REPEAT|__GFP_ZERO);
	if (pte) {
		__flush_page_to_ram(pte);
		flush_tlb_kernel_page(pte);
		nocache_page(pte);
	}

	return pte;
}

static inline void pte_free_kernel(struct mm_struct *mm, pte_t *pte)
{
	cache_page(pte);
	free_page((unsigned long) pte);
}

static inline pgtable_t pte_alloc_one(struct mm_struct *mm, unsigned long address)
{
	struct page *page = alloc_pages(GFP_KERNEL|__GFP_REPEAT|__GFP_ZERO, 0);
	pte_t *pte;

	if(!page)
		return NULL;

	pte = kmap(page);
	if (pte) {
		__flush_page_to_ram(pte);
		flush_tlb_kernel_page(pte);
		nocache_page(pte);
	}
	kunmap(pte);
	pgtable_page_ctor(page);
	return page;
}

static inline void pte_free(struct mm_struct *mm, pgtable_t page)
{
	pgtable_page_dtor(page);
	cache_page(kmap(page));
	kunmap(page);
	__free_page(page);
}

static inline void __pte_free_tlb(struct mmu_gather *tlb, pgtable_t page)
{
	pgtable_page_dtor(page);
	cache_page(kmap(page));
	kunmap(page);
	__free_page(page);
}


static inline pmd_t *pmd_alloc_one(struct mm_struct *mm, unsigned long address)
{
	return get_pointer_table();
}

static inline int pmd_free(struct mm_struct *mm, pmd_t *pmd)
{
	return free_pointer_table(pmd);
}

static inline int __pmd_free_tlb(struct mmu_gather *tlb, pmd_t *pmd)
{
	return free_pointer_table(pmd);
}


static inline void pgd_free(struct mm_struct *mm, pgd_t *pgd)
{
	pmd_free(mm, (pmd_t *)pgd);
}

static inline pgd_t *pgd_alloc(struct mm_struct *mm)
{
	return (pgd_t *)get_pointer_table();
}


static inline void pmd_populate_kernel(struct mm_struct *mm, pmd_t *pmd, pte_t *pte)
{
	pmd_set(pmd, pte);
}

static inline void pmd_populate(struct mm_struct *mm, pmd_t *pmd, pgtable_t page)
{
	pmd_set(pmd, page_address(page));
}
#define pmd_pgtable(pmd) pmd_page(pmd)

static inline void pgd_populate(struct mm_struct *mm, pgd_t *pgd, pmd_t *pmd)
{
	pgd_set(pgd, pmd);
}

#endif /* _MOTOROLA_PGALLOC_H */
