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
 * Motorola USB Phone driver
 *
 * Copyright (C) 2008 Greg Kroah-Hartman <greg@kroah.com>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License version 2 as
 *  published by the Free Software Foundation.
 *
 * {sigh}
 * Mororola should be using the CDC ACM USB spec, but instead
 * they try to just "do their own thing"...  This driver should handle a
 * few phones in which a basic "dumb serial connection" is needed to be
 * able to get a connection through to them.
 */

#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/tty.h>
#include <linux/module.h>
#include <linux/usb.h>
#include <linux/usb/serial.h>

static struct usb_device_id id_table [] = {
	{ USB_DEVICE(0x05c6, 0x3197) },	/* unknown Motorola phone */
	{ USB_DEVICE(0x0c44, 0x0022) },	/* unknown Mororola phone */
	{ USB_DEVICE(0x22b8, 0x2a64) },	/* Motorola KRZR K1m */
	{ },
};
MODULE_DEVICE_TABLE(usb, id_table);

static struct usb_driver moto_driver = {
	.name =		"moto-modem",
	.probe =	usb_serial_probe,
	.disconnect =	usb_serial_disconnect,
	.id_table =	id_table,
	.no_dynamic_id = 	1,
};

static struct usb_serial_driver moto_device = {
	.driver = {
		.owner =	THIS_MODULE,
		.name =		"moto-modem",
	},
	.id_table =		id_table,
	.num_ports =		1,
};

static int __init moto_init(void)
{
	int retval;

	retval = usb_serial_register(&moto_device);
	if (retval)
		return retval;
	retval = usb_register(&moto_driver);
	if (retval)
		usb_serial_deregister(&moto_device);
	return retval;
}

static void __exit moto_exit(void)
{
	usb_deregister(&moto_driver);
	usb_serial_deregister(&moto_device);
}

module_init(moto_init);
module_exit(moto_exit);
MODULE_LICENSE("GPL");
