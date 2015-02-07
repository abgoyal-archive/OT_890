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

#ifndef _SPARC_PGALLOC_H
#define _SPARC_PGALLOC_H

#include <linux/kernel.h>
#include <linux/sched.h>

#include <asm/page.h>
#include <asm/btfixup.h>

struct page;

extern struct pgtable_cache_struct {
	unsigned long *pgd_cache;
	unsigned long *pte_cache;
	unsigned long pgtable_cache_sz;
	unsigned long pgd_cache_sz;
} pgt_quicklists;
#define pgd_quicklist           (pgt_quicklists.pgd_cache)
#define pmd_quicklist           ((unsigned long *)0)
#define pte_quicklist           (pgt_quicklists.pte_cache)
#define pgtable_cache_size      (pgt_quicklists.pgtable_cache_sz)
#define pgd_cache_size		(pgt_quicklists.pgd_cache_sz)

extern void check_pgt_cache(void);
BTFIXUPDEF_CALL(void,	 do_check_pgt_cache, int, int)
#define do_check_pgt_cache(low,high) BTFIXUP_CALL(do_check_pgt_cache)(low,high)

BTFIXUPDEF_CALL(pgd_t *, get_pgd_fast, void)
#define get_pgd_fast()		BTFIXUP_CALL(get_pgd_fast)()

BTFIXUPDEF_CALL(void, free_pgd_fast, pgd_t *)
#define free_pgd_fast(pgd)	BTFIXUP_CALL(free_pgd_fast)(pgd)

#define pgd_free(mm, pgd)	free_pgd_fast(pgd)
#define pgd_alloc(mm)	get_pgd_fast()

BTFIXUPDEF_CALL(void, pgd_set, pgd_t *, pmd_t *)
#define pgd_set(pgdp,pmdp) BTFIXUP_CALL(pgd_set)(pgdp,pmdp)
#define pgd_populate(MM, PGD, PMD)      pgd_set(PGD, PMD)

BTFIXUPDEF_CALL(pmd_t *, pmd_alloc_one, struct mm_struct *, unsigned long)
#define pmd_alloc_one(mm, address)	BTFIXUP_CALL(pmd_alloc_one)(mm, address)

BTFIXUPDEF_CALL(void, free_pmd_fast, pmd_t *)
#define free_pmd_fast(pmd)	BTFIXUP_CALL(free_pmd_fast)(pmd)

#define pmd_free(mm, pmd)	free_pmd_fast(pmd)
#define __pmd_free_tlb(tlb, pmd) pmd_free((tlb)->mm, pmd)

BTFIXUPDEF_CALL(void, pmd_populate, pmd_t *, struct page *)
#define pmd_populate(MM, PMD, PTE)        BTFIXUP_CALL(pmd_populate)(PMD, PTE)
#define pmd_pgtable(pmd) pmd_page(pmd)
BTFIXUPDEF_CALL(void, pmd_set, pmd_t *, pte_t *)
#define pmd_populate_kernel(MM, PMD, PTE) BTFIXUP_CALL(pmd_set)(PMD, PTE)

BTFIXUPDEF_CALL(pgtable_t , pte_alloc_one, struct mm_struct *, unsigned long)
#define pte_alloc_one(mm, address)	BTFIXUP_CALL(pte_alloc_one)(mm, address)
BTFIXUPDEF_CALL(pte_t *, pte_alloc_one_kernel, struct mm_struct *, unsigned long)
#define pte_alloc_one_kernel(mm, addr)	BTFIXUP_CALL(pte_alloc_one_kernel)(mm, addr)

BTFIXUPDEF_CALL(void, free_pte_fast, pte_t *)
#define pte_free_kernel(mm, pte)	BTFIXUP_CALL(free_pte_fast)(pte)

BTFIXUPDEF_CALL(void, pte_free, pgtable_t )
#define pte_free(mm, pte)	BTFIXUP_CALL(pte_free)(pte)
#define __pte_free_tlb(tlb, pte)	pte_free((tlb)->mm, pte)

#endif /* _SPARC_PGALLOC_H */
