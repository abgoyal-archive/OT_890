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

/* sun3_pgalloc.h --
 * reorganization around 2.3.39, routines moved from sun3_pgtable.h
 *
 *
 * 02/27/2002 -- Modified to support "highpte" implementation in 2.5.5 (Sam)
 *
 * moved 1/26/2000 Sam Creasey
 */

#ifndef _SUN3_PGALLOC_H
#define _SUN3_PGALLOC_H

#include <asm/tlb.h>

/* FIXME - when we get this compiling */
/* erm, now that it's compiling, what do we do with it? */
#define _KERNPG_TABLE 0

extern const char bad_pmd_string[];

#define pmd_alloc_one(mm,address)       ({ BUG(); ((pmd_t *)2); })


static inline void pte_free_kernel(struct mm_struct *mm, pte_t *pte)
{
        free_page((unsigned long) pte);
}

static inline void pte_free(struct mm_struct *mm, pgtable_t page)
{
	pgtable_page_dtor(page);
        __free_page(page);
}

#define __pte_free_tlb(tlb,pte)				\
do {							\
	pgtable_page_dtor(pte);				\
	tlb_remove_page((tlb), pte);			\
} while (0)

static inline pte_t *pte_alloc_one_kernel(struct mm_struct *mm,
					  unsigned long address)
{
	unsigned long page = __get_free_page(GFP_KERNEL|__GFP_REPEAT);

	if (!page)
		return NULL;

	memset((void *)page, 0, PAGE_SIZE);
	return (pte_t *) (page);
}

static inline pgtable_t pte_alloc_one(struct mm_struct *mm,
					unsigned long address)
{
        struct page *page = alloc_pages(GFP_KERNEL|__GFP_REPEAT, 0);

	if (page == NULL)
		return NULL;

	clear_highpage(page);
	pgtable_page_ctor(page);
	return page;

}

static inline void pmd_populate_kernel(struct mm_struct *mm, pmd_t *pmd, pte_t *pte)
{
	pmd_val(*pmd) = __pa((unsigned long)pte);
}

static inline void pmd_populate(struct mm_struct *mm, pmd_t *pmd, pgtable_t page)
{
	pmd_val(*pmd) = __pa((unsigned long)page_address(page));
}
#define pmd_pgtable(pmd) pmd_page(pmd)

/*
 * allocating and freeing a pmd is trivial: the 1-entry pmd is
 * inside the pgd, so has no extra memory associated with it.
 */
#define pmd_free(mm, x)			do { } while (0)
#define __pmd_free_tlb(tlb, x)		do { } while (0)

static inline void pgd_free(struct mm_struct *mm, pgd_t *pgd)
{
        free_page((unsigned long) pgd);
}

static inline pgd_t * pgd_alloc(struct mm_struct *mm)
{
     pgd_t *new_pgd;

     new_pgd = (pgd_t *)get_zeroed_page(GFP_KERNEL);
     memcpy(new_pgd, swapper_pg_dir, PAGE_SIZE);
     memset(new_pgd, 0, (PAGE_OFFSET >> PGDIR_SHIFT));
     return new_pgd;
}

#define pgd_populate(mm, pmd, pte) BUG()

#endif /* SUN3_PGALLOC_H */
