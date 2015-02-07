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

#ifndef __UM_TLB_H
#define __UM_TLB_H

#include <linux/pagemap.h>
#include <linux/swap.h>
#include <asm/percpu.h>
#include <asm/pgalloc.h>
#include <asm/tlbflush.h>

#define tlb_start_vma(tlb, vma) do { } while (0)
#define tlb_end_vma(tlb, vma) do { } while (0)
#define tlb_flush(tlb) flush_tlb_mm((tlb)->mm)

/* struct mmu_gather is an opaque type used by the mm code for passing around
 * any data needed by arch specific code for tlb_remove_page.
 */
struct mmu_gather {
	struct mm_struct	*mm;
	unsigned int		need_flush; /* Really unmapped some ptes? */
	unsigned long		start;
	unsigned long		end;
	unsigned int		fullmm; /* non-zero means full mm flush */
};

/* Users of the generic TLB shootdown code must declare this storage space. */
DECLARE_PER_CPU(struct mmu_gather, mmu_gathers);

static inline void __tlb_remove_tlb_entry(struct mmu_gather *tlb, pte_t *ptep,
					  unsigned long address)
{
	if (tlb->start > address)
		tlb->start = address;
	if (tlb->end < address + PAGE_SIZE)
		tlb->end = address + PAGE_SIZE;
}

static inline void init_tlb_gather(struct mmu_gather *tlb)
{
	tlb->need_flush = 0;

	tlb->start = TASK_SIZE;
	tlb->end = 0;

	if (tlb->fullmm) {
		tlb->start = 0;
		tlb->end = TASK_SIZE;
	}
}

/* tlb_gather_mmu
 *	Return a pointer to an initialized struct mmu_gather.
 */
static inline struct mmu_gather *
tlb_gather_mmu(struct mm_struct *mm, unsigned int full_mm_flush)
{
	struct mmu_gather *tlb = &get_cpu_var(mmu_gathers);

	tlb->mm = mm;
	tlb->fullmm = full_mm_flush;

	init_tlb_gather(tlb);

	return tlb;
}

extern void flush_tlb_mm_range(struct mm_struct *mm, unsigned long start,
			       unsigned long end);

static inline void
tlb_flush_mmu(struct mmu_gather *tlb, unsigned long start, unsigned long end)
{
	if (!tlb->need_flush)
		return;

	flush_tlb_mm_range(tlb->mm, tlb->start, tlb->end);
	init_tlb_gather(tlb);
}

/* tlb_finish_mmu
 *	Called at the end of the shootdown operation to free up any resources
 *	that were required.
 */
static inline void
tlb_finish_mmu(struct mmu_gather *tlb, unsigned long start, unsigned long end)
{
	tlb_flush_mmu(tlb, start, end);

	/* keep the page table cache within bounds */
	check_pgt_cache();

	put_cpu_var(mmu_gathers);
}

/* tlb_remove_page
 *	Must perform the equivalent to __free_pte(pte_get_and_clear(ptep)),
 *	while handling the additional races in SMP caused by other CPUs
 *	caching valid mappings in their TLBs.
 */
static inline void tlb_remove_page(struct mmu_gather *tlb, struct page *page)
{
	tlb->need_flush = 1;
	free_page_and_swap_cache(page);
	return;
}

/**
 * tlb_remove_tlb_entry - remember a pte unmapping for later tlb invalidation.
 *
 * Record the fact that pte's were really umapped in ->need_flush, so we can
 * later optimise away the tlb invalidate.   This helps when userspace is
 * unmapping already-unmapped pages, which happens quite a lot.
 */
#define tlb_remove_tlb_entry(tlb, ptep, address)		\
	do {							\
		tlb->need_flush = 1;				\
		__tlb_remove_tlb_entry(tlb, ptep, address);	\
	} while (0)

#define pte_free_tlb(tlb, ptep) __pte_free_tlb(tlb, ptep)

#define pud_free_tlb(tlb, pudp) __pud_free_tlb(tlb, pudp)

#define pmd_free_tlb(tlb, pmdp) __pmd_free_tlb(tlb, pmdp)

#define tlb_migrate_finish(mm) do {} while (0)

#endif
