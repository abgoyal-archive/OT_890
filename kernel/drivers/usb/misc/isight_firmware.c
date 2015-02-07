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
 * Driver for loading USB isight firmware
 *
 * Copyright (C) 2008 Matthew Garrett <mjg@redhat.com>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation, version 2.
 *
 * The USB isight cameras in recent Apples are roughly compatible with the USB
 * video class specification, and can be driven by uvcvideo. However, they
 * need firmware to be loaded beforehand. After firmware loading, the device
 * detaches from the USB bus and reattaches with a new device ID. It can then
 * be claimed by the uvc driver.
 *
 * The firmware is non-free and must be extracted by the user. Tools to do this
 * are available at http://bersace03.free.fr/ift/
 *
 * The isight firmware loading was reverse engineered by Johannes Berg
 * <johannes@sipsolutions.de>, and this driver is based on code by Ronald
 * Bultje <rbultje@ronald.bitfreak.net>
 */

#include <linux/usb.h>
#include <linux/firmware.h>
#include <linux/errno.h>
#include <linux/module.h>

static struct usb_device_id id_table[] = {
	{USB_DEVICE(0x05ac, 0x8300)},
	{},
};

MODULE_DEVICE_TABLE(usb, id_table);

static int isight_firmware_load(struct usb_interface *intf,
				const struct usb_device_id *id)
{
	struct usb_device *dev = interface_to_usbdev(intf);
	int llen, len, req, ret = 0;
	const struct firmware *firmware;
	unsigned char *buf = kmalloc(50, GFP_KERNEL);
	unsigned char data[4];
	const u8 *ptr;

	if (!buf)
		return -ENOMEM;

	if (request_firmware(&firmware, "isight.fw", &dev->dev) != 0) {
		printk(KERN_ERR "Unable to load isight firmware\n");
		ret = -ENODEV;
		goto out;
	}

	ptr = firmware->data;

	if (usb_control_msg
	    (dev, usb_sndctrlpipe(dev, 0), 0xa0, 0x40, 0xe600, 0, "\1", 1,
	     300) != 1) {
		printk(KERN_ERR
		       "Failed to initialise isight firmware loader\n");
		ret = -ENODEV;
		goto out;
	}

	while (ptr+4 <= firmware->data+firmware->size) {
		memcpy(data, ptr, 4);
		len = (data[0] << 8 | data[1]);
		req = (data[2] << 8 | data[3]);
		ptr += 4;

		if (len == 0x8001)
			break;	/* success */
		else if (len == 0)
			continue;

		for (; len > 0; req += 50) {
			llen = min(len, 50);
			len -= llen;
			if (ptr+llen > firmware->data+firmware->size) {
				printk(KERN_ERR
				       "Malformed isight firmware");
				ret = -ENODEV;
				goto out;
			}
			memcpy(buf, ptr, llen);

			ptr += llen;

			if (usb_control_msg
			    (dev, usb_sndctrlpipe(dev, 0), 0xa0, 0x40, req, 0,
			     buf, llen, 300) != llen) {
				printk(KERN_ERR
				       "Failed to load isight firmware\n");
				ret = -ENODEV;
				goto out;
			}

		}
	}

	if (usb_control_msg
	    (dev, usb_sndctrlpipe(dev, 0), 0xa0, 0x40, 0xe600, 0, "\0", 1,
	     300) != 1) {
		printk(KERN_ERR "isight firmware loading completion failed\n");
		ret = -ENODEV;
	}

out:
	kfree(buf);
	release_firmware(firmware);
	return ret;
}

static void isight_firmware_disconnect(struct usb_interface *intf)
{
}

static struct usb_driver isight_firmware_driver = {
	.name = "isight_firmware",
	.probe = isight_firmware_load,
	.disconnect = isight_firmware_disconnect,
	.id_table = id_table,
};

static int __init isight_firmware_init(void)
{
	return usb_register(&isight_firmware_driver);
}

static void __exit isight_firmware_exit(void)
{
	usb_deregister(&isight_firmware_driver);
}

module_init(isight_firmware_init);
module_exit(isight_firmware_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Matthew Garrett <mjg@redhat.com>");
