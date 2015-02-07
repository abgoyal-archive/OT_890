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

#ifndef _PGTABLE_NOPUD_H
#define _PGTABLE_NOPUD_H

#ifndef __ASSEMBLY__

#define __PAGETABLE_PUD_FOLDED

/*
 * Having the pud type consist of a pgd gets the size right, and allows
 * us to conceptually access the pgd entry that this pud is folded into
 * without casting.
 */
typedef struct { pgd_t pgd; } pud_t;

#define PUD_SHIFT	PGDIR_SHIFT
#define PTRS_PER_PUD	1
#define PUD_SIZE  	(1UL << PUD_SHIFT)
#define PUD_MASK  	(~(PUD_SIZE-1))

/*
 * The "pgd_xxx()" functions here are trivial for a folded two-level
 * setup: the pud is never bad, and a pud always exists (as it's folded
 * into the pgd entry)
 */
static inline int pgd_none(pgd_t pgd)		{ return 0; }
static inline int pgd_bad(pgd_t pgd)		{ return 0; }
static inline int pgd_present(pgd_t pgd)	{ return 1; }
static inline void pgd_clear(pgd_t *pgd)	{ }
#define pud_ERROR(pud)				(pgd_ERROR((pud).pgd))

#define pgd_populate(mm, pgd, pud)		do { } while (0)
/*
 * (puds are folded into pgds so this doesn't get actually called,
 * but the define is needed for a generic inline function.)
 */
#define set_pgd(pgdptr, pgdval)			set_pud((pud_t *)(pgdptr), (pud_t) { pgdval })

static inline pud_t * pud_offset(pgd_t * pgd, unsigned long address)
{
	return (pud_t *)pgd;
}

#define pud_val(x)				(pgd_val((x).pgd))
#define __pud(x)				((pud_t) { __pgd(x) } )

#define pgd_page(pgd)				(pud_page((pud_t){ pgd }))
#define pgd_page_vaddr(pgd)			(pud_page_vaddr((pud_t){ pgd }))

/*
 * allocating and freeing a pud is trivial: the 1-entry pud is
 * inside the pgd, so has no extra memory associated with it.
 */
#define pud_alloc_one(mm, address)		NULL
#define pud_free(mm, x)				do { } while (0)
#define __pud_free_tlb(tlb, x)			do { } while (0)

#undef  pud_addr_end
#define pud_addr_end(addr, end)			(end)

#endif /* __ASSEMBLY__ */
#endif /* _PGTABLE_NOPUD_H */
