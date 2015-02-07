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

#ifndef _ASM_POWERPC_PGALLOC_32_H
#define _ASM_POWERPC_PGALLOC_32_H

#include <linux/threads.h>

#define PTE_NONCACHE_NUM	0  /* dummy for now to share code w/ppc64 */

extern void __bad_pte(pmd_t *pmd);

extern pgd_t *pgd_alloc(struct mm_struct *mm);
extern void pgd_free(struct mm_struct *mm, pgd_t *pgd);

/*
 * We don't have any real pmd's, and this code never triggers because
 * the pgd will always be present..
 */
/* #define pmd_alloc_one(mm,address)       ({ BUG(); ((pmd_t *)2); }) */
#define pmd_free(mm, x) 		do { } while (0)
#define __pmd_free_tlb(tlb,x)		do { } while (0)
/* #define pgd_populate(mm, pmd, pte)      BUG() */

#ifndef CONFIG_BOOKE
#define pmd_populate_kernel(mm, pmd, pte)	\
		(pmd_val(*(pmd)) = __pa(pte) | _PMD_PRESENT)
#define pmd_populate(mm, pmd, pte)	\
		(pmd_val(*(pmd)) = (page_to_pfn(pte) << PAGE_SHIFT) | _PMD_PRESENT)
#define pmd_pgtable(pmd) pmd_page(pmd)
#else
#define pmd_populate_kernel(mm, pmd, pte)	\
		(pmd_val(*(pmd)) = (unsigned long)pte | _PMD_PRESENT)
#define pmd_populate(mm, pmd, pte)	\
		(pmd_val(*(pmd)) = (unsigned long)lowmem_page_address(pte) | _PMD_PRESENT)
#define pmd_pgtable(pmd) pmd_page(pmd)
#endif

extern pte_t *pte_alloc_one_kernel(struct mm_struct *mm, unsigned long addr);
extern pgtable_t pte_alloc_one(struct mm_struct *mm, unsigned long addr);

static inline void pgtable_free(pgtable_free_t pgf)
{
	void *p = (void *)(pgf.val & ~PGF_CACHENUM_MASK);

	free_page((unsigned long)p);
}

#define check_pgt_cache()	do { } while (0)

#endif /* _ASM_POWERPC_PGALLOC_32_H */
