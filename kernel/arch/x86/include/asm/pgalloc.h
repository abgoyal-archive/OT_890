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

#ifndef _ASM_X86_PGALLOC_H
#define _ASM_X86_PGALLOC_H

#include <linux/threads.h>
#include <linux/mm.h>		/* for struct page */
#include <linux/pagemap.h>

static inline int  __paravirt_pgd_alloc(struct mm_struct *mm) { return 0; }

#ifdef CONFIG_PARAVIRT
#include <asm/paravirt.h>
#else
#define paravirt_pgd_alloc(mm)	__paravirt_pgd_alloc(mm)
static inline void paravirt_pgd_free(struct mm_struct *mm, pgd_t *pgd) {}
static inline void paravirt_alloc_pte(struct mm_struct *mm, unsigned long pfn)	{}
static inline void paravirt_alloc_pmd(struct mm_struct *mm, unsigned long pfn)	{}
static inline void paravirt_alloc_pmd_clone(unsigned long pfn, unsigned long clonepfn,
					    unsigned long start, unsigned long count) {}
static inline void paravirt_alloc_pud(struct mm_struct *mm, unsigned long pfn)	{}
static inline void paravirt_release_pte(unsigned long pfn) {}
static inline void paravirt_release_pmd(unsigned long pfn) {}
static inline void paravirt_release_pud(unsigned long pfn) {}
#endif

/*
 * Allocate and free page tables.
 */
extern pgd_t *pgd_alloc(struct mm_struct *);
extern void pgd_free(struct mm_struct *mm, pgd_t *pgd);

extern pte_t *pte_alloc_one_kernel(struct mm_struct *, unsigned long);
extern pgtable_t pte_alloc_one(struct mm_struct *, unsigned long);

/* Should really implement gc for free page table pages. This could be
   done with a reference count in struct page. */

static inline void pte_free_kernel(struct mm_struct *mm, pte_t *pte)
{
	BUG_ON((unsigned long)pte & (PAGE_SIZE-1));
	free_page((unsigned long)pte);
}

static inline void pte_free(struct mm_struct *mm, struct page *pte)
{
	pgtable_page_dtor(pte);
	__free_page(pte);
}

extern void __pte_free_tlb(struct mmu_gather *tlb, struct page *pte);

static inline void pmd_populate_kernel(struct mm_struct *mm,
				       pmd_t *pmd, pte_t *pte)
{
	paravirt_alloc_pte(mm, __pa(pte) >> PAGE_SHIFT);
	set_pmd(pmd, __pmd(__pa(pte) | _PAGE_TABLE));
}

static inline void pmd_populate(struct mm_struct *mm, pmd_t *pmd,
				struct page *pte)
{
	unsigned long pfn = page_to_pfn(pte);

	paravirt_alloc_pte(mm, pfn);
	set_pmd(pmd, __pmd(((pteval_t)pfn << PAGE_SHIFT) | _PAGE_TABLE));
}

#define pmd_pgtable(pmd) pmd_page(pmd)

#if PAGETABLE_LEVELS > 2
static inline pmd_t *pmd_alloc_one(struct mm_struct *mm, unsigned long addr)
{
	return (pmd_t *)get_zeroed_page(GFP_KERNEL|__GFP_REPEAT);
}

static inline void pmd_free(struct mm_struct *mm, pmd_t *pmd)
{
	BUG_ON((unsigned long)pmd & (PAGE_SIZE-1));
	free_page((unsigned long)pmd);
}

extern void __pmd_free_tlb(struct mmu_gather *tlb, pmd_t *pmd);

#ifdef CONFIG_X86_PAE
extern void pud_populate(struct mm_struct *mm, pud_t *pudp, pmd_t *pmd);
#else	/* !CONFIG_X86_PAE */
static inline void pud_populate(struct mm_struct *mm, pud_t *pud, pmd_t *pmd)
{
	paravirt_alloc_pmd(mm, __pa(pmd) >> PAGE_SHIFT);
	set_pud(pud, __pud(_PAGE_TABLE | __pa(pmd)));
}
#endif	/* CONFIG_X86_PAE */

#if PAGETABLE_LEVELS > 3
static inline void pgd_populate(struct mm_struct *mm, pgd_t *pgd, pud_t *pud)
{
	paravirt_alloc_pud(mm, __pa(pud) >> PAGE_SHIFT);
	set_pgd(pgd, __pgd(_PAGE_TABLE | __pa(pud)));
}

static inline pud_t *pud_alloc_one(struct mm_struct *mm, unsigned long addr)
{
	return (pud_t *)get_zeroed_page(GFP_KERNEL|__GFP_REPEAT);
}

static inline void pud_free(struct mm_struct *mm, pud_t *pud)
{
	BUG_ON((unsigned long)pud & (PAGE_SIZE-1));
	free_page((unsigned long)pud);
}

extern void __pud_free_tlb(struct mmu_gather *tlb, pud_t *pud);
#endif	/* PAGETABLE_LEVELS > 3 */
#endif	/* PAGETABLE_LEVELS > 2 */

#endif /* _ASM_X86_PGALLOC_H */
