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
 *  HID driver for some a4tech "special" devices
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
#include <linux/input.h>
#include <linux/hid.h>
#include <linux/module.h>

#include "hid-ids.h"

#define A4_2WHEEL_MOUSE_HACK_7	0x01
#define A4_2WHEEL_MOUSE_HACK_B8	0x02

struct a4tech_sc {
	unsigned long quirks;
	unsigned int hw_wheel;
	__s32 delayed_value;
};

static int a4_input_mapped(struct hid_device *hdev, struct hid_input *hi,
		struct hid_field *field, struct hid_usage *usage,
		unsigned long **bit, int *max)
{
	struct a4tech_sc *a4 = hid_get_drvdata(hdev);

	if (usage->type == EV_REL && usage->code == REL_WHEEL)
		set_bit(REL_HWHEEL, *bit);

	if ((a4->quirks & A4_2WHEEL_MOUSE_HACK_7) && usage->hid == 0x00090007)
		return -1;

	return 0;
}

static int a4_event(struct hid_device *hdev, struct hid_field *field,
		struct hid_usage *usage, __s32 value)
{
	struct a4tech_sc *a4 = hid_get_drvdata(hdev);
	struct input_dev *input;

	if (!(hdev->claimed & HID_CLAIMED_INPUT) || !field->hidinput ||
			!usage->type)
		return 0;

	input = field->hidinput->input;

	if (a4->quirks & A4_2WHEEL_MOUSE_HACK_B8) {
		if (usage->type == EV_REL && usage->code == REL_WHEEL) {
			a4->delayed_value = value;
			return 1;
		}

		if (usage->hid == 0x000100b8) {
			input_event(input, EV_REL, value ? REL_HWHEEL :
					REL_WHEEL, a4->delayed_value);
			return 1;
		}
	}

	if ((a4->quirks & A4_2WHEEL_MOUSE_HACK_7) && usage->hid == 0x00090007) {
		a4->hw_wheel = !!value;
		return 1;
	}

	if (usage->code == REL_WHEEL && a4->hw_wheel) {
		input_event(input, usage->type, REL_HWHEEL, value);
		return 1;
	}

	return 0;
}

static int a4_probe(struct hid_device *hdev, const struct hid_device_id *id)
{
	struct a4tech_sc *a4;
	int ret;

	a4 = kzalloc(sizeof(*a4), GFP_KERNEL);
	if (a4 == NULL) {
		dev_err(&hdev->dev, "can't alloc device descriptor\n");
		ret = -ENOMEM;
		goto err_free;
	}

	a4->quirks = id->driver_data;

	hid_set_drvdata(hdev, a4);

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
	kfree(a4);
	return ret;
}

static void a4_remove(struct hid_device *hdev)
{
	struct a4tech_sc *a4 = hid_get_drvdata(hdev);

	hid_hw_stop(hdev);
	kfree(a4);
}

static const struct hid_device_id a4_devices[] = {
	{ HID_USB_DEVICE(USB_VENDOR_ID_A4TECH, USB_DEVICE_ID_A4TECH_WCP32PU),
		.driver_data = A4_2WHEEL_MOUSE_HACK_7 },
	{ HID_USB_DEVICE(USB_VENDOR_ID_A4TECH, USB_DEVICE_ID_A4TECH_X5_005D),
		.driver_data = A4_2WHEEL_MOUSE_HACK_B8 },
	{ }
};
MODULE_DEVICE_TABLE(hid, a4_devices);

static struct hid_driver a4_driver = {
	.name = "a4tech",
	.id_table = a4_devices,
	.input_mapped = a4_input_mapped,
	.event = a4_event,
	.probe = a4_probe,
	.remove = a4_remove,
};

static int a4_init(void)
{
	return hid_register_driver(&a4_driver);
}

static void a4_exit(void)
{
	hid_unregister_driver(&a4_driver);
}

module_init(a4_init);
module_exit(a4_exit);
MODULE_LICENSE("GPL");

HID_COMPAT_LOAD_DRIVER(a4tech);
