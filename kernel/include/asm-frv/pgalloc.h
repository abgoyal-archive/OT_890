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

/* pgalloc.h: Page allocation routines for FRV
 *
 * Copyright (C) 2004 Red Hat, Inc. All Rights Reserved.
 * Written by David Howells (dhowells@redhat.com)
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version
 * 2 of the License, or (at your option) any later version.
 *
 * Derived from:
 *	include/asm-m68knommu/pgalloc.h
 *	include/asm-i386/pgalloc.h
 */
#ifndef _ASM_PGALLOC_H
#define _ASM_PGALLOC_H

#include <asm/setup.h>
#include <asm/virtconvert.h>

#ifdef CONFIG_MMU

#define pmd_populate_kernel(mm, pmd, pte) __set_pmd(pmd, __pa(pte) | _PAGE_TABLE)
#define pmd_populate(MM, PMD, PAGE)						\
do {										\
	__set_pmd((PMD), page_to_pfn(PAGE) << PAGE_SHIFT | _PAGE_TABLE);	\
} while(0)
#define pmd_pgtable(pmd) pmd_page(pmd)

/*
 * Allocate and free page tables.
 */

extern pgd_t *pgd_alloc(struct mm_struct *);
extern void pgd_free(struct mm_struct *mm, pgd_t *);

extern pte_t *pte_alloc_one_kernel(struct mm_struct *, unsigned long);

extern pgtable_t pte_alloc_one(struct mm_struct *, unsigned long);

static inline void pte_free_kernel(struct mm_struct *mm, pte_t *pte)
{
	free_page((unsigned long)pte);
}

static inline void pte_free(struct mm_struct *mm, pgtable_t pte)
{
	pgtable_page_dtor(pte);
	__free_page(pte);
}

#define __pte_free_tlb(tlb,pte)				\
do {							\
	pgtable_page_dtor(pte);				\
	tlb_remove_page((tlb),(pte));			\
} while (0)

/*
 * allocating and freeing a pmd is trivial: the 1-entry pmd is
 * inside the pgd, so has no extra memory associated with it.
 * (In the PAE case we free the pmds as part of the pgd.)
 */
#define pmd_alloc_one(mm, addr)		({ BUG(); ((pmd_t *) 2); })
#define pmd_free(mm, x)			do { } while (0)
#define __pmd_free_tlb(tlb,x)		do { } while (0)

#endif /* CONFIG_MMU */

#endif /* _ASM_PGALLOC_H */
