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
 *  HID driver for some sony "special" devices
 *
 *  Copyright (c) 1999 Andreas Gal
 *  Copyright (c) 2000-2005 Vojtech Pavlik <vojtech@suse.cz>
 *  Copyright (c) 2005 Michael Haboustak <mike-@cinci.rr.com> for Concept2, Inc
 *  Copyright (c) 2007 Paul Walmsley
 *  Copyright (c) 2008 Jiri Slaby
 *  Copyright (c) 2006-2008 Jiri Kosina
 */

/*
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation; either version 2 of the License, or (at your option)
 * any later version.
 */

#include <linux/device.h>
#include <linux/hid.h>
#include <linux/module.h>
#include <linux/usb.h>

#include "hid-ids.h"

#define VAIO_RDESC_CONSTANT 0x0001

struct sony_sc {
	unsigned long quirks;
};

/* Sony Vaio VGX has wrongly mouse pointer declared as constant */
static void sony_report_fixup(struct hid_device *hdev, __u8 *rdesc,
		unsigned int rsize)
{
	struct sony_sc *sc = hid_get_drvdata(hdev);

	if ((sc->quirks & VAIO_RDESC_CONSTANT) &&
			rsize >= 56 && rdesc[54] == 0x81 && rdesc[55] == 0x07) {
		dev_info(&hdev->dev, "Fixing up Sony Vaio VGX report "
				"descriptor\n");
		rdesc[55] = 0x06;
	}
}

/*
 * Sending HID_REQ_GET_REPORT changes the operation mode of the ps3 controller
 * to "operational".  Without this, the ps3 controller will not report any
 * events.
 */
static int sony_set_operational(struct hid_device *hdev)
{
	struct usb_interface *intf = to_usb_interface(hdev->dev.parent);
	struct usb_device *dev = interface_to_usbdev(intf);
	__u16 ifnum = intf->cur_altsetting->desc.bInterfaceNumber;
	int ret;
	char *buf = kmalloc(18, GFP_KERNEL);

	if (!buf)
		return -ENOMEM;

	ret = usb_control_msg(dev, usb_rcvctrlpipe(dev, 0),
				 HID_REQ_GET_REPORT,
				 USB_DIR_IN | USB_TYPE_CLASS |
				 USB_RECIP_INTERFACE,
				 (3 << 8) | 0xf2, ifnum, buf, 17,
				 USB_CTRL_GET_TIMEOUT);
	if (ret < 0)
		dev_err(&hdev->dev, "can't set operational mode\n");

	kfree(buf);

	return ret;
}

static int sony_probe(struct hid_device *hdev, const struct hid_device_id *id)
{
	int ret;
	unsigned long quirks = id->driver_data;
	struct sony_sc *sc;

	sc = kzalloc(sizeof(*sc), GFP_KERNEL);
	if (sc == NULL) {
		dev_err(&hdev->dev, "can't alloc apple descriptor\n");
		return -ENOMEM;
	}

	sc->quirks = quirks;
	hid_set_drvdata(hdev, sc);

	ret = hid_parse(hdev);
	if (ret) {
		dev_err(&hdev->dev, "parse failed\n");
		goto err_free;
	}

	ret = hid_hw_start(hdev, HID_CONNECT_DEFAULT |
			HID_CONNECT_HIDDEV_FORCE);
	if (ret) {
		dev_err(&hdev->dev, "hw start failed\n");
		goto err_free;
	}

	ret = sony_set_operational(hdev);
	if (ret < 0)
		goto err_stop;

	return 0;
err_stop:
	hid_hw_stop(hdev);
err_free:
	kfree(sc);
	return ret;
}

static void sony_remove(struct hid_device *hdev)
{
	hid_hw_stop(hdev);
	kfree(hid_get_drvdata(hdev));
}

static const struct hid_device_id sony_devices[] = {
	{ HID_USB_DEVICE(USB_VENDOR_ID_SONY, USB_DEVICE_ID_SONY_PS3_CONTROLLER) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_SONY, USB_DEVICE_ID_SONY_VAIO_VGX_MOUSE),
		.driver_data = VAIO_RDESC_CONSTANT },
	{ }
};
MODULE_DEVICE_TABLE(hid, sony_devices);

static struct hid_driver sony_driver = {
	.name = "sony",
	.id_table = sony_devices,
	.probe = sony_probe,
	.remove = sony_remove,
	.report_fixup = sony_report_fixup,
};

static int sony_init(void)
{
	return hid_register_driver(&sony_driver);
}

static void sony_exit(void)
{
	hid_unregister_driver(&sony_driver);
}

module_init(sony_init);
module_exit(sony_exit);
MODULE_LICENSE("GPL");

HID_COMPAT_LOAD_DRIVER(sony);
