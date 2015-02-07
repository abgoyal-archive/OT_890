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

/* arch/sparc64/mm/tlb.c
 *
 * Copyright (C) 2004 David S. Miller <davem@redhat.com>
 */

#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/percpu.h>
#include <linux/mm.h>
#include <linux/swap.h>
#include <linux/preempt.h>

#include <asm/pgtable.h>
#include <asm/pgalloc.h>
#include <asm/tlbflush.h>
#include <asm/cacheflush.h>
#include <asm/mmu_context.h>
#include <asm/tlb.h>

/* Heavily inspired by the ppc64 code.  */

DEFINE_PER_CPU(struct mmu_gather, mmu_gathers);

void flush_tlb_pending(void)
{
	struct mmu_gather *mp = &get_cpu_var(mmu_gathers);

	if (mp->tlb_nr) {
		flush_tsb_user(mp);

		if (CTX_VALID(mp->mm->context)) {
#ifdef CONFIG_SMP
			smp_flush_tlb_pending(mp->mm, mp->tlb_nr,
					      &mp->vaddrs[0]);
#else
			__flush_tlb_pending(CTX_HWBITS(mp->mm->context),
					    mp->tlb_nr, &mp->vaddrs[0]);
#endif
		}
		mp->tlb_nr = 0;
	}

	put_cpu_var(mmu_gathers);
}

void tlb_batch_add(struct mm_struct *mm, unsigned long vaddr, pte_t *ptep, pte_t orig)
{
	struct mmu_gather *mp = &__get_cpu_var(mmu_gathers);
	unsigned long nr;

	vaddr &= PAGE_MASK;
	if (pte_exec(orig))
		vaddr |= 0x1UL;

	if (tlb_type != hypervisor &&
	    pte_dirty(orig)) {
		unsigned long paddr, pfn = pte_pfn(orig);
		struct address_space *mapping;
		struct page *page;

		if (!pfn_valid(pfn))
			goto no_cache_flush;

		page = pfn_to_page(pfn);
		if (PageReserved(page))
			goto no_cache_flush;

		/* A real file page? */
		mapping = page_mapping(page);
		if (!mapping)
			goto no_cache_flush;

		paddr = (unsigned long) page_address(page);
		if ((paddr ^ vaddr) & (1 << 13))
			flush_dcache_page_all(mm, page);
	}

no_cache_flush:

	if (mp->fullmm)
		return;

	nr = mp->tlb_nr;

	if (unlikely(nr != 0 && mm != mp->mm)) {
		flush_tlb_pending();
		nr = 0;
	}

	if (nr == 0)
		mp->mm = mm;

	mp->vaddrs[nr] = vaddr;
	mp->tlb_nr = ++nr;
	if (nr >= TLB_BATCH_NR)
		flush_tlb_pending();
}
