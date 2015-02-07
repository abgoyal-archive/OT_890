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
 *  linux/arch/h8300/kernel/timer/itu.c
 *
 *  Yoshinori Sato <ysato@users.sourcefoge.jp>
 *
 *  ITU Timer Handler
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

#if CONFIG_H8300_ITU_CH == 0
#define ITUBASE	0xffff64
#define ITUIRQ	24
#elif CONFIG_H8300_ITU_CH == 1
#define ITUBASE	0xffff6e
#define ITUIRQ	28
#elif CONFIG_H8300_ITU_CH == 2
#define ITUBASE	0xffff78
#define ITUIRQ	32
#elif CONFIG_H8300_ITU_CH == 3
#define ITUBASE	0xffff82
#define ITUIRQ	36
#elif CONFIG_H8300_ITU_CH == 4
#define ITUBASE	0xffff92
#define ITUIRQ	40
#else
#error Unknown timer channel.
#endif

#define TCR	0
#define TIOR	1
#define TIER	2
#define TSR	3
#define TCNT	4
#define GRA	6
#define GRB	8

static irqreturn_t timer_interrupt(int irq, void *dev_id)
{
	h8300_timer_tick();
	ctrl_bclr(IMFA, ITUBASE + TSR);
	return IRQ_HANDLED;
}

static struct irqaction itu_irq = {
	.name		= "itu",
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

	setup_irq(ITUIRQ, &itu_irq);

	/* initalize timer */
	ctrl_outb(0, TSTR);
	ctrl_outb(CCLR0 | div, ITUBASE + TCR);
	ctrl_outb(0x01, ITUBASE + TIER);
	ctrl_outw(cnt, ITUBASE + GRA);
	ctrl_bset(CONFIG_H8300_ITU_CH, TSTR);
}
