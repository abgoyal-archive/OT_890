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
 * arch/arm/mach-l7200/include/mach/time.h
 *
 * Copyright (C) 2000 Rob Scott (rscott@mtrob.fdns.net)
 *                    Steve Hill (sjhill@cotw.com)
 *
 * Changelog:
 *   01-02-2000	RS	Created l7200 version, derived from rpc code
 *   05-03-2000	SJH	Complete rewrite
 */
#ifndef _ASM_ARCH_TIME_H
#define _ASM_ARCH_TIME_H

#include <mach/irqs.h>

/*
 * RTC base register address
 */
#define RTC_BASE	(IO_BASE_2 + 0x2000)

/*
 * RTC registers
 */
#define RTC_RTCDR	(*(volatile unsigned char *) (RTC_BASE + 0x000))
#define RTC_RTCMR	(*(volatile unsigned char *) (RTC_BASE + 0x004))
#define RTC_RTCS	(*(volatile unsigned char *) (RTC_BASE + 0x008))
#define RTC_RTCC	(*(volatile unsigned char *) (RTC_BASE + 0x008))
#define RTC_RTCDV	(*(volatile unsigned char *) (RTC_BASE + 0x00c))
#define RTC_RTCCR	(*(volatile unsigned char *) (RTC_BASE + 0x010))

/*
 * RTCCR register values
 */
#define RTC_RATE_32	0x00      /* 32 Hz tick */
#define RTC_RATE_64	0x10      /* 64 Hz tick */
#define RTC_RATE_128	0x20      /* 128 Hz tick */
#define RTC_RATE_256	0x30      /* 256 Hz tick */
#define RTC_EN_ALARM	0x01      /* Enable alarm */
#define RTC_EN_TIC	0x04      /* Enable counter */
#define RTC_EN_STWDOG	0x08      /* Enable watchdog */

/*
 * Handler for RTC timer interrupt
 */
static irqreturn_t
timer_interrupt(int irq, void *dev_id)
{
	struct pt_regs *regs = get_irq_regs();
	do_timer(1);
#ifndef CONFIG_SMP
	update_process_times(user_mode(regs));
#endif
	do_profile(regs);
	RTC_RTCC = 0;				/* Clear interrupt */

	return IRQ_HANDLED;
}

/*
 * Set up RTC timer interrupt, and return the current time in seconds.
 */
void __init time_init(void)
{
	RTC_RTCC = 0;				/* Clear interrupt */

	timer_irq.handler = timer_interrupt;

	setup_irq(IRQ_RTC_TICK, &timer_irq);

	RTC_RTCCR = RTC_RATE_128 | RTC_EN_TIC;	/* Set rate and enable timer */
}

#endif
