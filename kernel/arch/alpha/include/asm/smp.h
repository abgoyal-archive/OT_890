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

#ifndef __ASM_SMP_H
#define __ASM_SMP_H

#include <linux/threads.h>
#include <linux/cpumask.h>
#include <linux/bitops.h>
#include <asm/pal.h>

/* HACK: Cabrio WHAMI return value is bogus if more than 8 bits used.. :-( */

static __inline__ unsigned char
__hard_smp_processor_id(void)
{
	register unsigned char __r0 __asm__("$0");
	__asm__ __volatile__(
		"call_pal %1 #whami"
		: "=r"(__r0)
		:"i" (PAL_whami)
		: "$1", "$22", "$23", "$24", "$25");
	return __r0;
}

#ifdef CONFIG_SMP

#include <asm/irq.h>

struct cpuinfo_alpha {
	unsigned long loops_per_jiffy;
	unsigned long last_asn;
	int need_new_asn;
	int asn_lock;
	unsigned long ipi_count;
	unsigned long prof_multiplier;
	unsigned long prof_counter;
	unsigned char mcheck_expected;
	unsigned char mcheck_taken;
	unsigned char mcheck_extra;
} __attribute__((aligned(64)));

extern struct cpuinfo_alpha cpu_data[NR_CPUS];

#define PROC_CHANGE_PENALTY     20

#define hard_smp_processor_id()	__hard_smp_processor_id()
#define raw_smp_processor_id()	(current_thread_info()->cpu)

extern int smp_num_cpus;

extern void arch_send_call_function_single_ipi(int cpu);
extern void arch_send_call_function_ipi(cpumask_t mask);

#else /* CONFIG_SMP */

#define hard_smp_processor_id()		0
#define smp_call_function_on_cpu(func,info,wait,cpu)    ({ 0; })

#endif /* CONFIG_SMP */

#define NO_PROC_ID	(-1)

#endif
