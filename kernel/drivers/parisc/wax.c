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
 *	WAX Device Driver
 *
 *	(c) Copyright 2000 The Puffin Group Inc.
 *
 *	This program is free software; you can redistribute it and/or modify
 *	it under the terms of the GNU General Public License as published by
 *      the Free Software Foundation; either version 2 of the License, or
 *      (at your option) any later version.
 *
 *	by Helge Deller <deller@gmx.de>
 */

#include <linux/errno.h>
#include <linux/init.h>
#include <linux/interrupt.h>
#include <linux/ioport.h>
#include <linux/slab.h>
#include <linux/module.h>
#include <linux/types.h>

#include <asm/io.h>
#include <asm/hardware.h>

#include "gsc.h"

#define WAX_GSC_IRQ	7	/* Hardcoded Interrupt for GSC */

static void wax_choose_irq(struct parisc_device *dev, void *ctrl)
{
	int irq;

	switch (dev->id.sversion) {
		case 0x73:	irq =  1; break; /* i8042 General */
		case 0x8c:	irq =  6; break; /* Serial */
		case 0x90:	irq = 10; break; /* EISA */
		default:	return;		 /* Unknown */
	}

	gsc_asic_assign_irq(ctrl, irq, &dev->irq);

	switch (dev->id.sversion) {
		case 0x73:	irq =  2; break; /* i8042 High-priority */
		case 0x90:	irq =  0; break; /* EISA NMI */
		default:	return;		 /* No secondary IRQ */
	}

	gsc_asic_assign_irq(ctrl, irq, &dev->aux_irq);
}

static void __init
wax_init_irq(struct gsc_asic *wax)
{
	unsigned long base = wax->hpa;

	/* Wax-off */
	gsc_writel(0x00000000, base+OFFSET_IMR);

	/* clear pending interrupts */
	gsc_readl(base+OFFSET_IRR);

	/* We're not really convinced we want to reset the onboard
         * devices. Firmware does it for us...
	 */

	/* Resets */
//	gsc_writel(0xFFFFFFFF, base+0x1000); /* HIL */
//	gsc_writel(0xFFFFFFFF, base+0x2000); /* RS232-B on Wax */
}

static int __init wax_init_chip(struct parisc_device *dev)
{
	struct gsc_asic *wax;
	struct parisc_device *parent;
	struct gsc_irq gsc_irq;
	int ret;

	wax = kzalloc(sizeof(*wax), GFP_KERNEL);
	if (!wax)
		return -ENOMEM;

	wax->name = "wax";
	wax->hpa = dev->hpa.start;

	wax->version = 0;   /* gsc_readb(wax->hpa+WAX_VER); */
	printk(KERN_INFO "%s at 0x%lx found.\n", wax->name, wax->hpa);

	/* Stop wax hissing for a bit */
	wax_init_irq(wax);

	/* the IRQ wax should use */
	dev->irq = gsc_claim_irq(&gsc_irq, WAX_GSC_IRQ);
	if (dev->irq < 0) {
		printk(KERN_ERR "%s(): cannot get GSC irq\n",
				__func__);
		kfree(wax);
		return -EBUSY;
	}

	wax->eim = ((u32) gsc_irq.txn_addr) | gsc_irq.txn_data;

	ret = request_irq(gsc_irq.irq, gsc_asic_intr, 0, "wax", wax);
	if (ret < 0) {
		kfree(wax);
		return ret;
	}

	/* enable IRQ's for devices below WAX */
	gsc_writel(wax->eim, wax->hpa + OFFSET_IAR);

	/* Done init'ing, register this driver */
	ret = gsc_common_setup(dev, wax);
	if (ret) {
		kfree(wax);
		return ret;
	}

	gsc_fixup_irqs(dev, wax, wax_choose_irq);
	/* On 715-class machines, Wax EISA is a sibling of Wax, not a child. */
	parent = parisc_parent(dev);
	if (parent->id.hw_type != HPHW_IOA) {
		gsc_fixup_irqs(parent, wax, wax_choose_irq);
	}

	return ret;
}

static struct parisc_device_id wax_tbl[] = {
  	{ HPHW_BA, HVERSION_REV_ANY_ID, HVERSION_ANY_ID, 0x0008e },
	{ 0, }
};

MODULE_DEVICE_TABLE(parisc, wax_tbl);

struct parisc_driver wax_driver = {
	.name =		"wax",
	.id_table =	wax_tbl,
	.probe =	wax_init_chip,
};
