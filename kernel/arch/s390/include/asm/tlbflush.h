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

#ifndef _S390_TLBFLUSH_H
#define _S390_TLBFLUSH_H

#include <linux/mm.h>
#include <linux/sched.h>
#include <asm/processor.h>
#include <asm/pgalloc.h>

/*
 * Flush all tlb entries on the local cpu.
 */
static inline void __tlb_flush_local(void)
{
	asm volatile("ptlb" : : : "memory");
}

#ifdef CONFIG_SMP
/*
 * Flush all tlb entries on all cpus.
 */
void smp_ptlb_all(void);

static inline void __tlb_flush_global(void)
{
	register unsigned long reg2 asm("2");
	register unsigned long reg3 asm("3");
	register unsigned long reg4 asm("4");
	long dummy;

#ifndef __s390x__
	if (!MACHINE_HAS_CSP) {
		smp_ptlb_all();
		return;
	}
#endif /* __s390x__ */

	dummy = 0;
	reg2 = reg3 = 0;
	reg4 = ((unsigned long) &dummy) + 1;
	asm volatile(
		"	csp	%0,%2"
		: : "d" (reg2), "d" (reg3), "d" (reg4), "m" (dummy) : "cc" );
}

static inline void __tlb_flush_full(struct mm_struct *mm)
{
	cpumask_t local_cpumask;

	preempt_disable();
	/*
	 * If the process only ran on the local cpu, do a local flush.
	 */
	local_cpumask = cpumask_of_cpu(smp_processor_id());
	if (cpus_equal(mm->cpu_vm_mask, local_cpumask))
		__tlb_flush_local();
	else
		__tlb_flush_global();
	preempt_enable();
}
#else
#define __tlb_flush_full(mm)	__tlb_flush_local()
#endif

/*
 * Flush all tlb entries of a page table on all cpus.
 */
static inline void __tlb_flush_idte(unsigned long asce)
{
	asm volatile(
		"	.insn	rrf,0xb98e0000,0,%0,%1,0"
		: : "a" (2048), "a" (asce) : "cc" );
}

static inline void __tlb_flush_mm(struct mm_struct * mm)
{
	if (unlikely(cpus_empty(mm->cpu_vm_mask)))
		return;
	/*
	 * If the machine has IDTE we prefer to do a per mm flush
	 * on all cpus instead of doing a local flush if the mm
	 * only ran on the local cpu.
	 */
	if (MACHINE_HAS_IDTE) {
		if (mm->context.noexec)
			__tlb_flush_idte((unsigned long)
					 get_shadow_table(mm->pgd) |
					 mm->context.asce_bits);
		__tlb_flush_idte((unsigned long) mm->pgd |
				 mm->context.asce_bits);
		return;
	}
	__tlb_flush_full(mm);
}

static inline void __tlb_flush_mm_cond(struct mm_struct * mm)
{
	if (atomic_read(&mm->mm_users) <= 1 && mm == current->active_mm)
		__tlb_flush_mm(mm);
}

/*
 * TLB flushing:
 *  flush_tlb() - flushes the current mm struct TLBs
 *  flush_tlb_all() - flushes all processes TLBs
 *  flush_tlb_mm(mm) - flushes the specified mm context TLB's
 *  flush_tlb_page(vma, vmaddr) - flushes one page
 *  flush_tlb_range(vma, start, end) - flushes a range of pages
 *  flush_tlb_kernel_range(start, end) - flushes a range of kernel pages
 */

/*
 * flush_tlb_mm goes together with ptep_set_wrprotect for the
 * copy_page_range operation and flush_tlb_range is related to
 * ptep_get_and_clear for change_protection. ptep_set_wrprotect and
 * ptep_get_and_clear do not flush the TLBs directly if the mm has
 * only one user. At the end of the update the flush_tlb_mm and
 * flush_tlb_range functions need to do the flush.
 */
#define flush_tlb()				do { } while (0)
#define flush_tlb_all()				do { } while (0)
#define flush_tlb_page(vma, addr)		do { } while (0)

static inline void flush_tlb_mm(struct mm_struct *mm)
{
	__tlb_flush_mm_cond(mm);
}

static inline void flush_tlb_range(struct vm_area_struct *vma,
				   unsigned long start, unsigned long end)
{
	__tlb_flush_mm_cond(vma->vm_mm);
}

static inline void flush_tlb_kernel_range(unsigned long start,
					  unsigned long end)
{
	__tlb_flush_mm(&init_mm);
}

#endif /* _S390_TLBFLUSH_H */
