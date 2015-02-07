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
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 *
 * Copyright (C) 2007 MIPS Technologies, Inc.
 * Copyright (C) 2007 Ralf Baechle <ralf@linux-mips.org>
 */
#include <linux/clockchips.h>
#include <linux/interrupt.h>
#include <linux/percpu.h>

#include <asm/smtc_ipi.h>
#include <asm/time.h>
#include <asm/cevt-r4k.h>

/*
 * The SMTC Kernel for the 34K, 1004K, et. al. replaces several
 * of these routines with SMTC-specific variants.
 */

#ifndef CONFIG_MIPS_MT_SMTC

static int mips_next_event(unsigned long delta,
                           struct clock_event_device *evt)
{
	unsigned int cnt;
	int res;

	cnt = read_c0_count();
	cnt += delta;
	write_c0_compare(cnt);
	res = ((int)(read_c0_count() - cnt) > 0) ? -ETIME : 0;
	return res;
}

#endif /* CONFIG_MIPS_MT_SMTC */

void mips_set_clock_mode(enum clock_event_mode mode,
				struct clock_event_device *evt)
{
	/* Nothing to do ...  */
}

DEFINE_PER_CPU(struct clock_event_device, mips_clockevent_device);
int cp0_timer_irq_installed;

#ifndef CONFIG_MIPS_MT_SMTC

irqreturn_t c0_compare_interrupt(int irq, void *dev_id)
{
	const int r2 = cpu_has_mips_r2;
	struct clock_event_device *cd;
	int cpu = smp_processor_id();

	/*
	 * Suckage alert:
	 * Before R2 of the architecture there was no way to see if a
	 * performance counter interrupt was pending, so we have to run
	 * the performance counter interrupt handler anyway.
	 */
	if (handle_perf_irq(r2))
		goto out;

	/*
	 * The same applies to performance counter interrupts.  But with the
	 * above we now know that the reason we got here must be a timer
	 * interrupt.  Being the paranoiacs we are we check anyway.
	 */
	if (!r2 || (read_c0_cause() & (1 << 30))) {
		/* Clear Count/Compare Interrupt */
		write_c0_compare(read_c0_compare());
		cd = &per_cpu(mips_clockevent_device, cpu);
		cd->event_handler(cd);
	}

out:
	return IRQ_HANDLED;
}

#endif /* Not CONFIG_MIPS_MT_SMTC */

struct irqaction c0_compare_irqaction = {
	.handler = c0_compare_interrupt,
	.flags = IRQF_DISABLED | IRQF_PERCPU,
	.name = "timer",
};


void mips_event_handler(struct clock_event_device *dev)
{
}

/*
 * FIXME: This doesn't hold for the relocated E9000 compare interrupt.
 */
static int c0_compare_int_pending(void)
{
	return (read_c0_cause() >> cp0_compare_irq) & 0x100;
}

/*
 * Compare interrupt can be routed and latched outside the core,
 * so a single execution hazard barrier may not be enough to give
 * it time to clear as seen in the Cause register.  4 time the
 * pipeline depth seems reasonably conservative, and empirically
 * works better in configurations with high CPU/bus clock ratios.
 */

#define compare_change_hazard() \
	do { \
		irq_disable_hazard(); \
		irq_disable_hazard(); \
		irq_disable_hazard(); \
		irq_disable_hazard(); \
	} while (0)

int c0_compare_int_usable(void)
{
	unsigned int delta;
	unsigned int cnt;

	/*
	 * IP7 already pending?  Try to clear it by acking the timer.
	 */
	if (c0_compare_int_pending()) {
		write_c0_compare(read_c0_count());
		compare_change_hazard();
		if (c0_compare_int_pending())
			return 0;
	}

	for (delta = 0x10; delta <= 0x400000; delta <<= 1) {
		cnt = read_c0_count();
		cnt += delta;
		write_c0_compare(cnt);
		compare_change_hazard();
		if ((int)(read_c0_count() - cnt) < 0)
		    break;
		/* increase delta if the timer was already expired */
	}

	while ((int)(read_c0_count() - cnt) <= 0)
		;	/* Wait for expiry  */

	compare_change_hazard();
	if (!c0_compare_int_pending())
		return 0;

	write_c0_compare(read_c0_count());
	compare_change_hazard();
	if (c0_compare_int_pending())
		return 0;

	/*
	 * Feels like a real count / compare timer.
	 */
	return 1;
}

#ifndef CONFIG_MIPS_MT_SMTC

int __cpuinit r4k_clockevent_init(void)
{
	uint64_t mips_freq = mips_hpt_frequency;
	unsigned int cpu = smp_processor_id();
	struct clock_event_device *cd;
	unsigned int irq;

	if (!cpu_has_counter || !mips_hpt_frequency)
		return -ENXIO;

	if (!c0_compare_int_usable())
		return -ENXIO;

	/*
	 * With vectored interrupts things are getting platform specific.
	 * get_c0_compare_int is a hook to allow a platform to return the
	 * interrupt number of it's liking.
	 */
	irq = MIPS_CPU_IRQ_BASE + cp0_compare_irq;
	if (get_c0_compare_int)
		irq = get_c0_compare_int();

	cd = &per_cpu(mips_clockevent_device, cpu);

	cd->name		= "MIPS";
	cd->features		= CLOCK_EVT_FEAT_ONESHOT;

	/* Calculate the min / max delta */
	cd->mult	= div_sc((unsigned long) mips_freq, NSEC_PER_SEC, 32);
	cd->shift		= 32;
	cd->max_delta_ns	= clockevent_delta2ns(0x7fffffff, cd);
	cd->min_delta_ns	= clockevent_delta2ns(0x300, cd);

	cd->rating		= 300;
	cd->irq			= irq;
	cd->cpumask		= cpumask_of(cpu);
	cd->set_next_event	= mips_next_event;
	cd->set_mode		= mips_set_clock_mode;
	cd->event_handler	= mips_event_handler;

	clockevents_register_device(cd);

	if (cp0_timer_irq_installed)
		return 0;

	cp0_timer_irq_installed = 1;

	setup_irq(irq, &c0_compare_irqaction);

	return 0;
}

#endif /* Not CONFIG_MIPS_MT_SMTC */
