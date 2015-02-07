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
 *  linux/arch/arm/common/time-acorn.c
 *
 *  Copyright (c) 1996-2000 Russell King.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 *  Changelog:
 *   24-Sep-1996	RMK	Created
 *   10-Oct-1996	RMK	Brought up to date with arch-sa110eval
 *   04-Dec-1997	RMK	Updated for new arch/arm/time.c
 *   13=Jun-2004	DS	Moved to arch/arm/common b/c shared w/CLPS7500
 */
#include <linux/timex.h>
#include <linux/init.h>
#include <linux/interrupt.h>
#include <linux/irq.h>
#include <linux/io.h>

#include <mach/hardware.h>
#include <asm/hardware/ioc.h>

#include <asm/mach/time.h>

unsigned long ioc_timer_gettimeoffset(void)
{
	unsigned int count1, count2, status;
	long offset;

	ioc_writeb (0, IOC_T0LATCH);
	barrier ();
	count1 = ioc_readb(IOC_T0CNTL) | (ioc_readb(IOC_T0CNTH) << 8);
	barrier ();
	status = ioc_readb(IOC_IRQREQA);
	barrier ();
	ioc_writeb (0, IOC_T0LATCH);
	barrier ();
	count2 = ioc_readb(IOC_T0CNTL) | (ioc_readb(IOC_T0CNTH) << 8);

	offset = count2;
	if (count2 < count1) {
		/*
		 * We have not had an interrupt between reading count1
		 * and count2.
		 */
		if (status & (1 << 5))
			offset -= LATCH;
	} else if (count2 > count1) {
		/*
		 * We have just had another interrupt between reading
		 * count1 and count2.
		 */
		offset -= LATCH;
	}

	offset = (LATCH - offset) * (tick_nsec / 1000);
	return (offset + LATCH/2) / LATCH;
}

void __init ioctime_init(void)
{
	ioc_writeb(LATCH & 255, IOC_T0LTCHL);
	ioc_writeb(LATCH >> 8, IOC_T0LTCHH);
	ioc_writeb(0, IOC_T0GO);
}

static irqreturn_t
ioc_timer_interrupt(int irq, void *dev_id)
{
	timer_tick();
	return IRQ_HANDLED;
}

static struct irqaction ioc_timer_irq = {
	.name		= "timer",
	.flags		= IRQF_DISABLED,
	.handler	= ioc_timer_interrupt
};

/*
 * Set up timer interrupt.
 */
static void __init ioc_timer_init(void)
{
	ioctime_init();
	setup_irq(IRQ_TIMER, &ioc_timer_irq);
}

struct sys_timer ioc_timer = {
	.init		= ioc_timer_init,
	.offset		= ioc_timer_gettimeoffset,
};

