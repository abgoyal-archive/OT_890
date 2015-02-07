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
 * USB BlackBerry charging module
 *
 * Copyright (C) 2007 Greg Kroah-Hartman <gregkh@suse.de>
 *
 *	This program is free software; you can redistribute it and/or
 *	modify it under the terms of the GNU General Public License as
 *	published by the Free Software Foundation, version 2.
 *
 * Information on how to switch configs was taken by the bcharge.cc file
 * created by the barry.sf.net project.
 *
 * bcharge.cc has the following copyright:
 * 	Copyright (C) 2006, Net Direct Inc. (http://www.netdirect.ca/)
 * and is released under the GPLv2.
 *
 *
 */

#include <linux/kernel.h>
#include <linux/errno.h>
#include <linux/init.h>
#include <linux/slab.h>
#include <linux/module.h>
#include <linux/usb.h>

#define RIM_VENDOR		0x0fca
#define BLACKBERRY		0x0001
#define BLACKBERRY_PEARL_DUAL   0x0004
#define BLACKBERRY_PEARL        0x0006

static int debug;
static int pearl_dual_mode = 1;

#ifdef dbg
#undef dbg
#endif
#define dbg(dev, format, arg...)				\
	if (debug)						\
		dev_printk(KERN_DEBUG , dev , format , ## arg)

static struct usb_device_id id_table [] = {
	{ USB_DEVICE(RIM_VENDOR, BLACKBERRY) },
	{ USB_DEVICE(RIM_VENDOR, BLACKBERRY_PEARL) },
	{ USB_DEVICE(RIM_VENDOR, BLACKBERRY_PEARL_DUAL) },
	{ },					/* Terminating entry */
};
MODULE_DEVICE_TABLE(usb, id_table);

static int magic_charge(struct usb_device *udev)
{
	char *dummy_buffer = kzalloc(2, GFP_KERNEL);
	int retval;

	if (!dummy_buffer)
		return -ENOMEM;

	/* send two magic commands and then set the configuration.  The device
	 * will then reset itself with the new power usage and should start
	 * charging. */

	/* Note, with testing, it only seems that the first message is really
	 * needed (at least for the 8700c), but to be safe, we emulate what
	 * other operating systems seem to be sending to their device.  We
	 * really need to get some specs for this device to be sure about what
	 * is going on here.
	 */
	dbg(&udev->dev, "Sending first magic command\n");
	retval = usb_control_msg(udev, usb_rcvctrlpipe(udev, 0),
				 0xa5, 0xc0, 0, 1, dummy_buffer, 2, 100);
	if (retval != 2) {
		dev_err(&udev->dev, "First magic command failed: %d.\n",
			retval);
		goto exit;
	}

	dbg(&udev->dev, "Sending second magic command\n");
	retval = usb_control_msg(udev, usb_rcvctrlpipe(udev, 0),
				 0xa2, 0x40, 0, 1, dummy_buffer, 0, 100);
	if (retval != 0) {
		dev_err(&udev->dev, "Second magic command failed: %d.\n",
			retval);
		goto exit;
	}

	dbg(&udev->dev, "Calling set_configuration\n");
	retval = usb_driver_set_configuration(udev, 1);
	if (retval)
		dev_err(&udev->dev, "Set Configuration failed :%d.\n", retval);

exit:
	kfree(dummy_buffer);
	return retval;
}

static int magic_dual_mode(struct usb_device *udev)
{
	char *dummy_buffer = kzalloc(2, GFP_KERNEL);
	int retval;

	if (!dummy_buffer)
		return -ENOMEM;

	/* send magic command so that the Blackberry Pearl device exposes
	 * two interfaces: both the USB mass-storage one and one which can
	 * be used for database access. */
	dbg(&udev->dev, "Sending magic pearl command\n");
	retval = usb_control_msg(udev, usb_rcvctrlpipe(udev, 0),
				 0xa9, 0xc0, 1, 1, dummy_buffer, 2, 100);
	dbg(&udev->dev, "Magic pearl command returned %d\n", retval);

	dbg(&udev->dev, "Calling set_configuration\n");
	retval = usb_driver_set_configuration(udev, 1);
	if (retval)
		dev_err(&udev->dev, "Set Configuration failed :%d.\n", retval);

	kfree(dummy_buffer);
	return retval;
}

static int berry_probe(struct usb_interface *intf,
		       const struct usb_device_id *id)
{
	struct usb_device *udev = interface_to_usbdev(intf);

	if (udev->bus_mA < 500) {
		dbg(&udev->dev, "Not enough power to charge available\n");
		return -ENODEV;
	}

	dbg(&udev->dev, "Power is set to %dmA\n",
	    udev->actconfig->desc.bMaxPower * 2);

	/* check the power usage so we don't try to enable something that is
	 * already enabled */
	if ((udev->actconfig->desc.bMaxPower * 2) == 500) {
		dbg(&udev->dev, "device is already charging, power is "
		    "set to %dmA\n", udev->actconfig->desc.bMaxPower * 2);
		return -ENODEV;
	}

	/* turn the power on */
	magic_charge(udev);

	if ((le16_to_cpu(udev->descriptor.idProduct) == BLACKBERRY_PEARL) &&
	    (pearl_dual_mode))
		magic_dual_mode(udev);

	/* we don't really want to bind to the device, userspace programs can
	 * handle the syncing just fine, so get outta here. */
	return -ENODEV;
}

static void berry_disconnect(struct usb_interface *intf)
{
}

static struct usb_driver berry_driver = {
	.name =		"berry_charge",
	.probe =	berry_probe,
	.disconnect =	berry_disconnect,
	.id_table =	id_table,
};

static int __init berry_init(void)
{
	return usb_register(&berry_driver);
}

static void __exit berry_exit(void)
{
	usb_deregister(&berry_driver);
}

module_init(berry_init);
module_exit(berry_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Greg Kroah-Hartman <gregkh@suse.de>");
module_param(debug, bool, S_IRUGO | S_IWUSR);
MODULE_PARM_DESC(debug, "Debug enabled or not");
module_param(pearl_dual_mode, bool, S_IRUGO | S_IWUSR);
MODULE_PARM_DESC(pearl_dual_mode, "Change Blackberry Pearl to run in dual mode");
