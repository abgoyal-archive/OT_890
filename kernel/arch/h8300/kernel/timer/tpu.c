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
 *  linux/arch/h8300/kernel/timer/tpu.c
 *
 *  Yoshinori Sato <ysato@users.sourceforge.jp>
 *
 *  TPU Timer Handler
 *
 */

#include <linux/config.h>
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
#include <asm/regs267x.h>

/* TPU */
#if CONFIG_H8300_TPU_CH == 0
#define TPUBASE	0xffffd0
#define TPUIRQ	40
#elif CONFIG_H8300_TPU_CH == 1
#define TPUBASE	0xffffe0
#define TPUIRQ	48
#elif CONFIG_H8300_TPU_CH == 2
#define TPUBASE	0xfffff0
#define TPUIRQ	52
#elif CONFIG_H8300_TPU_CH == 3
#define TPUBASE	0xfffe80
#define TPUIRQ	56
#elif CONFIG_H8300_TPU_CH == 4
#define TPUBASE	0xfffe90
#define TPUIRQ	64
#else
#error Unknown timer channel.
#endif

#define _TCR	0
#define _TMDR	1
#define _TIOR	2
#define _TIER	4
#define _TSR	5
#define _TCNT	6
#define _GRA	8
#define _GRB	10

#define CCLR0	0x20

static irqreturn_t timer_interrupt(int irq, void *dev_id)
{
	h8300_timer_tick();
	ctrl_bclr(0, TPUBASE + _TSR);
	return IRQ_HANDLED;
}

static struct irqaction tpu_irq = {
	.name		= "tpu",
	.handler	= timer_interrupt,
	.flags		= IRQF_DISABLED | IRQF_TIMER,
	.mask		= CPU_MASK_NONE,
};

const static int __initdata divide_rate[] = {
#if CONFIG_H8300_TPU_CH == 0
	1,4,16,64,0,0,0,0,
#elif (CONFIG_H8300_TPU_CH == 1) || (CONFIG_H8300_TPU_CH == 5)
	1,4,16,64,0,0,256,0,
#elif (CONFIG_H8300_TPU_CH == 2) || (CONFIG_H8300_TPU_CH == 4)
	1,4,16,64,0,0,0,1024,
#elif CONFIG_H8300_TPU_CH == 3
	1,4,16,64,0,1024,256,4096,
#endif
};

void __init h8300_timer_setup(void)
{
	unsigned int cnt;
	unsigned int div;

	calc_param(cnt, div, divide_rate, 0x10000);

	setup_irq(TPUIRQ, &tpu_irq);

	/* TPU module enabled */
	ctrl_bclr(3, MSTPCRH);

	ctrl_outb(0, TSTR);
	ctrl_outb(CCLR0 | div, TPUBASE + _TCR);
	ctrl_outb(0, TPUBASE + _TMDR);
	ctrl_outw(0, TPUBASE + _TIOR);
	ctrl_outb(0x01, TPUBASE + _TIER);
	ctrl_outw(cnt, TPUBASE + _GRA);
	ctrl_bset(CONFIG_H8300_TPU_CH, TSTR);
}
