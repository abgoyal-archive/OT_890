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

#ifndef _ASM_M32R_MMU_CONTEXT_H
#define _ASM_M32R_MMU_CONTEXT_H
#ifdef __KERNEL__

#include <asm/m32r.h>

#define MMU_CONTEXT_ASID_MASK      (0x000000FF)
#define MMU_CONTEXT_VERSION_MASK   (0xFFFFFF00)
#define MMU_CONTEXT_FIRST_VERSION  (0x00000100)
#define NO_CONTEXT                 (0x00000000)

#ifndef __ASSEMBLY__

#include <asm/atomic.h>
#include <asm/pgalloc.h>
#include <asm/mmu.h>
#include <asm/tlbflush.h>
#include <asm-generic/mm_hooks.h>

/*
 * Cache of MMU context last used.
 */
#ifndef CONFIG_SMP
extern unsigned long mmu_context_cache_dat;
#define mmu_context_cache	mmu_context_cache_dat
#define mm_context(mm)		mm->context
#else /* not CONFIG_SMP */
extern unsigned long mmu_context_cache_dat[];
#define mmu_context_cache	mmu_context_cache_dat[smp_processor_id()]
#define mm_context(mm)		mm->context[smp_processor_id()]
#endif /* not CONFIG_SMP */

#define set_tlb_tag(entry, tag)		(*entry = (tag & PAGE_MASK)|get_asid())
#define set_tlb_data(entry, data)	(*entry = (data | _PAGE_PRESENT))

#ifdef CONFIG_MMU
#define enter_lazy_tlb(mm, tsk)	do { } while (0)

static inline void get_new_mmu_context(struct mm_struct *mm)
{
	unsigned long mc = ++mmu_context_cache;

	if (!(mc & MMU_CONTEXT_ASID_MASK)) {
		/* We exhaust ASID of this version.
		   Flush all TLB and start new cycle. */
		local_flush_tlb_all();
		/* Fix version if needed.
		   Note that we avoid version #0 to distingush NO_CONTEXT. */
		if (!mc)
			mmu_context_cache = mc = MMU_CONTEXT_FIRST_VERSION;
	}
	mm_context(mm) = mc;
}

/*
 * Get MMU context if needed.
 */
static inline void get_mmu_context(struct mm_struct *mm)
{
	if (mm) {
		unsigned long mc = mmu_context_cache;

		/* Check if we have old version of context.
		   If it's old, we need to get new context with new version. */
		if ((mm_context(mm) ^ mc) & MMU_CONTEXT_VERSION_MASK)
			get_new_mmu_context(mm);
	}
}

/*
 * Initialize the context related info for a new mm_struct
 * instance.
 */
static inline int init_new_context(struct task_struct *tsk,
	struct mm_struct *mm)
{
#ifndef CONFIG_SMP
	mm->context = NO_CONTEXT;
#else /* CONFIG_SMP */
	int num_cpus = num_online_cpus();
	int i;

	for (i = 0 ; i < num_cpus ; i++)
		mm->context[i] = NO_CONTEXT;
#endif /* CONFIG_SMP */

	return 0;
}

/*
 * Destroy context related info for an mm_struct that is about
 * to be put to rest.
 */
#define destroy_context(mm)	do { } while (0)

static inline void set_asid(unsigned long asid)
{
	*(volatile unsigned long *)MASID = (asid & MMU_CONTEXT_ASID_MASK);
}

static inline unsigned long get_asid(void)
{
	unsigned long asid;

	asid = *(volatile long *)MASID;
	asid &= MMU_CONTEXT_ASID_MASK;

	return asid;
}

/*
 * After we have set current->mm to a new value, this activates
 * the context for the new mm so we see the new mappings.
 */
static inline void activate_context(struct mm_struct *mm)
{
	get_mmu_context(mm);
	set_asid(mm_context(mm) & MMU_CONTEXT_ASID_MASK);
}

static inline void switch_mm(struct mm_struct *prev,
	struct mm_struct *next, struct task_struct *tsk)
{
#ifdef CONFIG_SMP
	int cpu = smp_processor_id();
#endif	/* CONFIG_SMP */

	if (prev != next) {
#ifdef CONFIG_SMP
		cpu_set(cpu, next->cpu_vm_mask);
#endif /* CONFIG_SMP */
		/* Set MPTB = next->pgd */
		*(volatile unsigned long *)MPTB = (unsigned long)next->pgd;
		activate_context(next);
	}
#ifdef CONFIG_SMP
	else
		if (!cpu_test_and_set(cpu, next->cpu_vm_mask))
			activate_context(next);
#endif /* CONFIG_SMP */
}

#define deactivate_mm(tsk, mm)	do { } while (0)

#define activate_mm(prev, next)	\
	switch_mm((prev), (next), NULL)

#else /* not CONFIG_MMU */
#define get_mmu_context(mm)             do { } while (0)
#define init_new_context(tsk,mm)        (0)
#define destroy_context(mm)             do { } while (0)
#define set_asid(asid)                  do { } while (0)
#define get_asid()                      (0)
#define activate_context(mm)            do { } while (0)
#define switch_mm(prev,next,tsk)        do { } while (0)
#define deactivate_mm(mm,tsk)           do { } while (0)
#define activate_mm(prev,next)          do { } while (0)
#define enter_lazy_tlb(mm,tsk)          do { } while (0)
#endif /* not CONFIG_MMU */

#endif /* not __ASSEMBLY__ */

#endif /* __KERNEL__ */
#endif /* _ASM_M32R_MMU_CONTEXT_H */
