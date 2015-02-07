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
 * arch/sh/mm/tlb-sh3.c
 *
 * SH-3 specific TLB operations
 *
 * Copyright (C) 1999  Niibe Yutaka
 * Copyright (C) 2002  Paul Mundt
 *
 * Released under the terms of the GNU GPL v2.0.
 */
#include <linux/signal.h>
#include <linux/sched.h>
#include <linux/kernel.h>
#include <linux/errno.h>
#include <linux/string.h>
#include <linux/types.h>
#include <linux/ptrace.h>
#include <linux/mman.h>
#include <linux/mm.h>
#include <linux/smp.h>
#include <linux/smp_lock.h>
#include <linux/interrupt.h>

#include <asm/system.h>
#include <asm/io.h>
#include <asm/uaccess.h>
#include <asm/pgalloc.h>
#include <asm/mmu_context.h>
#include <asm/cacheflush.h>

void update_mmu_cache(struct vm_area_struct * vma,
		      unsigned long address, pte_t pte)
{
	unsigned long flags;
	unsigned long pteval;
	unsigned long vpn;

	/* Ptrace may call this routine. */
	if (vma && current->active_mm != vma->vm_mm)
		return;

#if defined(CONFIG_SH7705_CACHE_32KB)
	{
		struct page *page = pte_page(pte);
		unsigned long pfn = pte_pfn(pte);

		if (pfn_valid(pfn) && !test_bit(PG_mapped, &page->flags)) {
			unsigned long phys = pte_val(pte) & PTE_PHYS_MASK;

			__flush_wback_region((void *)P1SEGADDR(phys),
					     PAGE_SIZE);
			__set_bit(PG_mapped, &page->flags);
		}
	}
#endif

	local_irq_save(flags);

	/* Set PTEH register */
	vpn = (address & MMU_VPN_MASK) | get_asid();
	ctrl_outl(vpn, MMU_PTEH);

	pteval = pte_val(pte);

	/* Set PTEL register */
	pteval &= _PAGE_FLAGS_HARDWARE_MASK; /* drop software flags */
	/* conveniently, we want all the software flags to be 0 anyway */
	ctrl_outl(pteval, MMU_PTEL);

	/* Load the TLB */
	asm volatile("ldtlb": /* no output */ : /* no input */ : "memory");
	local_irq_restore(flags);
}

void local_flush_tlb_one(unsigned long asid, unsigned long page)
{
	unsigned long addr, data;
	int i, ways = MMU_NTLB_WAYS;

	/*
	 * NOTE: PTEH.ASID should be set to this MM
	 *       _AND_ we need to write ASID to the array.
	 *
	 * It would be simple if we didn't need to set PTEH.ASID...
	 */
	addr = MMU_TLB_ADDRESS_ARRAY | (page & 0x1F000);
	data = (page & 0xfffe0000) | asid; /* VALID bit is off */

	if ((current_cpu_data.flags & CPU_HAS_MMU_PAGE_ASSOC)) {
		addr |= MMU_PAGE_ASSOC_BIT;
		ways = 1;	/* we already know the way .. */
	}

	for (i = 0; i < ways; i++)
		ctrl_outl(data, addr + (i << 8));
}

