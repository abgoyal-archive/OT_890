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
 * HP4x Calculators Serial USB driver
 *
 * Copyright (C) 2005 Arthur Huillet (ahuillet@users.sf.net)
 * Copyright (C) 2001-2005 Greg Kroah-Hartman (greg@kroah.com)
 *
 *	This program is free software; you can redistribute it and/or modify
 *	it under the terms of the GNU General Public License as published by
 *	the Free Software Foundation; either version 2 of the License, or
 *	(at your option) any later version.
 *
 * See Documentation/usb/usb-serial.txt for more information on using this
 * driver
 */

#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/tty.h>
#include <linux/module.h>
#include <linux/usb.h>
#include <linux/usb/serial.h>

/*
 * Version Information
 */
#define DRIVER_VERSION "v1.00"
#define DRIVER_DESC "HP4x (48/49) Generic Serial driver"

#define HP_VENDOR_ID 0x03f0
#define HP49GP_PRODUCT_ID 0x0121

static struct usb_device_id id_table [] = {
	{ USB_DEVICE(HP_VENDOR_ID, HP49GP_PRODUCT_ID) },
	{ }					/* Terminating entry */
};

MODULE_DEVICE_TABLE(usb, id_table);

static struct usb_driver hp49gp_driver = {
	.name =		"hp4X",
	.probe =	usb_serial_probe,
	.disconnect =	usb_serial_disconnect,
	.id_table =	id_table,
	.no_dynamic_id = 	1,
};

static struct usb_serial_driver hp49gp_device = {
	.driver = {
		.owner =	THIS_MODULE,
		.name =		"hp4X",
	},
	.id_table =		id_table,
	.usb_driver = 		&hp49gp_driver,
	.num_ports =		1,
};

static int __init hp49gp_init(void)
{
	int retval;
	retval = usb_serial_register(&hp49gp_device);
	if (retval)
		goto failed_usb_serial_register;
	retval = usb_register(&hp49gp_driver);
	if (retval)
		goto failed_usb_register;
	printk(KERN_INFO KBUILD_MODNAME ": " DRIVER_VERSION ":"
	       DRIVER_DESC "\n");
	return 0;
failed_usb_register:
	usb_serial_deregister(&hp49gp_device);
failed_usb_serial_register:
	return retval;
}

static void __exit hp49gp_exit(void)
{
	usb_deregister(&hp49gp_driver);
	usb_serial_deregister(&hp49gp_device);
}

module_init(hp49gp_init);
module_exit(hp49gp_exit);

MODULE_DESCRIPTION(DRIVER_DESC);
MODULE_VERSION(DRIVER_VERSION);
MODULE_LICENSE("GPL");
