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
 * arch/sh/mm/pg-sh4.c
 *
 * Copyright (C) 1999, 2000, 2002  Niibe Yutaka
 * Copyright (C) 2002 - 2007  Paul Mundt
 *
 * Released under the terms of the GNU GPL v2.0.
 */
#include <linux/mm.h>
#include <linux/init.h>
#include <linux/mutex.h>
#include <linux/fs.h>
#include <linux/highmem.h>
#include <linux/module.h>
#include <asm/mmu_context.h>
#include <asm/cacheflush.h>

#define CACHE_ALIAS (current_cpu_data.dcache.alias_mask)

#define kmap_get_fixmap_pte(vaddr)                                     \
	pte_offset_kernel(pmd_offset(pud_offset(pgd_offset_k(vaddr), (vaddr)), (vaddr)), (vaddr))

static pte_t *kmap_coherent_pte;

void __init kmap_coherent_init(void)
{
	unsigned long vaddr;

	/* cache the first coherent kmap pte */
	vaddr = __fix_to_virt(FIX_CMAP_BEGIN);
	kmap_coherent_pte = kmap_get_fixmap_pte(vaddr);
}

static inline void *kmap_coherent(struct page *page, unsigned long addr)
{
	enum fixed_addresses idx;
	unsigned long vaddr, flags;
	pte_t pte;

	inc_preempt_count();

	idx = (addr & current_cpu_data.dcache.alias_mask) >> PAGE_SHIFT;
	vaddr = __fix_to_virt(FIX_CMAP_END - idx);
	pte = mk_pte(page, PAGE_KERNEL);

	local_irq_save(flags);
	flush_tlb_one(get_asid(), vaddr);
	local_irq_restore(flags);

	update_mmu_cache(NULL, vaddr, pte);

	set_pte(kmap_coherent_pte - (FIX_CMAP_END - idx), pte);

	return (void *)vaddr;
}

static inline void kunmap_coherent(struct page *page)
{
	dec_preempt_count();
	preempt_check_resched();
}

/*
 * clear_user_page
 * @to: P1 address
 * @address: U0 address to be mapped
 * @page: page (virt_to_page(to))
 */
void clear_user_page(void *to, unsigned long address, struct page *page)
{
	__set_bit(PG_mapped, &page->flags);

	clear_page(to);
	if ((((address & PAGE_MASK) ^ (unsigned long)to) & CACHE_ALIAS))
		__flush_wback_region(to, PAGE_SIZE);
}

void copy_to_user_page(struct vm_area_struct *vma, struct page *page,
		       unsigned long vaddr, void *dst, const void *src,
		       unsigned long len)
{
	void *vto;

	__set_bit(PG_mapped, &page->flags);

	vto = kmap_coherent(page, vaddr) + (vaddr & ~PAGE_MASK);
	memcpy(vto, src, len);
	kunmap_coherent(vto);

	if (vma->vm_flags & VM_EXEC)
		flush_cache_page(vma, vaddr, page_to_pfn(page));
}

void copy_from_user_page(struct vm_area_struct *vma, struct page *page,
			 unsigned long vaddr, void *dst, const void *src,
			 unsigned long len)
{
	void *vfrom;

	__set_bit(PG_mapped, &page->flags);

	vfrom = kmap_coherent(page, vaddr) + (vaddr & ~PAGE_MASK);
	memcpy(dst, vfrom, len);
	kunmap_coherent(vfrom);
}

void copy_user_highpage(struct page *to, struct page *from,
			unsigned long vaddr, struct vm_area_struct *vma)
{
	void *vfrom, *vto;

	__set_bit(PG_mapped, &to->flags);

	vto = kmap_atomic(to, KM_USER1);
	vfrom = kmap_coherent(from, vaddr);
	copy_page(vto, vfrom);
	kunmap_coherent(vfrom);

	if (((vaddr ^ (unsigned long)vto) & CACHE_ALIAS))
		__flush_wback_region(vto, PAGE_SIZE);

	kunmap_atomic(vto, KM_USER1);
	/* Make sure this page is cleared on other CPU's too before using it */
	smp_wmb();
}
EXPORT_SYMBOL(copy_user_highpage);

/*
 * For SH-4, we have our own implementation for ptep_get_and_clear
 */
pte_t ptep_get_and_clear(struct mm_struct *mm, unsigned long addr, pte_t *ptep)
{
	pte_t pte = *ptep;

	pte_clear(mm, addr, ptep);
	if (!pte_not_present(pte)) {
		unsigned long pfn = pte_pfn(pte);
		if (pfn_valid(pfn)) {
			struct page *page = pfn_to_page(pfn);
			struct address_space *mapping = page_mapping(page);
			if (!mapping || !mapping_writably_mapped(mapping))
				__clear_bit(PG_mapped, &page->flags);
		}
	}
	return pte;
}
