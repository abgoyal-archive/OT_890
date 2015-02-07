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
 * Copyright (C) 2000, 2001, 2002 Jeff Dike (jdike@karaya.com)
 * Copyright 2003 PathScale, Inc.
 * Derived from include/asm-i386/pgalloc.h and include/asm-i386/pgtable.h
 * Licensed under the GPL
 */

#ifndef __UM_PGALLOC_H
#define __UM_PGALLOC_H

#include "linux/mm.h"
#include "asm/fixmap.h"

#define pmd_populate_kernel(mm, pmd, pte) \
	set_pmd(pmd, __pmd(_PAGE_TABLE + (unsigned long) __pa(pte)))

#define pmd_populate(mm, pmd, pte) 				\
	set_pmd(pmd, __pmd(_PAGE_TABLE +			\
		((unsigned long long)page_to_pfn(pte) <<	\
			(unsigned long long) PAGE_SHIFT)))
#define pmd_pgtable(pmd) pmd_page(pmd)

/*
 * Allocate and free page tables.
 */
extern pgd_t *pgd_alloc(struct mm_struct *);
extern void pgd_free(struct mm_struct *mm, pgd_t *pgd);

extern pte_t *pte_alloc_one_kernel(struct mm_struct *, unsigned long);
extern pgtable_t pte_alloc_one(struct mm_struct *, unsigned long);

static inline void pte_free_kernel(struct mm_struct *mm, pte_t *pte)
{
	free_page((unsigned long) pte);
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

#ifdef CONFIG_3_LEVEL_PGTABLES

static inline void pmd_free(struct mm_struct *mm, pmd_t *pmd)
{
	free_page((unsigned long)pmd);
}

#define __pmd_free_tlb(tlb,x)   tlb_remove_page((tlb),virt_to_page(x))
#endif

#define check_pgt_cache()	do { } while (0)

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
