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
 *  Amiga mouse driver for Linux/m68k
 *
 *  Copyright (c) 2000-2002 Vojtech Pavlik
 *
 *  Based on the work of:
 *	Michael Rausch		James Banks
 *	Matther Dillon		David Giller
 *	Nathan Laredo		Linus Torvalds
 *	Johan Myreen		Jes Sorensen
 *	Russell King
 */

/*
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2 as published by
 * the Free Software Foundation
 */

#include <linux/module.h>
#include <linux/init.h>
#include <linux/input.h>
#include <linux/interrupt.h>

#include <asm/irq.h>
#include <asm/setup.h>
#include <asm/system.h>
#include <asm/uaccess.h>
#include <asm/amigahw.h>
#include <asm/amigaints.h>

MODULE_AUTHOR("Vojtech Pavlik <vojtech@ucw.cz>");
MODULE_DESCRIPTION("Amiga mouse driver");
MODULE_LICENSE("GPL");

static int amimouse_lastx, amimouse_lasty;
static struct input_dev *amimouse_dev;

static irqreturn_t amimouse_interrupt(int irq, void *dummy)
{
	unsigned short joy0dat, potgor;
	int nx, ny, dx, dy;

	joy0dat = amiga_custom.joy0dat;

	nx = joy0dat & 0xff;
	ny = joy0dat >> 8;

	dx = nx - amimouse_lastx;
	dy = ny - amimouse_lasty;

	if (dx < -127) dx = (256 + nx) - amimouse_lastx;
	if (dx >  127) dx = (nx - 256) - amimouse_lastx;
	if (dy < -127) dy = (256 + ny) - amimouse_lasty;
	if (dy >  127) dy = (ny - 256) - amimouse_lasty;

	amimouse_lastx = nx;
	amimouse_lasty = ny;

	potgor = amiga_custom.potgor;

	input_report_rel(amimouse_dev, REL_X, dx);
	input_report_rel(amimouse_dev, REL_Y, dy);

	input_report_key(amimouse_dev, BTN_LEFT,   ciaa.pra & 0x40);
	input_report_key(amimouse_dev, BTN_MIDDLE, potgor & 0x0100);
	input_report_key(amimouse_dev, BTN_RIGHT,  potgor & 0x0400);

	input_sync(amimouse_dev);

	return IRQ_HANDLED;
}

static int amimouse_open(struct input_dev *dev)
{
	unsigned short joy0dat;

	joy0dat = amiga_custom.joy0dat;

	amimouse_lastx = joy0dat & 0xff;
	amimouse_lasty = joy0dat >> 8;

	if (request_irq(IRQ_AMIGA_VERTB, amimouse_interrupt, 0, "amimouse", amimouse_interrupt)) {
                printk(KERN_ERR "amimouse.c: Can't allocate irq %d\n", IRQ_AMIGA_VERTB);
                return -EBUSY;
        }

        return 0;
}

static void amimouse_close(struct input_dev *dev)
{
	free_irq(IRQ_AMIGA_VERTB, amimouse_interrupt);
}

static int __init amimouse_init(void)
{
	int err;

	if (!MACH_IS_AMIGA || !AMIGAHW_PRESENT(AMI_MOUSE))
		return -ENODEV;

	amimouse_dev = input_allocate_device();
	if (!amimouse_dev)
		return -ENOMEM;

	amimouse_dev->name = "Amiga mouse";
	amimouse_dev->phys = "amimouse/input0";
	amimouse_dev->id.bustype = BUS_AMIGA;
	amimouse_dev->id.vendor = 0x0001;
	amimouse_dev->id.product = 0x0002;
	amimouse_dev->id.version = 0x0100;

	amimouse_dev->evbit[0] = BIT_MASK(EV_KEY) | BIT_MASK(EV_REL);
	amimouse_dev->relbit[0] = BIT_MASK(REL_X) | BIT_MASK(REL_Y);
	amimouse_dev->keybit[BIT_WORD(BTN_LEFT)] = BIT_MASK(BTN_LEFT) |
		BIT_MASK(BTN_MIDDLE) | BIT_MASK(BTN_RIGHT);
	amimouse_dev->open = amimouse_open;
	amimouse_dev->close = amimouse_close;

	err = input_register_device(amimouse_dev);
	if (err) {
		input_free_device(amimouse_dev);
		return err;
	}

	return 0;
}

static void __exit amimouse_exit(void)
{
        input_unregister_device(amimouse_dev);
}

module_init(amimouse_init);
module_exit(amimouse_exit);
