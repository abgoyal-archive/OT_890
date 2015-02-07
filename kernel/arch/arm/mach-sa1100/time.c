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
 * linux/arch/arm/mach-sa1100/time.c
 *
 * Copyright (C) 1998 Deborah Wallach.
 * Twiddles  (C) 1999 Hugo Fiennes <hugo@empeg.com>
 *
 * 2000/03/29 (C) Nicolas Pitre <nico@cam.org>
 *	Rewritten: big cleanup, much simpler, better HZ accuracy.
 *
 */
#include <linux/init.h>
#include <linux/errno.h>
#include <linux/interrupt.h>
#include <linux/irq.h>
#include <linux/timex.h>
#include <linux/clockchips.h>

#include <asm/mach/time.h>
#include <mach/hardware.h>

#define MIN_OSCR_DELTA 2

static irqreturn_t sa1100_ost0_interrupt(int irq, void *dev_id)
{
	struct clock_event_device *c = dev_id;

	/* Disarm the compare/match, signal the event. */
	OIER &= ~OIER_E0;
	OSSR = OSSR_M0;
	c->event_handler(c);

	return IRQ_HANDLED;
}

static int
sa1100_osmr0_set_next_event(unsigned long delta, struct clock_event_device *c)
{
	unsigned long flags, next, oscr;

	raw_local_irq_save(flags);
	OIER |= OIER_E0;
	next = OSCR + delta;
	OSMR0 = next;
	oscr = OSCR;
	raw_local_irq_restore(flags);

	return (signed)(next - oscr) <= MIN_OSCR_DELTA ? -ETIME : 0;
}

static void
sa1100_osmr0_set_mode(enum clock_event_mode mode, struct clock_event_device *c)
{
	unsigned long flags;

	switch (mode) {
	case CLOCK_EVT_MODE_ONESHOT:
	case CLOCK_EVT_MODE_UNUSED:
	case CLOCK_EVT_MODE_SHUTDOWN:
		raw_local_irq_save(flags);
		OIER &= ~OIER_E0;
		OSSR = OSSR_M0;
		raw_local_irq_restore(flags);
		break;

	case CLOCK_EVT_MODE_RESUME:
	case CLOCK_EVT_MODE_PERIODIC:
		break;
	}
}

static struct clock_event_device ckevt_sa1100_osmr0 = {
	.name		= "osmr0",
	.features	= CLOCK_EVT_FEAT_ONESHOT,
	.shift		= 32,
	.rating		= 200,
	.set_next_event	= sa1100_osmr0_set_next_event,
	.set_mode	= sa1100_osmr0_set_mode,
};

static cycle_t sa1100_read_oscr(void)
{
	return OSCR;
}

static struct clocksource cksrc_sa1100_oscr = {
	.name		= "oscr",
	.rating		= 200,
	.read		= sa1100_read_oscr,
	.mask		= CLOCKSOURCE_MASK(32),
	.shift		= 20,
	.flags		= CLOCK_SOURCE_IS_CONTINUOUS,
};

static struct irqaction sa1100_timer_irq = {
	.name		= "ost0",
	.flags		= IRQF_DISABLED | IRQF_TIMER | IRQF_IRQPOLL,
	.handler	= sa1100_ost0_interrupt,
	.dev_id		= &ckevt_sa1100_osmr0,
};

static void __init sa1100_timer_init(void)
{
	OIER = 0;		/* disable any timer interrupts */
	OSSR = 0xf;		/* clear status on all timers */

	ckevt_sa1100_osmr0.mult =
		div_sc(3686400, NSEC_PER_SEC, ckevt_sa1100_osmr0.shift);
	ckevt_sa1100_osmr0.max_delta_ns =
		clockevent_delta2ns(0x7fffffff, &ckevt_sa1100_osmr0);
	ckevt_sa1100_osmr0.min_delta_ns =
		clockevent_delta2ns(MIN_OSCR_DELTA * 2, &ckevt_sa1100_osmr0) + 1;
	ckevt_sa1100_osmr0.cpumask = cpumask_of(0);

	cksrc_sa1100_oscr.mult =
		clocksource_hz2mult(CLOCK_TICK_RATE, cksrc_sa1100_oscr.shift);

	setup_irq(IRQ_OST0, &sa1100_timer_irq);

	clocksource_register(&cksrc_sa1100_oscr);
	clockevents_register_device(&ckevt_sa1100_osmr0);
}

#ifdef CONFIG_PM
unsigned long osmr[4], oier;

static void sa1100_timer_suspend(void)
{
	osmr[0] = OSMR0;
	osmr[1] = OSMR1;
	osmr[2] = OSMR2;
	osmr[3] = OSMR3;
	oier = OIER;
}

static void sa1100_timer_resume(void)
{
	OSSR = 0x0f;
	OSMR0 = osmr[0];
	OSMR1 = osmr[1];
	OSMR2 = osmr[2];
	OSMR3 = osmr[3];
	OIER = oier;

	/*
	 * OSMR0 is the system timer: make sure OSCR is sufficiently behind
	 */
	OSCR = OSMR0 - LATCH;
}
#else
#define sa1100_timer_suspend NULL
#define sa1100_timer_resume NULL
#endif

struct sys_timer sa1100_timer = {
	.init		= sa1100_timer_init,
	.suspend	= sa1100_timer_suspend,
	.resume		= sa1100_timer_resume,
};
