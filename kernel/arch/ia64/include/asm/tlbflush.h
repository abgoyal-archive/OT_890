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

#ifndef _ASM_IA64_TLBFLUSH_H
#define _ASM_IA64_TLBFLUSH_H

/*
 * Copyright (C) 2002 Hewlett-Packard Co
 *	David Mosberger-Tang <davidm@hpl.hp.com>
 */


#include <linux/mm.h>

#include <asm/intrinsics.h>
#include <asm/mmu_context.h>
#include <asm/page.h>

/*
 * Now for some TLB flushing routines.  This is the kind of stuff that
 * can be very expensive, so try to avoid them whenever possible.
 */
extern void setup_ptcg_sem(int max_purges, int from_palo);

/*
 * Flush everything (kernel mapping may also have changed due to
 * vmalloc/vfree).
 */
extern void local_flush_tlb_all (void);

#ifdef CONFIG_SMP
  extern void smp_flush_tlb_all (void);
  extern void smp_flush_tlb_mm (struct mm_struct *mm);
  extern void smp_flush_tlb_cpumask (cpumask_t xcpumask);
# define flush_tlb_all()	smp_flush_tlb_all()
#else
# define flush_tlb_all()	local_flush_tlb_all()
# define smp_flush_tlb_cpumask(m) local_flush_tlb_all()
#endif

static inline void
local_finish_flush_tlb_mm (struct mm_struct *mm)
{
	if (mm == current->active_mm)
		activate_context(mm);
}

/*
 * Flush a specified user mapping.  This is called, e.g., as a result of fork() and
 * exit().  fork() ends up here because the copy-on-write mechanism needs to write-protect
 * the PTEs of the parent task.
 */
static inline void
flush_tlb_mm (struct mm_struct *mm)
{
	if (!mm)
		return;

	set_bit(mm->context, ia64_ctx.flushmap);
	mm->context = 0;

	if (atomic_read(&mm->mm_users) == 0)
		return;		/* happens as a result of exit_mmap() */

#ifdef CONFIG_SMP
	smp_flush_tlb_mm(mm);
#else
	local_finish_flush_tlb_mm(mm);
#endif
}

extern void flush_tlb_range (struct vm_area_struct *vma, unsigned long start, unsigned long end);

/*
 * Page-granular tlb flush.
 */
static inline void
flush_tlb_page (struct vm_area_struct *vma, unsigned long addr)
{
#ifdef CONFIG_SMP
	flush_tlb_range(vma, (addr & PAGE_MASK), (addr & PAGE_MASK) + PAGE_SIZE);
#else
	if (vma->vm_mm == current->active_mm)
		ia64_ptcl(addr, (PAGE_SHIFT << 2));
	else
		vma->vm_mm->context = 0;
#endif
}

/*
 * Flush the local TLB. Invoked from another cpu using an IPI.
 */
#ifdef CONFIG_SMP
void smp_local_flush_tlb(void);
#else
#define smp_local_flush_tlb()
#endif

static inline void flush_tlb_kernel_range(unsigned long start,
					  unsigned long end)
{
	flush_tlb_all();	/* XXX fix me */
}

#endif /* _ASM_IA64_TLBFLUSH_H */
