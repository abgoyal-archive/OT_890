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
 * Copyright (C) 2003 Ralf Baechle
 *
 * This program is free software; you can redistribute  it and/or modify it
 * under  the terms of  the GNU General  Public License as published by the
 * Free Software Foundation;  either version 2 of the  License, or (at your
 * option) any later version.
 *
 * Handler for RM9000 extended interrupts.  These are a non-standard
 * feature so we handle them separately from standard interrupts.
 */
#include <linux/init.h>
#include <linux/interrupt.h>
#include <linux/kernel.h>
#include <linux/module.h>

#include <asm/irq_cpu.h>
#include <asm/mipsregs.h>
#include <asm/system.h>

static inline void unmask_rm9k_irq(unsigned int irq)
{
	set_c0_intcontrol(0x1000 << (irq - RM9K_CPU_IRQ_BASE));
}

static inline void mask_rm9k_irq(unsigned int irq)
{
	clear_c0_intcontrol(0x1000 << (irq - RM9K_CPU_IRQ_BASE));
}

static inline void rm9k_cpu_irq_enable(unsigned int irq)
{
	unsigned long flags;

	local_irq_save(flags);
	unmask_rm9k_irq(irq);
	local_irq_restore(flags);
}

/*
 * Performance counter interrupts are global on all processors.
 */
static void local_rm9k_perfcounter_irq_startup(void *args)
{
	unsigned int irq = (unsigned int) args;

	rm9k_cpu_irq_enable(irq);
}

static unsigned int rm9k_perfcounter_irq_startup(unsigned int irq)
{
	on_each_cpu(local_rm9k_perfcounter_irq_startup, (void *) irq, 1);

	return 0;
}

static void local_rm9k_perfcounter_irq_shutdown(void *args)
{
	unsigned int irq = (unsigned int) args;
	unsigned long flags;

	local_irq_save(flags);
	mask_rm9k_irq(irq);
	local_irq_restore(flags);
}

static void rm9k_perfcounter_irq_shutdown(unsigned int irq)
{
	on_each_cpu(local_rm9k_perfcounter_irq_shutdown, (void *) irq, 1);
}

static struct irq_chip rm9k_irq_controller = {
	.name = "RM9000",
	.ack = mask_rm9k_irq,
	.mask = mask_rm9k_irq,
	.mask_ack = mask_rm9k_irq,
	.unmask = unmask_rm9k_irq,
	.eoi	= unmask_rm9k_irq
};

static struct irq_chip rm9k_perfcounter_irq = {
	.name = "RM9000",
	.startup = rm9k_perfcounter_irq_startup,
	.shutdown = rm9k_perfcounter_irq_shutdown,
	.ack = mask_rm9k_irq,
	.mask = mask_rm9k_irq,
	.mask_ack = mask_rm9k_irq,
	.unmask = unmask_rm9k_irq,
};

unsigned int rm9000_perfcount_irq;

EXPORT_SYMBOL(rm9000_perfcount_irq);

void __init rm9k_cpu_irq_init(void)
{
	int base = RM9K_CPU_IRQ_BASE;
	int i;

	clear_c0_intcontrol(0x0000f000);		/* Mask all */

	for (i = base; i < base + 4; i++)
		set_irq_chip_and_handler(i, &rm9k_irq_controller,
					 handle_level_irq);

	rm9000_perfcount_irq = base + 1;
	set_irq_chip_and_handler(rm9000_perfcount_irq, &rm9k_perfcounter_irq,
				 handle_percpu_irq);
}
