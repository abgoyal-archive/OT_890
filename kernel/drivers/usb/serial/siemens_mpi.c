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
 * Siemens USB-MPI Serial USB driver
 *
 * Copyright (C) 2005 Thomas Hergenhahn <thomas.hergenhahn@suse.de>
 * Copyright (C) 2005,2008 Greg Kroah-Hartman <gregkh@suse.de>
 *
 *	This program is free software; you can redistribute it and/or
 *	modify it under the terms of the GNU General Public License version
 *	2 as published by the Free Software Foundation.
 */

#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/tty.h>
#include <linux/module.h>
#include <linux/usb.h>
#include <linux/usb/serial.h>

/* Version Information */
#define DRIVER_VERSION "Version 0.1 09/26/2005"
#define DRIVER_AUTHOR "Thomas Hergenhahn@web.de http://libnodave.sf.net"
#define DRIVER_DESC "Driver for Siemens USB/MPI adapter"


static struct usb_device_id id_table[] = {
	/* Vendor and product id for 6ES7-972-0CB20-0XA0 */
	{ USB_DEVICE(0x908, 0x0004) },
	{ },
};
MODULE_DEVICE_TABLE(usb, id_table);

static struct usb_driver siemens_usb_mpi_driver = {
	.name =		"siemens_mpi",
	.probe =	usb_serial_probe,
	.disconnect =	usb_serial_disconnect,
	.id_table =	id_table,
};

static struct usb_serial_driver siemens_usb_mpi_device = {
	.driver = {
		.owner =	THIS_MODULE,
		.name =		"siemens_mpi",
	},
	.id_table =		id_table,
	.num_ports =		1,
};

static int __init siemens_usb_mpi_init(void)
{
	int retval;

	retval = usb_serial_register(&siemens_usb_mpi_device);
	if (retval)
		goto failed_usb_serial_register;
	retval = usb_register(&siemens_usb_mpi_driver);
	if (retval)
		goto failed_usb_register;
	printk(KERN_INFO DRIVER_DESC "\n");
	printk(KERN_INFO DRIVER_VERSION " " DRIVER_AUTHOR "\n");
	return retval;
failed_usb_register:
	usb_serial_deregister(&siemens_usb_mpi_device);
failed_usb_serial_register:
	return retval;
}

static void __exit siemens_usb_mpi_exit(void)
{
	usb_deregister(&siemens_usb_mpi_driver);
	usb_serial_deregister(&siemens_usb_mpi_device);
}

module_init(siemens_usb_mpi_init);
module_exit(siemens_usb_mpi_exit);
MODULE_AUTHOR(DRIVER_AUTHOR);
MODULE_DESCRIPTION(DRIVER_DESC);
MODULE_LICENSE("GPL");
