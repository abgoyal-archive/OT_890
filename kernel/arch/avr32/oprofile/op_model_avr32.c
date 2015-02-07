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
 * AVR32 Performance Counter Driver
 *
 * Copyright (C) 2005-2007 Atmel Corporation
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * Author: Ronny Pedersen
 */
#include <linux/errno.h>
#include <linux/interrupt.h>
#include <linux/irq.h>
#include <linux/oprofile.h>
#include <linux/sched.h>
#include <linux/types.h>

#include <asm/sysreg.h>
#include <asm/system.h>

#define AVR32_PERFCTR_IRQ_GROUP	0
#define AVR32_PERFCTR_IRQ_LINE	1

void avr32_backtrace(struct pt_regs * const regs, unsigned int depth);

enum { PCCNT, PCNT0, PCNT1, NR_counter };

struct avr32_perf_counter {
	unsigned long	enabled;
	unsigned long	event;
	unsigned long	count;
	unsigned long	unit_mask;
	unsigned long	kernel;
	unsigned long	user;

	u32		ie_mask;
	u32		flag_mask;
};

static struct avr32_perf_counter counter[NR_counter] = {
	{
		.ie_mask	= SYSREG_BIT(IEC),
		.flag_mask	= SYSREG_BIT(FC),
	}, {
		.ie_mask	= SYSREG_BIT(IE0),
		.flag_mask	= SYSREG_BIT(F0),
	}, {
		.ie_mask	= SYSREG_BIT(IE1),
		.flag_mask	= SYSREG_BIT(F1),
	},
};

static void avr32_perf_counter_reset(void)
{
	/* Reset all counter and disable/clear all interrupts */
	sysreg_write(PCCR, (SYSREG_BIT(PCCR_R)
				| SYSREG_BIT(PCCR_C)
				| SYSREG_BIT(FC)
				| SYSREG_BIT(F0)
				| SYSREG_BIT(F1)));
}

static irqreturn_t avr32_perf_counter_interrupt(int irq, void *dev_id)
{
	struct avr32_perf_counter *ctr = dev_id;
	struct pt_regs *regs;
	u32 pccr;

	if (likely(!(intc_get_pending(AVR32_PERFCTR_IRQ_GROUP)
					& (1 << AVR32_PERFCTR_IRQ_LINE))))
		return IRQ_NONE;

	regs = get_irq_regs();
	pccr = sysreg_read(PCCR);

	/* Clear the interrupt flags we're about to handle */
	sysreg_write(PCCR, pccr);

	/* PCCNT */
	if (ctr->enabled && (pccr & ctr->flag_mask)) {
		sysreg_write(PCCNT, -ctr->count);
		oprofile_add_sample(regs, PCCNT);
	}
	ctr++;
	/* PCNT0 */
	if (ctr->enabled && (pccr & ctr->flag_mask)) {
		sysreg_write(PCNT0, -ctr->count);
		oprofile_add_sample(regs, PCNT0);
	}
	ctr++;
	/* PCNT1 */
	if (ctr->enabled && (pccr & ctr->flag_mask)) {
		sysreg_write(PCNT1, -ctr->count);
		oprofile_add_sample(regs, PCNT1);
	}

	return IRQ_HANDLED;
}

static int avr32_perf_counter_create_files(struct super_block *sb,
		struct dentry *root)
{
	struct dentry *dir;
	unsigned int i;
	char filename[4];

	for (i = 0; i < NR_counter; i++) {
		snprintf(filename, sizeof(filename), "%u", i);
		dir = oprofilefs_mkdir(sb, root, filename);

		oprofilefs_create_ulong(sb, dir, "enabled",
				&counter[i].enabled);
		oprofilefs_create_ulong(sb, dir, "event",
				&counter[i].event);
		oprofilefs_create_ulong(sb, dir, "count",
				&counter[i].count);

		/* Dummy entries */
		oprofilefs_create_ulong(sb, dir, "kernel",
				&counter[i].kernel);
		oprofilefs_create_ulong(sb, dir, "user",
				&counter[i].user);
		oprofilefs_create_ulong(sb, dir, "unit_mask",
				&counter[i].unit_mask);
	}

	return 0;
}

static int avr32_perf_counter_setup(void)
{
	struct avr32_perf_counter *ctr;
	u32 pccr;
	int ret;
	int i;

	pr_debug("avr32_perf_counter_setup\n");

	if (sysreg_read(PCCR) & SYSREG_BIT(PCCR_E)) {
		printk(KERN_ERR
			"oprofile: setup: perf counter already enabled\n");
		return -EBUSY;
	}

	ret = request_irq(AVR32_PERFCTR_IRQ_GROUP,
			avr32_perf_counter_interrupt, IRQF_SHARED,
			"oprofile", counter);
	if (ret)
		return ret;

	avr32_perf_counter_reset();

	pccr = 0;
	for (i = PCCNT; i < NR_counter; i++) {
		ctr = &counter[i];
		if (!ctr->enabled)
			continue;

		pr_debug("enabling counter %d...\n", i);

		pccr |= ctr->ie_mask;

		switch (i) {
		case PCCNT:
			/* PCCNT always counts cycles, so no events */
			sysreg_write(PCCNT, -ctr->count);
			break;
		case PCNT0:
			pccr |= SYSREG_BF(CONF0, ctr->event);
			sysreg_write(PCNT0, -ctr->count);
			break;
		case PCNT1:
			pccr |= SYSREG_BF(CONF1, ctr->event);
			sysreg_write(PCNT1, -ctr->count);
			break;
		}
	}

	pr_debug("oprofile: writing 0x%x to PCCR...\n", pccr);

	sysreg_write(PCCR, pccr);

	return 0;
}

static void avr32_perf_counter_shutdown(void)
{
	pr_debug("avr32_perf_counter_shutdown\n");

	avr32_perf_counter_reset();
	free_irq(AVR32_PERFCTR_IRQ_GROUP, counter);
}

static int avr32_perf_counter_start(void)
{
	pr_debug("avr32_perf_counter_start\n");

	sysreg_write(PCCR, sysreg_read(PCCR) | SYSREG_BIT(PCCR_E));

	return 0;
}

static void avr32_perf_counter_stop(void)
{
	pr_debug("avr32_perf_counter_stop\n");

	sysreg_write(PCCR, sysreg_read(PCCR) & ~SYSREG_BIT(PCCR_E));
}

static struct oprofile_operations avr32_perf_counter_ops __initdata = {
	.create_files	= avr32_perf_counter_create_files,
	.setup		= avr32_perf_counter_setup,
	.shutdown	= avr32_perf_counter_shutdown,
	.start		= avr32_perf_counter_start,
	.stop		= avr32_perf_counter_stop,
	.cpu_type	= "avr32",
};

int __init oprofile_arch_init(struct oprofile_operations *ops)
{
	if (!(current_cpu_data.features & AVR32_FEATURE_PCTR))
		return -ENODEV;

	memcpy(ops, &avr32_perf_counter_ops,
			sizeof(struct oprofile_operations));

	ops->backtrace = avr32_backtrace;

	printk(KERN_INFO "oprofile: using AVR32 performance monitoring.\n");

	return 0;
}

void oprofile_arch_exit(void)
{

}
