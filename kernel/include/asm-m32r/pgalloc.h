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

#ifndef _ASM_M32R_PGALLOC_H
#define _ASM_M32R_PGALLOC_H

#include <linux/mm.h>

#include <asm/io.h>

#define pmd_populate_kernel(mm, pmd, pte)	\
	set_pmd(pmd, __pmd(_PAGE_TABLE + __pa(pte)))

static __inline__ void pmd_populate(struct mm_struct *mm, pmd_t *pmd,
	pgtable_t pte)
{
	set_pmd(pmd, __pmd(_PAGE_TABLE + page_to_phys(pte)));
}
#define pmd_pgtable(pmd) pmd_page(pmd)

/*
 * Allocate and free page tables.
 */
static __inline__ pgd_t *pgd_alloc(struct mm_struct *mm)
{
	pgd_t *pgd = (pgd_t *)__get_free_page(GFP_KERNEL|__GFP_ZERO);

	return pgd;
}

static inline void pgd_free(struct mm_struct *mm, pgd_t *pgd)
{
	free_page((unsigned long)pgd);
}

static __inline__ pte_t *pte_alloc_one_kernel(struct mm_struct *mm,
	unsigned long address)
{
	pte_t *pte = (pte_t *)__get_free_page(GFP_KERNEL|__GFP_ZERO);

	return pte;
}

static __inline__ pgtable_t pte_alloc_one(struct mm_struct *mm,
	unsigned long address)
{
	struct page *pte = alloc_page(GFP_KERNEL|__GFP_ZERO);

	pgtable_page_ctor(pte);
	return pte;
}

static inline void pte_free_kernel(struct mm_struct *mm, pte_t *pte)
{
	free_page((unsigned long)pte);
}

static inline void pte_free(struct mm_struct *mm, pgtable_t pte)
{
	pgtable_page_dtor(pte);
	__free_page(pte);
}

#define __pte_free_tlb(tlb, pte)	pte_free((tlb)->mm, (pte))

/*
 * allocating and freeing a pmd is trivial: the 1-entry pmd is
 * inside the pgd, so has no extra memory associated with it.
 * (In the PAE case we free the pmds as part of the pgd.)
 */

#define pmd_alloc_one(mm, addr)		({ BUG(); ((pmd_t *)2); })
#define pmd_free(mm, x)			do { } while (0)
#define __pmd_free_tlb(tlb, x)		do { } while (0)
#define pgd_populate(mm, pmd, pte)	BUG()

#define check_pgt_cache()	do { } while (0)

#endif /* _ASM_M32R_PGALLOC_H */
