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
 * arch/arm/mach-at91/at91x40.c
 *
 * (C) Copyright 2007, Greg Ungerer <gerg@snapgear.com>
 * Copyright (C) 2005 SAN People
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */

#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/irq.h>
#include <asm/mach/arch.h>
#include <mach/at91x40.h>
#include <mach/at91_st.h>
#include <mach/timex.h>
#include "generic.h"

/*
 * Export the clock functions for the AT91X40. Some external code common
 * to all AT91 family parts relys on this, like the gpio and serial support.
 */
int clk_enable(struct clk *clk)
{
	return 0;
}

void clk_disable(struct clk *clk)
{
}

unsigned long clk_get_rate(struct clk *clk)
{
	return AT91X40_MASTER_CLOCK;
}

struct clk *clk_get(struct device *dev, const char *id)
{
	return NULL;
}

void __init at91x40_initialize(unsigned long main_clock)
{
	at91_extern_irq = (1 << AT91X40_ID_IRQ0) | (1 << AT91X40_ID_IRQ1)
			| (1 << AT91X40_ID_IRQ2);
}

/*
 * The default interrupt priority levels (0 = lowest, 7 = highest).
 */
static unsigned int at91x40_default_irq_priority[NR_AIC_IRQS] __initdata = {
	7,	/* Advanced Interrupt Controller (FIQ) */
	0,	/* System Peripherals */
	0,	/* USART 0 */
	0,	/* USART 1 */
	2,	/* Timer Counter 0 */
	2,	/* Timer Counter 1 */
	2,	/* Timer Counter 2 */
	0,	/* Watchdog timer */
	0,	/* Parallel IO Controller A */
	0,	/* Reserved */
	0,	/* Reserved */
	0,	/* Reserved */
	0,	/* Reserved */
	0,	/* Reserved */
	0,	/* Reserved */
	0,	/* Reserved */
	0,	/* External IRQ0 */
	0,	/* External IRQ1 */
	0,	/* External IRQ2 */
};

void __init at91x40_init_interrupts(unsigned int priority[NR_AIC_IRQS])
{
	if (!priority)
		priority = at91x40_default_irq_priority;

	at91_aic_init(priority);
}

