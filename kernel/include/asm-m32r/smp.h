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

#ifndef _ASM_M32R_SMP_H
#define _ASM_M32R_SMP_H

#ifdef CONFIG_SMP
#ifndef __ASSEMBLY__

#include <linux/cpumask.h>
#include <linux/spinlock.h>
#include <linux/threads.h>
#include <asm/m32r.h>

#define PHYSID_ARRAY_SIZE       1

struct physid_mask
{
	unsigned long mask[PHYSID_ARRAY_SIZE];
};

typedef struct physid_mask physid_mask_t;

#define physid_set(physid, map)                 set_bit(physid, (map).mask)
#define physid_clear(physid, map)               clear_bit(physid, (map).mask)
#define physid_isset(physid, map)               test_bit(physid, (map).mask)
#define physid_test_and_set(physid, map)        test_and_set_bit(physid, (map).mask)

#define physids_and(dst, src1, src2)            bitmap_and((dst).mask, (src1).mask, (src2).mask, MAX_APICS)
#define physids_or(dst, src1, src2)             bitmap_or((dst).mask, (src1).mask, (src2).mask, MAX_APICS)
#define physids_clear(map)                      bitmap_zero((map).mask, MAX_APICS)
#define physids_complement(dst, src)            bitmap_complement((dst).mask,(src).mask, MAX_APICS)
#define physids_empty(map)                      bitmap_empty((map).mask, MAX_APICS)
#define physids_equal(map1, map2)               bitmap_equal((map1).mask, (map2).mask, MAX_APICS)
#define physids_weight(map)                     bitmap_weight((map).mask, MAX_APICS)
#define physids_shift_right(d, s, n)            bitmap_shift_right((d).mask, (s).mask, n, MAX_APICS)
#define physids_shift_left(d, s, n)             bitmap_shift_left((d).mask, (s).mask, n, MAX_APICS)
#define physids_coerce(map)                     ((map).mask[0])

#define physids_promote(physids)					\
	({								\
		physid_mask_t __physid_mask = PHYSID_MASK_NONE;		\
		__physid_mask.mask[0] = physids;			\
		__physid_mask;						\
	})

#define physid_mask_of_physid(physid)					\
	({								\
		physid_mask_t __physid_mask = PHYSID_MASK_NONE;		\
		physid_set(physid, __physid_mask);			\
		__physid_mask;						\
	})

#define PHYSID_MASK_ALL         { {[0 ... PHYSID_ARRAY_SIZE-1] = ~0UL} }
#define PHYSID_MASK_NONE        { {[0 ... PHYSID_ARRAY_SIZE-1] = 0UL} }

extern physid_mask_t phys_cpu_present_map;

/*
 * Some lowlevel functions might want to know about
 * the real CPU ID <-> CPU # mapping.
 */
extern volatile int cpu_2_physid[NR_CPUS];
#define cpu_to_physid(cpu_id)	cpu_2_physid[cpu_id]

#define raw_smp_processor_id()	(current_thread_info()->cpu)

extern cpumask_t cpu_callout_map;

static __inline__ int hard_smp_processor_id(void)
{
	return (int)*(volatile long *)M32R_CPUID_PORTL;
}

static __inline__ int cpu_logical_map(int cpu)
{
	return cpu;
}

static __inline__ int cpu_number_map(int cpu)
{
	return cpu;
}

static __inline__ unsigned int num_booting_cpus(void)
{
	return cpus_weight(cpu_callout_map);
}

extern void smp_send_timer(void);
extern unsigned long send_IPI_mask_phys(cpumask_t, int, int);

extern void arch_send_call_function_single_ipi(int cpu);
extern void arch_send_call_function_ipi(cpumask_t mask);

#endif	/* not __ASSEMBLY__ */

#define NO_PROC_ID (0xff)	/* No processor magic marker */

#define PROC_CHANGE_PENALTY	(15)	/* Schedule penalty */

/*
 * M32R-mp IPI
 */
#define RESCHEDULE_IPI		(M32R_IRQ_IPI0-M32R_IRQ_IPI0)
#define INVALIDATE_TLB_IPI	(M32R_IRQ_IPI1-M32R_IRQ_IPI0)
#define CALL_FUNCTION_IPI	(M32R_IRQ_IPI2-M32R_IRQ_IPI0)
#define LOCAL_TIMER_IPI		(M32R_IRQ_IPI3-M32R_IRQ_IPI0)
#define INVALIDATE_CACHE_IPI	(M32R_IRQ_IPI4-M32R_IRQ_IPI0)
#define CPU_BOOT_IPI		(M32R_IRQ_IPI5-M32R_IRQ_IPI0)
#define CALL_FUNC_SINGLE_IPI	(M32R_IRQ_IPI6-M32R_IRQ_IPI0)

#define IPI_SHIFT	(0)
#define NR_IPIS		(8)

#else	/* CONFIG_SMP */

#define hard_smp_processor_id()		0

#endif /* CONFIG_SMP */

#endif	/* _ASM_M32R_SMP_H */
