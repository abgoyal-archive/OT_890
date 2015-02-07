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
 *  PC Speaker beeper driver for Linux
 *
 *  Copyright (c) 2002 Vojtech Pavlik
 *  Copyright (c) 1992 Orest Zborowski
 *
 */

/*
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2 as published by
 * the Free Software Foundation
 */

#include <linux/init.h>
#include <linux/input.h>
#include <asm/io.h>
#include "pcsp.h"

static void pcspkr_do_sound(unsigned int count)
{
	unsigned long flags;

	spin_lock_irqsave(&i8253_lock, flags);

	if (count) {
		/* set command for counter 2, 2 byte write */
		outb_p(0xB6, 0x43);
		/* select desired HZ */
		outb_p(count & 0xff, 0x42);
		outb((count >> 8) & 0xff, 0x42);
		/* enable counter 2 */
		outb_p(inb_p(0x61) | 3, 0x61);
	} else {
		/* disable counter 2 */
		outb(inb_p(0x61) & 0xFC, 0x61);
	}

	spin_unlock_irqrestore(&i8253_lock, flags);
}

void pcspkr_stop_sound(void)
{
	pcspkr_do_sound(0);
}

static int pcspkr_input_event(struct input_dev *dev, unsigned int type,
			      unsigned int code, int value)
{
	unsigned int count = 0;

	if (atomic_read(&pcsp_chip.timer_active) || !pcsp_chip.pcspkr)
		return 0;

	switch (type) {
	case EV_SND:
		switch (code) {
		case SND_BELL:
			if (value)
				value = 1000;
		case SND_TONE:
			break;
		default:
			return -1;
		}
		break;

	default:
		return -1;
	}

	if (value > 20 && value < 32767)
		count = PIT_TICK_RATE / value;

	pcspkr_do_sound(count);

	return 0;
}

int __devinit pcspkr_input_init(struct input_dev **rdev, struct device *dev)
{
	int err;

	struct input_dev *input_dev = input_allocate_device();
	if (!input_dev)
		return -ENOMEM;

	input_dev->name = "PC Speaker";
	input_dev->phys = "isa0061/input0";
	input_dev->id.bustype = BUS_ISA;
	input_dev->id.vendor = 0x001f;
	input_dev->id.product = 0x0001;
	input_dev->id.version = 0x0100;
	input_dev->dev.parent = dev;

	input_dev->evbit[0] = BIT(EV_SND);
	input_dev->sndbit[0] = BIT(SND_BELL) | BIT(SND_TONE);
	input_dev->event = pcspkr_input_event;

	err = input_register_device(input_dev);
	if (err) {
		input_free_device(input_dev);
		return err;
	}

	*rdev = input_dev;
	return 0;
}

int pcspkr_input_remove(struct input_dev *dev)
{
	pcspkr_stop_sound();
	input_unregister_device(dev);	/* this also does kfree() */

	return 0;
}
