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
 * arch/xtensa/mm/tlb.c
 *
 * Logic that manipulates the Xtensa MMU.  Derived from MIPS.
 *
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 *
 * Copyright (C) 2001 - 2003 Tensilica Inc.
 *
 * Joe Taylor
 * Chris Zankel	<chris@zankel.net>
 * Marc Gauthier
 */

#include <linux/mm.h>
#include <asm/processor.h>
#include <asm/mmu_context.h>
#include <asm/tlbflush.h>
#include <asm/system.h>
#include <asm/cacheflush.h>


static inline void __flush_itlb_all (void)
{
	int w, i;

	for (w = 0; w < ITLB_ARF_WAYS; w++) {
		for (i = 0; i < (1 << XCHAL_ITLB_ARF_ENTRIES_LOG2); i++) {
			int e = w + (i << PAGE_SHIFT);
			invalidate_itlb_entry_no_isync(e);
		}
	}
	asm volatile ("isync\n");
}

static inline void __flush_dtlb_all (void)
{
	int w, i;

	for (w = 0; w < DTLB_ARF_WAYS; w++) {
		for (i = 0; i < (1 << XCHAL_DTLB_ARF_ENTRIES_LOG2); i++) {
			int e = w + (i << PAGE_SHIFT);
			invalidate_dtlb_entry_no_isync(e);
		}
	}
	asm volatile ("isync\n");
}


void flush_tlb_all (void)
{
	__flush_itlb_all();
	__flush_dtlb_all();
}

/* If mm is current, we simply assign the current task a new ASID, thus,
 * invalidating all previous tlb entries. If mm is someone else's user mapping,
 * wie invalidate the context, thus, when that user mapping is swapped in,
 * a new context will be assigned to it.
 */

void flush_tlb_mm(struct mm_struct *mm)
{
	if (mm == current->active_mm) {
		int flags;
		local_save_flags(flags);
		__get_new_mmu_context(mm);
		__load_mmu_context(mm);
		local_irq_restore(flags);
	}
	else
		mm->context = 0;
}

#define _ITLB_ENTRIES (ITLB_ARF_WAYS << XCHAL_ITLB_ARF_ENTRIES_LOG2)
#define _DTLB_ENTRIES (DTLB_ARF_WAYS << XCHAL_DTLB_ARF_ENTRIES_LOG2)
#if _ITLB_ENTRIES > _DTLB_ENTRIES
# define _TLB_ENTRIES _ITLB_ENTRIES
#else
# define _TLB_ENTRIES _DTLB_ENTRIES
#endif

void flush_tlb_range (struct vm_area_struct *vma,
    		      unsigned long start, unsigned long end)
{
	struct mm_struct *mm = vma->vm_mm;
	unsigned long flags;

	if (mm->context == NO_CONTEXT)
		return;

#if 0
	printk("[tlbrange<%02lx,%08lx,%08lx>]\n",
			(unsigned long)mm->context, start, end);
#endif
	local_save_flags(flags);

	if (end-start + (PAGE_SIZE-1) <= _TLB_ENTRIES << PAGE_SHIFT) {
		int oldpid = get_rasid_register();
		set_rasid_register (ASID_INSERT(mm->context));
		start &= PAGE_MASK;
 		if (vma->vm_flags & VM_EXEC)
			while(start < end) {
				invalidate_itlb_mapping(start);
				invalidate_dtlb_mapping(start);
				start += PAGE_SIZE;
			}
		else
			while(start < end) {
				invalidate_dtlb_mapping(start);
				start += PAGE_SIZE;
			}

		set_rasid_register(oldpid);
	} else {
		flush_tlb_mm(mm);
	}
	local_irq_restore(flags);
}

void flush_tlb_page (struct vm_area_struct *vma, unsigned long page)
{
	struct mm_struct* mm = vma->vm_mm;
	unsigned long flags;
	int oldpid;

	if(mm->context == NO_CONTEXT)
		return;

	local_save_flags(flags);

       	oldpid = get_rasid_register();

	if (vma->vm_flags & VM_EXEC)
		invalidate_itlb_mapping(page);
	invalidate_dtlb_mapping(page);

	set_rasid_register(oldpid);

	local_irq_restore(flags);
}

