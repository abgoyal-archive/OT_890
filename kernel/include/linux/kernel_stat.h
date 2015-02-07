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

#ifndef _LINUX_KERNEL_STAT_H
#define _LINUX_KERNEL_STAT_H

#include <linux/smp.h>
#include <linux/threads.h>
#include <linux/percpu.h>
#include <linux/cpumask.h>
#include <asm/irq.h>
#include <asm/cputime.h>

/*
 * 'kernel_stat.h' contains the definitions needed for doing
 * some kernel statistics (CPU usage, context switches ...),
 * used by rstatd/perfmeter
 */

struct cpu_usage_stat {
	cputime64_t user;
	cputime64_t nice;
	cputime64_t system;
	cputime64_t softirq;
	cputime64_t irq;
	cputime64_t idle;
	cputime64_t iowait;
	cputime64_t steal;
	cputime64_t guest;
};

struct kernel_stat {
	struct cpu_usage_stat	cpustat;
#ifndef CONFIG_SPARSE_IRQ
       unsigned int irqs[NR_IRQS];
#endif
};

DECLARE_PER_CPU(struct kernel_stat, kstat);

#define kstat_cpu(cpu)	per_cpu(kstat, cpu)
/* Must have preemption disabled for this to be meaningful. */
#define kstat_this_cpu	__get_cpu_var(kstat)

extern unsigned long long nr_context_switches(void);

#ifndef CONFIG_SPARSE_IRQ
#define kstat_irqs_this_cpu(irq) \
	(kstat_this_cpu.irqs[irq])

struct irq_desc;

static inline void kstat_incr_irqs_this_cpu(unsigned int irq,
					    struct irq_desc *desc)
{
	kstat_this_cpu.irqs[irq]++;
}
#endif


#ifndef CONFIG_SPARSE_IRQ
static inline unsigned int kstat_irqs_cpu(unsigned int irq, int cpu)
{
       return kstat_cpu(cpu).irqs[irq];
}
#else
extern unsigned int kstat_irqs_cpu(unsigned int irq, int cpu);
#endif

/*
 * Number of interrupts per specific IRQ source, since bootup
 */
static inline unsigned int kstat_irqs(unsigned int irq)
{
	unsigned int sum = 0;
	int cpu;

	for_each_possible_cpu(cpu)
		sum += kstat_irqs_cpu(irq, cpu);

	return sum;
}

extern unsigned long long task_delta_exec(struct task_struct *);
extern void account_user_time(struct task_struct *, cputime_t, cputime_t);
extern void account_system_time(struct task_struct *, int, cputime_t, cputime_t);
extern void account_steal_time(cputime_t);
extern void account_idle_time(cputime_t);

extern void account_process_tick(struct task_struct *, int user);
extern void account_steal_ticks(unsigned long ticks);
extern void account_idle_ticks(unsigned long ticks);

#endif /* _LINUX_KERNEL_STAT_H */
