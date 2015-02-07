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
 * arch/arm/plat-iop/time.c
 *
 * Timer code for IOP32x and IOP33x based systems
 *
 * Author: Deepak Saxena <dsaxena@mvista.com>
 *
 * Copyright 2002-2003 MontaVista Software Inc.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.
 */

#include <linux/kernel.h>
#include <linux/interrupt.h>
#include <linux/time.h>
#include <linux/init.h>
#include <linux/timex.h>
#include <linux/io.h>
#include <mach/hardware.h>
#include <asm/irq.h>
#include <asm/uaccess.h>
#include <asm/mach/irq.h>
#include <asm/mach/time.h>
#include <mach/time.h>

static unsigned long ticks_per_jiffy;
static unsigned long ticks_per_usec;
static unsigned long next_jiffy_time;

unsigned long iop_gettimeoffset(void)
{
	unsigned long offset, temp;

	/* enable cp6, if necessary, to avoid taking the overhead of an
	 * undefined instruction trap
	 */
	asm volatile (
	"mrc	p15, 0, %0, c15, c1, 0\n\t"
	"tst	%0, #(1 << 6)\n\t"
	"orreq	%0, %0, #(1 << 6)\n\t"
	"mcreq	p15, 0, %0, c15, c1, 0\n\t"
#ifdef CONFIG_CPU_XSCALE
	"mrceq	p15, 0, %0, c15, c1, 0\n\t"
	"moveq	%0, %0\n\t"
	"subeq	pc, pc, #4\n\t"
#endif
	: "=r"(temp) : : "cc");

	offset = next_jiffy_time - read_tcr1();

	return offset / ticks_per_usec;
}

static irqreturn_t
iop_timer_interrupt(int irq, void *dev_id)
{
	write_tisr(1);

	while ((signed long)(next_jiffy_time - read_tcr1())
		>= ticks_per_jiffy) {
		timer_tick();
		next_jiffy_time -= ticks_per_jiffy;
	}

	return IRQ_HANDLED;
}

static struct irqaction iop_timer_irq = {
	.name		= "IOP Timer Tick",
	.handler	= iop_timer_interrupt,
	.flags		= IRQF_DISABLED | IRQF_TIMER | IRQF_IRQPOLL,
};

static unsigned long iop_tick_rate;
unsigned long get_iop_tick_rate(void)
{
	return iop_tick_rate;
}
EXPORT_SYMBOL(get_iop_tick_rate);

void __init iop_init_time(unsigned long tick_rate)
{
	u32 timer_ctl;

	ticks_per_jiffy = (tick_rate + HZ/2) / HZ;
	ticks_per_usec = tick_rate / 1000000;
	next_jiffy_time = 0xffffffff;
	iop_tick_rate = tick_rate;

	timer_ctl = IOP_TMR_EN | IOP_TMR_PRIVILEGED |
			IOP_TMR_RELOAD | IOP_TMR_RATIO_1_1;

	/*
	 * We use timer 0 for our timer interrupt, and timer 1 as
	 * monotonic counter for tracking missed jiffies.
	 */
	write_trr0(ticks_per_jiffy - 1);
	write_tmr0(timer_ctl);
	write_trr1(0xffffffff);
	write_tmr1(timer_ctl);

	setup_irq(IRQ_IOP_TIMER0, &iop_timer_irq);
}
