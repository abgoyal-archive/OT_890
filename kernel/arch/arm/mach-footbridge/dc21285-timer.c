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
 *  linux/arch/arm/mach-footbridge/dc21285-timer.c
 *
 *  Copyright (C) 1998 Russell King.
 *  Copyright (C) 1998 Phil Blundell
 */
#include <linux/init.h>
#include <linux/interrupt.h>
#include <linux/irq.h>
#include <linux/spinlock.h>

#include <asm/irq.h>

#include <asm/hardware/dec21285.h>
#include <asm/mach/time.h>

#include "common.h"

/*
 * Footbridge timer 1 support.
 */
static unsigned long timer1_latch;

static unsigned long timer1_gettimeoffset (void)
{
	unsigned long value = timer1_latch - *CSR_TIMER1_VALUE;

	return ((tick_nsec / 1000) * value) / timer1_latch;
}

static irqreturn_t
timer1_interrupt(int irq, void *dev_id)
{
	*CSR_TIMER1_CLR = 0;

	timer_tick();

	return IRQ_HANDLED;
}

static struct irqaction footbridge_timer_irq = {
	.name		= "Timer1 timer tick",
	.handler	= timer1_interrupt,
	.flags		= IRQF_DISABLED | IRQF_TIMER | IRQF_IRQPOLL,
};

/*
 * Set up timer interrupt.
 */
static void __init footbridge_timer_init(void)
{
	timer1_latch = (mem_fclk_21285 + 8 * HZ) / (16 * HZ);

	*CSR_TIMER1_CLR  = 0;
	*CSR_TIMER1_LOAD = timer1_latch;
	*CSR_TIMER1_CNTL = TIMER_CNTL_ENABLE | TIMER_CNTL_AUTORELOAD | TIMER_CNTL_DIV16;

	setup_irq(IRQ_TIMER1, &footbridge_timer_irq);

	isa_rtc_init();
}

struct sys_timer footbridge_timer = {
	.init		= footbridge_timer_init,
	.offset		= timer1_gettimeoffset,
};
