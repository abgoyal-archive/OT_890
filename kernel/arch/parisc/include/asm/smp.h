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


#if defined(CONFIG_SMP)

/* Page Zero Location PDC will look for the address to branch to when we poke
** slave CPUs still in "Icache loop".
*/
#define PDC_OS_BOOT_RENDEZVOUS     0x10
#define PDC_OS_BOOT_RENDEZVOUS_HI  0x28

#ifndef ASSEMBLY
#include <linux/bitops.h>
#include <linux/threads.h>	/* for NR_CPUS */
#include <linux/cpumask.h>
typedef unsigned long address_t;


/*
 *	Private routines/data
 *
 *	physical and logical are equivalent until we support CPU hotplug.
 */
#define cpu_number_map(cpu)	(cpu)
#define cpu_logical_map(cpu)	(cpu)

extern void smp_send_reschedule(int cpu);
extern void smp_send_all_nop(void);

extern void arch_send_call_function_single_ipi(int cpu);
extern void arch_send_call_function_ipi(cpumask_t mask);

#endif /* !ASSEMBLY */

/*
 *	This magic constant controls our willingness to transfer
 *      a process across CPUs. Such a transfer incurs cache and tlb
 *      misses. The current value is inherited from i386. Still needs
 *      to be tuned for parisc.
 */
 
#define PROC_CHANGE_PENALTY	15		/* Schedule penalty */

#define raw_smp_processor_id()	(current_thread_info()->cpu)

#else /* CONFIG_SMP */

static inline void smp_send_all_nop(void) { return; }

#endif

#define NO_PROC_ID		0xFF		/* No processor magic marker */
#define ANY_PROC_ID		0xFF		/* Any processor magic marker */
static inline int __cpu_disable (void) {
  return 0;
}
static inline void __cpu_die (unsigned int cpu) {
  while(1)
    ;
}
extern int __cpu_up (unsigned int cpu);

#endif /*  __ASM_SMP_H */
