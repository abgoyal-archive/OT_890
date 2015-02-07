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

#ifndef __ASM_SH_PROCESSOR_H
#define __ASM_SH_PROCESSOR_H

#include <asm/cpu-features.h>
#include <asm/segment.h>
#include <asm/cache.h>

#ifndef __ASSEMBLY__
/*
 *  CPU type and hardware bug flags. Kept separately for each CPU.
 *
 *  Each one of these also needs a CONFIG_CPU_SUBTYPE_xxx entry
 *  in arch/sh/mm/Kconfig, as well as an entry in arch/sh/kernel/setup.c
 *  for parsing the subtype in get_cpu_subtype().
 */
enum cpu_type {
	/* SH-2 types */
	CPU_SH7619,

	/* SH-2A types */
	CPU_SH7201, CPU_SH7203, CPU_SH7206, CPU_SH7263, CPU_MXG,

	/* SH-3 types */
	CPU_SH7705, CPU_SH7706, CPU_SH7707,
	CPU_SH7708, CPU_SH7708S, CPU_SH7708R,
	CPU_SH7709, CPU_SH7709A, CPU_SH7710, CPU_SH7712,
	CPU_SH7720, CPU_SH7721, CPU_SH7729,

	/* SH-4 types */
	CPU_SH7750, CPU_SH7750S, CPU_SH7750R, CPU_SH7751, CPU_SH7751R,
	CPU_SH7760, CPU_SH4_202, CPU_SH4_501,

	/* SH-4A types */
	CPU_SH7763, CPU_SH7770, CPU_SH7780, CPU_SH7781, CPU_SH7785,
	CPU_SH7723, CPU_SHX3,

	/* SH4AL-DSP types */
	CPU_SH7343, CPU_SH7722, CPU_SH7366,

	/* SH-5 types */
        CPU_SH5_101, CPU_SH5_103,

	/* Unknown subtype */
	CPU_SH_NONE
};

/*
 * TLB information structure
 *
 * Defined for both I and D tlb, per-processor.
 */
struct tlb_info {
	unsigned long long next;
	unsigned long long first;
	unsigned long long last;

	unsigned int entries;
	unsigned int step;

	unsigned long flags;
};

struct sh_cpuinfo {
	unsigned int type;
	int cut_major, cut_minor;
	unsigned long loops_per_jiffy;
	unsigned long asid_cache;

	struct cache_info icache;	/* Primary I-cache */
	struct cache_info dcache;	/* Primary D-cache */
	struct cache_info scache;	/* Secondary cache */

	/* TLB info */
	struct tlb_info itlb;
	struct tlb_info dtlb;

	unsigned long flags;
} __attribute__ ((aligned(L1_CACHE_BYTES)));

extern struct sh_cpuinfo cpu_data[];
#define boot_cpu_data cpu_data[0]
#define current_cpu_data cpu_data[smp_processor_id()]
#define raw_current_cpu_data cpu_data[raw_smp_processor_id()]

#define cpu_sleep()	__asm__ __volatile__ ("sleep" : : : "memory")
#define cpu_relax()	barrier()

/* Forward decl */
struct seq_operations;

extern struct pt_regs fake_swapper_regs;

/* arch/sh/kernel/setup.c */
const char *get_cpu_subtype(struct sh_cpuinfo *c);
extern const struct seq_operations cpuinfo_op;

#ifdef CONFIG_VSYSCALL
int vsyscall_init(void);
#else
#define vsyscall_init() do { } while (0)
#endif

#endif /* __ASSEMBLY__ */

#ifdef CONFIG_SUPERH32
# include "processor_32.h"
#else
# include "processor_64.h"
#endif

#endif /* __ASM_SH_PROCESSOR_H */
