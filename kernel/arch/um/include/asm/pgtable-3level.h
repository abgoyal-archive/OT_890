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
 * Copyright 2003 PathScale Inc
 * Derived from include/asm-i386/pgtable.h
 * Licensed under the GPL
 */

#ifndef __UM_PGTABLE_3LEVEL_H
#define __UM_PGTABLE_3LEVEL_H

#include <asm-generic/pgtable-nopud.h>

/* PGDIR_SHIFT determines what a third-level page table entry can map */

#ifdef CONFIG_64BIT
#define PGDIR_SHIFT	30
#else
#define PGDIR_SHIFT	31
#endif
#define PGDIR_SIZE	(1UL << PGDIR_SHIFT)
#define PGDIR_MASK	(~(PGDIR_SIZE-1))

/* PMD_SHIFT determines the size of the area a second-level page table can
 * map
 */

#define PMD_SHIFT	21
#define PMD_SIZE	(1UL << PMD_SHIFT)
#define PMD_MASK	(~(PMD_SIZE-1))

/*
 * entries per page directory level
 */

#define PTRS_PER_PTE 512
#ifdef CONFIG_64BIT
#define PTRS_PER_PMD 512
#define PTRS_PER_PGD 512
#else
#define PTRS_PER_PMD 1024
#define PTRS_PER_PGD 1024
#endif

#define USER_PTRS_PER_PGD ((TASK_SIZE + (PGDIR_SIZE - 1)) / PGDIR_SIZE)
#define FIRST_USER_ADDRESS	0

#define pte_ERROR(e) \
        printk("%s:%d: bad pte %p(%016lx).\n", __FILE__, __LINE__, &(e), \
	       pte_val(e))
#define pmd_ERROR(e) \
        printk("%s:%d: bad pmd %p(%016lx).\n", __FILE__, __LINE__, &(e), \
	       pmd_val(e))
#define pgd_ERROR(e) \
        printk("%s:%d: bad pgd %p(%016lx).\n", __FILE__, __LINE__, &(e), \
	       pgd_val(e))

#define pud_none(x)	(!(pud_val(x) & ~_PAGE_NEWPAGE))
#define	pud_bad(x)	((pud_val(x) & (~PAGE_MASK & ~_PAGE_USER)) != _KERNPG_TABLE)
#define pud_present(x)	(pud_val(x) & _PAGE_PRESENT)
#define pud_populate(mm, pud, pmd) \
	set_pud(pud, __pud(_PAGE_TABLE + __pa(pmd)))

#ifdef CONFIG_64BIT
#define set_pud(pudptr, pudval) set_64bit((phys_t *) (pudptr), pud_val(pudval))
#else
#define set_pud(pudptr, pudval) (*(pudptr) = (pudval))
#endif

static inline int pgd_newpage(pgd_t pgd)
{
	return(pgd_val(pgd) & _PAGE_NEWPAGE);
}

static inline void pgd_mkuptodate(pgd_t pgd) { pgd_val(pgd) &= ~_PAGE_NEWPAGE; }

#ifdef CONFIG_64BIT
#define set_pmd(pmdptr, pmdval) set_64bit((phys_t *) (pmdptr), pmd_val(pmdval))
#else
#define set_pmd(pmdptr, pmdval) (*(pmdptr) = (pmdval))
#endif

struct mm_struct;
extern pmd_t *pmd_alloc_one(struct mm_struct *mm, unsigned long address);

static inline void pud_clear (pud_t *pud)
{
	set_pud(pud, __pud(_PAGE_NEWPAGE));
}

#define pud_page(pud) phys_to_page(pud_val(pud) & PAGE_MASK)
#define pud_page_vaddr(pud) ((unsigned long) __va(pud_val(pud) & PAGE_MASK))

/* Find an entry in the second-level page table.. */
#define pmd_offset(pud, address) ((pmd_t *) pud_page_vaddr(*(pud)) + \
			pmd_index(address))

static inline unsigned long pte_pfn(pte_t pte)
{
	return phys_to_pfn(pte_val(pte));
}

static inline pte_t pfn_pte(pfn_t page_nr, pgprot_t pgprot)
{
	pte_t pte;
	phys_t phys = pfn_to_phys(page_nr);

	pte_set_val(pte, phys, pgprot);
	return pte;
}

static inline pmd_t pfn_pmd(pfn_t page_nr, pgprot_t pgprot)
{
	return __pmd((page_nr << PAGE_SHIFT) | pgprot_val(pgprot));
}

/*
 * Bits 0 through 3 are taken in the low part of the pte,
 * put the 32 bits of offset into the high part.
 */
#define PTE_FILE_MAX_BITS	32

#ifdef CONFIG_64BIT

#define pte_to_pgoff(p) ((p).pte >> 32)

#define pgoff_to_pte(off) ((pte_t) { ((off) << 32) | _PAGE_FILE })

#else

#define pte_to_pgoff(pte) ((pte).pte_high)

#define pgoff_to_pte(off) ((pte_t) { _PAGE_FILE, (off) })

#endif

#endif

/*
 * Overrides for Emacs so that we follow Linus's tabbing style.
 * Emacs will notice this stuff at the end of the file and automatically
 * adjust the settings for this buffer only.  This must remain at the end
 * of the file.
 * ---------------------------------------------------------------------------
 * Local variables:
 * c-file-style: "linux"
 * End:
 */
