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

#ifndef _ALPHA_PGALLOC_H
#define _ALPHA_PGALLOC_H

#include <linux/mm.h>
#include <linux/mmzone.h>

/*      
 * Allocate and free page tables. The xxx_kernel() versions are
 * used to allocate a kernel page table - this turns on ASN bits
 * if any.
 */

static inline void
pmd_populate(struct mm_struct *mm, pmd_t *pmd, pgtable_t pte)
{
	pmd_set(pmd, (pte_t *)(page_to_pa(pte) + PAGE_OFFSET));
}
#define pmd_pgtable(pmd) pmd_page(pmd)

static inline void
pmd_populate_kernel(struct mm_struct *mm, pmd_t *pmd, pte_t *pte)
{
	pmd_set(pmd, pte);
}

static inline void
pgd_populate(struct mm_struct *mm, pgd_t *pgd, pmd_t *pmd)
{
	pgd_set(pgd, pmd);
}

extern pgd_t *pgd_alloc(struct mm_struct *mm);

static inline void
pgd_free(struct mm_struct *mm, pgd_t *pgd)
{
	free_page((unsigned long)pgd);
}

static inline pmd_t *
pmd_alloc_one(struct mm_struct *mm, unsigned long address)
{
	pmd_t *ret = (pmd_t *)__get_free_page(GFP_KERNEL|__GFP_REPEAT|__GFP_ZERO);
	return ret;
}

static inline void
pmd_free(struct mm_struct *mm, pmd_t *pmd)
{
	free_page((unsigned long)pmd);
}

static inline pte_t *
pte_alloc_one_kernel(struct mm_struct *mm, unsigned long address)
{
	pte_t *pte = (pte_t *)__get_free_page(GFP_KERNEL|__GFP_REPEAT|__GFP_ZERO);
	return pte;
}

static inline void
pte_free_kernel(struct mm_struct *mm, pte_t *pte)
{
	free_page((unsigned long)pte);
}

static inline pgtable_t
pte_alloc_one(struct mm_struct *mm, unsigned long address)
{
	pte_t *pte = pte_alloc_one_kernel(mm, address);
	struct page *page;

	if (!pte)
		return NULL;
	page = virt_to_page(pte);
	pgtable_page_ctor(page);
	return page;
}

static inline void
pte_free(struct mm_struct *mm, pgtable_t page)
{
	pgtable_page_dtor(page);
	__free_page(page);
}

#define check_pgt_cache()	do { } while (0)

#endif /* _ALPHA_PGALLOC_H */
