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
 *  linux/arch/arm/mach-footbridge/isa-timer.c
 *
 *  Copyright (C) 1998 Russell King.
 *  Copyright (C) 1998 Phil Blundell
 */
#include <linux/init.h>
#include <linux/interrupt.h>
#include <linux/irq.h>
#include <linux/io.h>

#include <asm/irq.h>

#include <asm/mach/time.h>

#include "common.h"

/*
 * ISA timer tick support
 */
#define mSEC_10_from_14 ((14318180 + 100) / 200)

static unsigned long isa_gettimeoffset(void)
{
	int count;

	static int count_p = (mSEC_10_from_14/6);    /* for the first call after boot */
	static unsigned long jiffies_p = 0;

	/*
	 * cache volatile jiffies temporarily; we have IRQs turned off. 
	 */
	unsigned long jiffies_t;

	/* timer count may underflow right here */
	outb_p(0x00, 0x43);	/* latch the count ASAP */

	count = inb_p(0x40);	/* read the latched count */

	/*
	 * We do this guaranteed double memory access instead of a _p 
	 * postfix in the previous port access. Wheee, hackady hack
	 */
 	jiffies_t = jiffies;

	count |= inb_p(0x40) << 8;

	/* Detect timer underflows.  If we haven't had a timer tick since 
	   the last time we were called, and time is apparently going
	   backwards, the counter must have wrapped during this routine. */
	if ((jiffies_t == jiffies_p) && (count > count_p))
		count -= (mSEC_10_from_14/6);
	else
		jiffies_p = jiffies_t;

	count_p = count;

	count = (((mSEC_10_from_14/6)-1) - count) * (tick_nsec / 1000);
	count = (count + (mSEC_10_from_14/6)/2) / (mSEC_10_from_14/6);

	return count;
}

static irqreturn_t
isa_timer_interrupt(int irq, void *dev_id)
{
	timer_tick();
	return IRQ_HANDLED;
}

static struct irqaction isa_timer_irq = {
	.name		= "ISA timer tick",
	.handler	= isa_timer_interrupt,
	.flags		= IRQF_DISABLED | IRQF_TIMER | IRQF_IRQPOLL,
};

static void __init isa_timer_init(void)
{
	isa_rtc_init();

	/* enable PIT timer */
	/* set for periodic (4) and LSB/MSB write (0x30) */
	outb(0x34, 0x43);
	outb((mSEC_10_from_14/6) & 0xFF, 0x40);
	outb((mSEC_10_from_14/6) >> 8, 0x40);

	setup_irq(IRQ_ISA_TIMER, &isa_timer_irq);
}

struct sys_timer isa_timer = {
	.init		= isa_timer_init,
	.offset		= isa_gettimeoffset,
};
