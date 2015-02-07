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
 *  linux/arch/h8300/kernel/timer/timer16.c
 *
 *  Yoshinori Sato <ysato@users.sourcefoge.jp>
 *
 *  16bit Timer Handler
 *
 */

#include <linux/errno.h>
#include <linux/sched.h>
#include <linux/kernel.h>
#include <linux/param.h>
#include <linux/string.h>
#include <linux/mm.h>
#include <linux/interrupt.h>
#include <linux/init.h>
#include <linux/timex.h>

#include <asm/segment.h>
#include <asm/io.h>
#include <asm/irq.h>
#include <asm/regs306x.h>

/* 16bit timer */
#if CONFIG_H8300_TIMER16_CH == 0
#define _16BASE	0xffff78
#define _16IRQ	24
#elif CONFIG_H8300_TIMER16_CH == 1
#define _16BASE	0xffff80
#define _16IRQ	28
#elif CONFIG_H8300_TIMER16_CH == 2
#define _16BASE	0xffff88
#define _16IRQ	32
#else
#error Unknown timer channel.
#endif

#define TCR	0
#define TIOR	1
#define TCNT	2
#define GRA	4
#define GRB	6

#define H8300_TIMER_FREQ CONFIG_CPU_CLOCK*10000 /* Timer input freq. */

static irqreturn_t timer_interrupt(int irq, void *dev_id)
{
	h8300_timer_tick();
	ctrl_bclr(CONFIG_H8300_TIMER16_CH, TISRA);
	return IRQ_HANDLED;
}

static struct irqaction timer16_irq = {
	.name		= "timer-16",
	.handler	= timer_interrupt,
	.flags		= IRQF_DISABLED | IRQF_TIMER,
	.mask		= CPU_MASK_NONE,
};

static const int __initdata divide_rate[] = {1, 2, 4, 8};

void __init h8300_timer_setup(void)
{
	unsigned int div;
	unsigned int cnt;

	calc_param(cnt, div, divide_rate, 0x10000);

	setup_irq(_16IRQ, &timer16_irq);

	/* initalize timer */
	ctrl_outb(0, TSTR);
	ctrl_outb(CCLR0 | div, _16BASE + TCR);
	ctrl_outw(cnt, _16BASE + GRA);
	ctrl_bset(4 + CONFIG_H8300_TIMER16_CH, TISRA);
	ctrl_bset(CONFIG_H8300_TIMER16_CH, TSTR);
}
