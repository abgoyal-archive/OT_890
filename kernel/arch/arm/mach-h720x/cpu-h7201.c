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
 * linux/arch/arm/mach-h720x/cpu-h7201.c
 *
 * Copyright (C) 2003 Thomas Gleixner <tglx@linutronix.de>
 *               2003 Robert Schwebel <r.schwebel@pengutronix.de>
 *               2004 Sascha Hauer    <s.hauer@pengutronix.de>
 *
 * processor specific stuff for the Hynix h7201
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 */

#include <linux/init.h>
#include <linux/interrupt.h>
#include <linux/module.h>
#include <asm/types.h>
#include <mach/hardware.h>
#include <asm/irq.h>
#include <mach/irqs.h>
#include <asm/mach/irq.h>
#include <asm/mach/time.h>
#include "common.h"
/*
 * Timer interrupt handler
 */
static irqreturn_t
h7201_timer_interrupt(int irq, void *dev_id)
{
	CPU_REG (TIMER_VIRT, TIMER_TOPSTAT);
	timer_tick();

	return IRQ_HANDLED;
}

static struct irqaction h7201_timer_irq = {
	.name		= "h7201 Timer Tick",
	.flags		= IRQF_DISABLED | IRQF_TIMER | IRQF_IRQPOLL,
	.handler	= h7201_timer_interrupt,
};

/*
 * Setup TIMER0 as system timer
 */
void __init h7201_init_time(void)
{
	CPU_REG (TIMER_VIRT, TM0_PERIOD) = LATCH;
	CPU_REG (TIMER_VIRT, TM0_CTRL) = TM_RESET;
	CPU_REG (TIMER_VIRT, TM0_CTRL) = TM_REPEAT | TM_START;
	CPU_REG (TIMER_VIRT, TIMER_TOPCTRL) = ENABLE_TM0_INTR | TIMER_ENABLE_BIT;

	setup_irq(IRQ_TIMER0, &h7201_timer_irq);
}

struct sys_timer h7201_timer = {
	.init		= h7201_init_time,
	.offset		= h720x_gettimeoffset,
};
