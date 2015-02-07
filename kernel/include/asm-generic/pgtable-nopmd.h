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

#ifndef _PGTABLE_NOPMD_H
#define _PGTABLE_NOPMD_H

#ifndef __ASSEMBLY__

#include <asm-generic/pgtable-nopud.h>

struct mm_struct;

#define __PAGETABLE_PMD_FOLDED

/*
 * Having the pmd type consist of a pud gets the size right, and allows
 * us to conceptually access the pud entry that this pmd is folded into
 * without casting.
 */
typedef struct { pud_t pud; } pmd_t;

#define PMD_SHIFT	PUD_SHIFT
#define PTRS_PER_PMD	1
#define PMD_SIZE  	(1UL << PMD_SHIFT)
#define PMD_MASK  	(~(PMD_SIZE-1))

/*
 * The "pud_xxx()" functions here are trivial for a folded two-level
 * setup: the pmd is never bad, and a pmd always exists (as it's folded
 * into the pud entry)
 */
static inline int pud_none(pud_t pud)		{ return 0; }
static inline int pud_bad(pud_t pud)		{ return 0; }
static inline int pud_present(pud_t pud)	{ return 1; }
static inline void pud_clear(pud_t *pud)	{ }
#define pmd_ERROR(pmd)				(pud_ERROR((pmd).pud))

#define pud_populate(mm, pmd, pte)		do { } while (0)

/*
 * (pmds are folded into puds so this doesn't get actually called,
 * but the define is needed for a generic inline function.)
 */
#define set_pud(pudptr, pudval)			set_pmd((pmd_t *)(pudptr), (pmd_t) { pudval })

static inline pmd_t * pmd_offset(pud_t * pud, unsigned long address)
{
	return (pmd_t *)pud;
}

#define pmd_val(x)				(pud_val((x).pud))
#define __pmd(x)				((pmd_t) { __pud(x) } )

#define pud_page(pud)				(pmd_page((pmd_t){ pud }))
#define pud_page_vaddr(pud)			(pmd_page_vaddr((pmd_t){ pud }))

/*
 * allocating and freeing a pmd is trivial: the 1-entry pmd is
 * inside the pud, so has no extra memory associated with it.
 */
#define pmd_alloc_one(mm, address)		NULL
static inline void pmd_free(struct mm_struct *mm, pmd_t *pmd)
{
}
#define __pmd_free_tlb(tlb, x)			do { } while (0)

#undef  pmd_addr_end
#define pmd_addr_end(addr, end)			(end)

#endif /* __ASSEMBLY__ */

#endif /* _PGTABLE_NOPMD_H */
