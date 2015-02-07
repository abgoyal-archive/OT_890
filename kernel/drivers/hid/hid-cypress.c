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
 *  HID driver for some cypress "special" devices
 *
 *  Copyright (c) 1999 Andreas Gal
 *  Copyright (c) 2000-2005 Vojtech Pavlik <vojtech@suse.cz>
 *  Copyright (c) 2005 Michael Haboustak <mike-@cinci.rr.com> for Concept2, Inc
 *  Copyright (c) 2006-2007 Jiri Kosina
 *  Copyright (c) 2007 Paul Walmsley
 *  Copyright (c) 2008 Jiri Slaby
 */

/*
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation; either version 2 of the License, or (at your option)
 * any later version.
 */

#include <linux/device.h>
#include <linux/hid.h>
#include <linux/input.h>
#include <linux/module.h>

#include "hid-ids.h"

#define CP_RDESC_SWAPPED_MIN_MAX	0x01
#define CP_2WHEEL_MOUSE_HACK		0x02
#define CP_2WHEEL_MOUSE_HACK_ON		0x04

/*
 * Some USB barcode readers from cypress have usage min and usage max in
 * the wrong order
 */
static void cp_report_fixup(struct hid_device *hdev, __u8 *rdesc,
		unsigned int rsize)
{
	unsigned long quirks = (unsigned long)hid_get_drvdata(hdev);
	unsigned int i;

	if (!(quirks & CP_RDESC_SWAPPED_MIN_MAX))
		return;

	for (i = 0; i < rsize - 4; i++)
		if (rdesc[i] == 0x29 && rdesc[i + 2] == 0x19) {
			__u8 tmp;

			rdesc[i] = 0x19;
			rdesc[i + 2] = 0x29;
			tmp = rdesc[i + 3];
			rdesc[i + 3] = rdesc[i + 1];
			rdesc[i + 1] = tmp;
		}
}

static int cp_input_mapped(struct hid_device *hdev, struct hid_input *hi,
		struct hid_field *field, struct hid_usage *usage,
		unsigned long **bit, int *max)
{
	unsigned long quirks = (unsigned long)hid_get_drvdata(hdev);

	if (!(quirks & CP_2WHEEL_MOUSE_HACK))
		return 0;

	if (usage->type == EV_REL && usage->code == REL_WHEEL)
		set_bit(REL_HWHEEL, *bit);
	if (usage->hid == 0x00090005)
		return -1;

	return 0;
}

static int cp_event(struct hid_device *hdev, struct hid_field *field,
		struct hid_usage *usage, __s32 value)
{
	unsigned long quirks = (unsigned long)hid_get_drvdata(hdev);

	if (!(hdev->claimed & HID_CLAIMED_INPUT) || !field->hidinput ||
			!usage->type || !(quirks & CP_2WHEEL_MOUSE_HACK))
		return 0;

	if (usage->hid == 0x00090005) {
		if (value)
			quirks |=  CP_2WHEEL_MOUSE_HACK_ON;
		else
			quirks &= ~CP_2WHEEL_MOUSE_HACK_ON;
		hid_set_drvdata(hdev, (void *)quirks);
		return 1;
	}

	if (usage->code == REL_WHEEL && (quirks & CP_2WHEEL_MOUSE_HACK_ON)) {
		struct input_dev *input = field->hidinput->input;

		input_event(input, usage->type, REL_HWHEEL, value);
		return 1;
	}

	return 0;
}

static int cp_probe(struct hid_device *hdev, const struct hid_device_id *id)
{
	unsigned long quirks = id->driver_data;
	int ret;

	hid_set_drvdata(hdev, (void *)quirks);

	ret = hid_parse(hdev);
	if (ret) {
		dev_err(&hdev->dev, "parse failed\n");
		goto err_free;
	}

	ret = hid_hw_start(hdev, HID_CONNECT_DEFAULT);
	if (ret) {
		dev_err(&hdev->dev, "hw start failed\n");
		goto err_free;
	}

	return 0;
err_free:
	return ret;
}

static const struct hid_device_id cp_devices[] = {
	{ HID_USB_DEVICE(USB_VENDOR_ID_CYPRESS, USB_DEVICE_ID_CYPRESS_BARCODE_1),
		.driver_data = CP_RDESC_SWAPPED_MIN_MAX },
	{ HID_USB_DEVICE(USB_VENDOR_ID_CYPRESS, USB_DEVICE_ID_CYPRESS_BARCODE_2),
		.driver_data = CP_RDESC_SWAPPED_MIN_MAX },
	{ HID_USB_DEVICE(USB_VENDOR_ID_CYPRESS, USB_DEVICE_ID_CYPRESS_MOUSE),
		.driver_data = CP_2WHEEL_MOUSE_HACK },
	{ }
};
MODULE_DEVICE_TABLE(hid, cp_devices);

static struct hid_driver cp_driver = {
	.name = "cypress",
	.id_table = cp_devices,
	.report_fixup = cp_report_fixup,
	.input_mapped = cp_input_mapped,
	.event = cp_event,
	.probe = cp_probe,
};

static int cp_init(void)
{
	return hid_register_driver(&cp_driver);
}

static void cp_exit(void)
{
	hid_unregister_driver(&cp_driver);
}

module_init(cp_init);
module_exit(cp_exit);
MODULE_LICENSE("GPL");

HID_COMPAT_LOAD_DRIVER(cypress);
