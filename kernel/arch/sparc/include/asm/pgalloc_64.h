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

#ifndef _SPARC64_PGALLOC_H
#define _SPARC64_PGALLOC_H

#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/mm.h>
#include <linux/slab.h>
#include <linux/quicklist.h>

#include <asm/spitfire.h>
#include <asm/cpudata.h>
#include <asm/cacheflush.h>
#include <asm/page.h>

/* Page table allocation/freeing. */

static inline pgd_t *pgd_alloc(struct mm_struct *mm)
{
	return quicklist_alloc(0, GFP_KERNEL, NULL);
}

static inline void pgd_free(struct mm_struct *mm, pgd_t *pgd)
{
	quicklist_free(0, NULL, pgd);
}

#define pud_populate(MM, PUD, PMD)	pud_set(PUD, PMD)

static inline pmd_t *pmd_alloc_one(struct mm_struct *mm, unsigned long addr)
{
	return quicklist_alloc(0, GFP_KERNEL, NULL);
}

static inline void pmd_free(struct mm_struct *mm, pmd_t *pmd)
{
	quicklist_free(0, NULL, pmd);
}

static inline pte_t *pte_alloc_one_kernel(struct mm_struct *mm,
					  unsigned long address)
{
	return quicklist_alloc(0, GFP_KERNEL, NULL);
}

static inline pgtable_t pte_alloc_one(struct mm_struct *mm,
					unsigned long address)
{
	struct page *page;
	void *pg;

	pg = quicklist_alloc(0, GFP_KERNEL, NULL);
	if (!pg)
		return NULL;
	page = virt_to_page(pg);
	pgtable_page_ctor(page);
	return page;
}

static inline void pte_free_kernel(struct mm_struct *mm, pte_t *pte)
{
	quicklist_free(0, NULL, pte);
}

static inline void pte_free(struct mm_struct *mm, pgtable_t ptepage)
{
	pgtable_page_dtor(ptepage);
	quicklist_free_page(0, NULL, ptepage);
}


#define pmd_populate_kernel(MM, PMD, PTE)	pmd_set(PMD, PTE)
#define pmd_populate(MM,PMD,PTE_PAGE)		\
	pmd_populate_kernel(MM,PMD,page_address(PTE_PAGE))
#define pmd_pgtable(pmd) pmd_page(pmd)

static inline void check_pgt_cache(void)
{
	quicklist_trim(0, NULL, 25, 16);
}

#endif /* _SPARC64_PGALLOC_H */
