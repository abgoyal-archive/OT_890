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
 *	Copyright (C) 1992, 1998 Linus Torvalds, Ingo Molnar
 *
 * This file contains the lowest level x86_64-specific interrupt
 * entry and irq statistics code. All the remaining irq logic is
 * done by the generic kernel/irq/ code and in the
 * x86_64-specific irq controller code. (e.g. i8259.c and
 * io_apic.c.)
 */

#include <linux/kernel_stat.h>
#include <linux/interrupt.h>
#include <linux/seq_file.h>
#include <linux/module.h>
#include <linux/delay.h>
#include <linux/ftrace.h>
#include <linux/uaccess.h>
#include <linux/smp.h>
#include <asm/io_apic.h>
#include <asm/idle.h>

/*
 * Probabilistic stack overflow check:
 *
 * Only check the stack in process context, because everything else
 * runs on the big interrupt stacks. Checking reliably is too expensive,
 * so we just check from interrupts.
 */
static inline void stack_overflow_check(struct pt_regs *regs)
{
#ifdef CONFIG_DEBUG_STACKOVERFLOW
	u64 curbase = (u64)task_stack_page(current);

	WARN_ONCE(regs->sp >= curbase &&
		  regs->sp <= curbase + THREAD_SIZE &&
		  regs->sp <  curbase + sizeof(struct thread_info) +
					sizeof(struct pt_regs) + 128,

		  "do_IRQ: %s near stack overflow (cur:%Lx,sp:%lx)\n",
			current->comm, curbase, regs->sp);
#endif
}

/*
 * do_IRQ handles all normal device IRQ's (the special
 * SMP cross-CPU interrupts have their own specific
 * handlers).
 */
asmlinkage unsigned int __irq_entry do_IRQ(struct pt_regs *regs)
{
	struct pt_regs *old_regs = set_irq_regs(regs);
	struct irq_desc *desc;

	/* high bit used in ret_from_ code  */
	unsigned vector = ~regs->orig_ax;
	unsigned irq;

	exit_idle();
	irq_enter();
	irq = __get_cpu_var(vector_irq)[vector];

	stack_overflow_check(regs);

	desc = irq_to_desc(irq);
	if (likely(desc))
		generic_handle_irq_desc(irq, desc);
	else {
		if (!disable_apic)
			ack_APIC_irq();

		if (printk_ratelimit())
			printk(KERN_EMERG "%s: %d.%d No irq handler for vector\n",
				__func__, smp_processor_id(), vector);
	}

	irq_exit();

	set_irq_regs(old_regs);
	return 1;
}

#ifdef CONFIG_HOTPLUG_CPU
/* A cpu has been removed from cpu_online_mask.  Reset irq affinities. */
void fixup_irqs(void)
{
	unsigned int irq;
	static int warned;
	struct irq_desc *desc;

	for_each_irq_desc(irq, desc) {
		int break_affinity = 0;
		int set_affinity = 1;
		const struct cpumask *affinity;

		if (!desc)
			continue;
		if (irq == 2)
			continue;

		/* interrupt's are disabled at this point */
		spin_lock(&desc->lock);

		affinity = &desc->affinity;
		if (!irq_has_action(irq) ||
		    cpumask_equal(affinity, cpu_online_mask)) {
			spin_unlock(&desc->lock);
			continue;
		}

		if (cpumask_any_and(affinity, cpu_online_mask) >= nr_cpu_ids) {
			break_affinity = 1;
			affinity = cpu_all_mask;
		}

		if (desc->chip->mask)
			desc->chip->mask(irq);

		if (desc->chip->set_affinity)
			desc->chip->set_affinity(irq, affinity);
		else if (!(warned++))
			set_affinity = 0;

		if (desc->chip->unmask)
			desc->chip->unmask(irq);

		spin_unlock(&desc->lock);

		if (break_affinity && set_affinity)
			printk("Broke affinity for irq %i\n", irq);
		else if (!set_affinity)
			printk("Cannot set affinity for irq %i\n", irq);
	}

	/* That doesn't seem sufficient.  Give it 1ms. */
	local_irq_enable();
	mdelay(1);
	local_irq_disable();
}
#endif

extern void call_softirq(void);

asmlinkage void do_softirq(void)
{
	__u32 pending;
	unsigned long flags;

	if (in_interrupt())
		return;

	local_irq_save(flags);
	pending = local_softirq_pending();
	/* Switch to interrupt stack */
	if (pending) {
		call_softirq();
		WARN_ON_ONCE(softirq_count());
	}
	local_irq_restore(flags);
}
