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
 * linux/arch/sh/boards/snapgear/setup.c
 *
 * Copyright (C) 2002  David McCullough <davidm@snapgear.com>
 * Copyright (C) 2003  Paul Mundt <lethal@linux-sh.org>
 *
 * Based on files with the following comments:
 *
 *           Copyright (C) 2000  Kazumoto Kojima
 *
 *           Modified for 7751 Solution Engine by
 *           Ian da Silva and Jeremy Siegel, 2001.
 */
#include <linux/init.h>
#include <linux/irq.h>
#include <linux/interrupt.h>
#include <linux/timer.h>
#include <linux/delay.h>
#include <linux/module.h>
#include <linux/sched.h>
#include <asm/machvec.h>
#include <mach/snapgear.h>
#include <asm/irq.h>
#include <asm/io.h>
#include <cpu/timer.h>

/*
 * EraseConfig handling functions
 */

static irqreturn_t eraseconfig_interrupt(int irq, void *dev_id)
{
	(void)ctrl_inb(0xb8000000);	/* dummy read */

	printk("SnapGear: erase switch interrupt!\n");

	return IRQ_HANDLED;
}

static int __init eraseconfig_init(void)
{
	printk("SnapGear: EraseConfig init\n");
	/* Setup "EraseConfig" switch on external IRQ 0 */
	if (request_irq(IRL0_IRQ, eraseconfig_interrupt, IRQF_DISABLED,
				"Erase Config", NULL))
		printk("SnapGear: failed to register IRQ%d for Reset witch\n",
				IRL0_IRQ);
	else
		printk("SnapGear: registered EraseConfig switch on IRQ%d\n",
				IRL0_IRQ);
	return(0);
}

module_init(eraseconfig_init);

/****************************************************************************/
/*
 * Initialize IRQ setting
 *
 * IRL0 = erase switch
 * IRL1 = eth0
 * IRL2 = eth1
 * IRL3 = crypto
 */

static void __init init_snapgear_IRQ(void)
{
	printk("Setup SnapGear IRQ/IPR ...\n");
	/* enable individual interrupt mode for externals */
	plat_irq_setup_pins(IRQ_MODE_IRQ);
}

/*
 * The Machine Vector
 */
static struct sh_machine_vector mv_snapgear __initmv = {
	.mv_name		= "SnapGear SecureEdge5410",
	.mv_nr_irqs		= 72,

	.mv_inb			= snapgear_inb,
	.mv_inw			= snapgear_inw,
	.mv_inl			= snapgear_inl,
	.mv_outb		= snapgear_outb,
	.mv_outw		= snapgear_outw,
	.mv_outl		= snapgear_outl,

	.mv_inb_p		= snapgear_inb_p,
	.mv_inw_p		= snapgear_inw,
	.mv_inl_p		= snapgear_inl,
	.mv_outb_p		= snapgear_outb_p,
	.mv_outw_p		= snapgear_outw,
	.mv_outl_p		= snapgear_outl,

	.mv_init_irq		= init_snapgear_IRQ,
};
