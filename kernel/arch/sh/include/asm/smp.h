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

#ifndef __ASM_SH_SMP_H
#define __ASM_SH_SMP_H

#include <linux/bitops.h>
#include <linux/cpumask.h>

#ifdef CONFIG_SMP

#include <linux/spinlock.h>
#include <asm/atomic.h>
#include <asm/current.h>

#define raw_smp_processor_id()	(current_thread_info()->cpu)
#define hard_smp_processor_id()	plat_smp_processor_id()

/* Map from cpu id to sequential logical cpu number. */
extern int __cpu_number_map[NR_CPUS];
#define cpu_number_map(cpu)  __cpu_number_map[cpu]

/* The reverse map from sequential logical cpu number to cpu id.  */
extern int __cpu_logical_map[NR_CPUS];
#define cpu_logical_map(cpu)  __cpu_logical_map[cpu]

enum {
	SMP_MSG_FUNCTION,
	SMP_MSG_RESCHEDULE,
	SMP_MSG_FUNCTION_SINGLE,
	SMP_MSG_TIMER,

	SMP_MSG_NR,	/* must be last */
};

void smp_message_recv(unsigned int msg);
void smp_timer_broadcast(const struct cpumask *mask);

void local_timer_interrupt(void);
void local_timer_setup(unsigned int cpu);

void plat_smp_setup(void);
void plat_prepare_cpus(unsigned int max_cpus);
int plat_smp_processor_id(void);
void plat_start_cpu(unsigned int cpu, unsigned long entry_point);
void plat_send_ipi(unsigned int cpu, unsigned int message);

void arch_send_call_function_single_ipi(int cpu);
void arch_send_call_function_ipi(cpumask_t mask);

#else

#define hard_smp_processor_id()	(0)

#endif /* CONFIG_SMP */

#endif /* __ASM_SH_SMP_H */
