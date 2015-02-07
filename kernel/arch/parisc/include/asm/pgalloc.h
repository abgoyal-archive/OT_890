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

#ifndef _ASM_PGALLOC_H
#define _ASM_PGALLOC_H

#include <linux/gfp.h>
#include <linux/mm.h>
#include <linux/threads.h>
#include <asm/processor.h>
#include <asm/fixmap.h>

#include <asm/cache.h>

/* Allocate the top level pgd (page directory)
 *
 * Here (for 64 bit kernels) we implement a Hybrid L2/L3 scheme: we
 * allocate the first pmd adjacent to the pgd.  This means that we can
 * subtract a constant offset to get to it.  The pmd and pgd sizes are
 * arranged so that a single pmd covers 4GB (giving a full 64-bit
 * process access to 8TB) so our lookups are effectively L2 for the
 * first 4GB of the kernel (i.e. for all ILP32 processes and all the
 * kernel for machines with under 4GB of memory) */
static inline pgd_t *pgd_alloc(struct mm_struct *mm)
{
	pgd_t *pgd = (pgd_t *)__get_free_pages(GFP_KERNEL,
					       PGD_ALLOC_ORDER);
	pgd_t *actual_pgd = pgd;

	if (likely(pgd != NULL)) {
		memset(pgd, 0, PAGE_SIZE<<PGD_ALLOC_ORDER);
#ifdef CONFIG_64BIT
		actual_pgd += PTRS_PER_PGD;
		/* Populate first pmd with allocated memory.  We mark it
		 * with PxD_FLAG_ATTACHED as a signal to the system that this
		 * pmd entry may not be cleared. */
		__pgd_val_set(*actual_pgd, (PxD_FLAG_PRESENT | 
				        PxD_FLAG_VALID | 
					PxD_FLAG_ATTACHED) 
			+ (__u32)(__pa((unsigned long)pgd) >> PxD_VALUE_SHIFT));
		/* The first pmd entry also is marked with _PAGE_GATEWAY as
		 * a signal that this pmd may not be freed */
		__pgd_val_set(*pgd, PxD_FLAG_ATTACHED);
#endif
	}
	return actual_pgd;
}

static inline void pgd_free(struct mm_struct *mm, pgd_t *pgd)
{
#ifdef CONFIG_64BIT
	pgd -= PTRS_PER_PGD;
#endif
	free_pages((unsigned long)pgd, PGD_ALLOC_ORDER);
}

#if PT_NLEVELS == 3

/* Three Level Page Table Support for pmd's */

static inline void pgd_populate(struct mm_struct *mm, pgd_t *pgd, pmd_t *pmd)
{
	__pgd_val_set(*pgd, (PxD_FLAG_PRESENT | PxD_FLAG_VALID) +
		        (__u32)(__pa((unsigned long)pmd) >> PxD_VALUE_SHIFT));
}

static inline pmd_t *pmd_alloc_one(struct mm_struct *mm, unsigned long address)
{
	pmd_t *pmd = (pmd_t *)__get_free_pages(GFP_KERNEL|__GFP_REPEAT,
					       PMD_ORDER);
	if (pmd)
		memset(pmd, 0, PAGE_SIZE<<PMD_ORDER);
	return pmd;
}

static inline void pmd_free(struct mm_struct *mm, pmd_t *pmd)
{
#ifdef CONFIG_64BIT
	if(pmd_flag(*pmd) & PxD_FLAG_ATTACHED)
		/* This is the permanent pmd attached to the pgd;
		 * cannot free it */
		return;
#endif
	free_pages((unsigned long)pmd, PMD_ORDER);
}

#else

/* Two Level Page Table Support for pmd's */

/*
 * allocating and freeing a pmd is trivial: the 1-entry pmd is
 * inside the pgd, so has no extra memory associated with it.
 */

#define pmd_alloc_one(mm, addr)		({ BUG(); ((pmd_t *)2); })
#define pmd_free(mm, x)			do { } while (0)
#define pgd_populate(mm, pmd, pte)	BUG()

#endif

static inline void
pmd_populate_kernel(struct mm_struct *mm, pmd_t *pmd, pte_t *pte)
{
#ifdef CONFIG_64BIT
	/* preserve the gateway marker if this is the beginning of
	 * the permanent pmd */
	if(pmd_flag(*pmd) & PxD_FLAG_ATTACHED)
		__pmd_val_set(*pmd, (PxD_FLAG_PRESENT |
				 PxD_FLAG_VALID |
				 PxD_FLAG_ATTACHED) 
			+ (__u32)(__pa((unsigned long)pte) >> PxD_VALUE_SHIFT));
	else
#endif
		__pmd_val_set(*pmd, (PxD_FLAG_PRESENT | PxD_FLAG_VALID) 
			+ (__u32)(__pa((unsigned long)pte) >> PxD_VALUE_SHIFT));
}

#define pmd_populate(mm, pmd, pte_page) \
	pmd_populate_kernel(mm, pmd, page_address(pte_page))
#define pmd_pgtable(pmd) pmd_page(pmd)

static inline pgtable_t
pte_alloc_one(struct mm_struct *mm, unsigned long address)
{
	struct page *page = alloc_page(GFP_KERNEL|__GFP_REPEAT|__GFP_ZERO);
	if (page)
		pgtable_page_ctor(page);
	return page;
}

static inline pte_t *
pte_alloc_one_kernel(struct mm_struct *mm, unsigned long addr)
{
	pte_t *pte = (pte_t *)__get_free_page(GFP_KERNEL|__GFP_REPEAT|__GFP_ZERO);
	return pte;
}

static inline void pte_free_kernel(struct mm_struct *mm, pte_t *pte)
{
	free_page((unsigned long)pte);
}

static inline void pte_free(struct mm_struct *mm, struct page *pte)
{
	pgtable_page_dtor(pte);
	pte_free_kernel(mm, page_address(pte));
}

#define check_pgt_cache()	do { } while (0)

#endif
