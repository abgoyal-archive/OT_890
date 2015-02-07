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
 * Copyright (C) 2004-2006 Atmel Corporation
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */
#ifndef __ASM_AVR32_PGALLOC_H
#define __ASM_AVR32_PGALLOC_H

#include <linux/quicklist.h>
#include <asm/page.h>
#include <asm/pgtable.h>

#define QUICK_PGD	0	/* Preserve kernel mappings over free */
#define QUICK_PT	1	/* Zero on free */

static inline void pmd_populate_kernel(struct mm_struct *mm,
				       pmd_t *pmd, pte_t *pte)
{
	set_pmd(pmd, __pmd((unsigned long)pte));
}

static inline void pmd_populate(struct mm_struct *mm, pmd_t *pmd,
				    pgtable_t pte)
{
	set_pmd(pmd, __pmd((unsigned long)page_address(pte)));
}
#define pmd_pgtable(pmd) pmd_page(pmd)

static inline void pgd_ctor(void *x)
{
	pgd_t *pgd = x;

	memcpy(pgd + USER_PTRS_PER_PGD,
		swapper_pg_dir + USER_PTRS_PER_PGD,
		(PTRS_PER_PGD - USER_PTRS_PER_PGD) * sizeof(pgd_t));
}

/*
 * Allocate and free page tables
 */
static inline pgd_t *pgd_alloc(struct mm_struct *mm)
{
	return quicklist_alloc(QUICK_PGD, GFP_KERNEL | __GFP_REPEAT, pgd_ctor);
}

static inline void pgd_free(struct mm_struct *mm, pgd_t *pgd)
{
	quicklist_free(QUICK_PGD, NULL, pgd);
}

static inline pte_t *pte_alloc_one_kernel(struct mm_struct *mm,
					  unsigned long address)
{
	return quicklist_alloc(QUICK_PT, GFP_KERNEL | __GFP_REPEAT, NULL);
}

static inline pgtable_t pte_alloc_one(struct mm_struct *mm,
					 unsigned long address)
{
	struct page *page;
	void *pg;

	pg = quicklist_alloc(QUICK_PT, GFP_KERNEL | __GFP_REPEAT, NULL);
	if (!pg)
		return NULL;

	page = virt_to_page(pg);
	pgtable_page_ctor(page);

	return page;
}

static inline void pte_free_kernel(struct mm_struct *mm, pte_t *pte)
{
	quicklist_free(QUICK_PT, NULL, pte);
}

static inline void pte_free(struct mm_struct *mm, pgtable_t pte)
{
	pgtable_page_dtor(pte);
	quicklist_free_page(QUICK_PT, NULL, pte);
}

#define __pte_free_tlb(tlb,pte)				\
do {							\
	pgtable_page_dtor(pte);				\
	tlb_remove_page((tlb), pte);			\
} while (0)

static inline void check_pgt_cache(void)
{
	quicklist_trim(QUICK_PGD, NULL, 25, 16);
	quicklist_trim(QUICK_PT, NULL, 25, 16);
}

#endif /* __ASM_AVR32_PGALLOC_H */
