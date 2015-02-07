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
 *  HID driver for some sunplus "special" devices
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
#include <linux/module.h>

#include "hid-ids.h"

static void sp_report_fixup(struct hid_device *hdev, __u8 *rdesc,
		unsigned int rsize)
{
	if (rsize >= 107 && rdesc[104] == 0x26 && rdesc[105] == 0x80 &&
			rdesc[106] == 0x03) {
		dev_info(&hdev->dev, "fixing up Sunplus Wireless Desktop "
				"report descriptor\n");
		rdesc[105] = rdesc[110] = 0x03;
		rdesc[106] = rdesc[111] = 0x21;
	}
}

#define sp_map_key_clear(c)	hid_map_usage_clear(hi, usage, bit, max, \
		EV_KEY, (c))
static int sp_input_mapping(struct hid_device *hdev, struct hid_input *hi,
		struct hid_field *field, struct hid_usage *usage,
		unsigned long **bit, int *max)
{
	if ((usage->hid & HID_USAGE_PAGE) != HID_UP_CONSUMER)
		return 0;

	switch (usage->hid & HID_USAGE) {
	case 0x2003: sp_map_key_clear(KEY_ZOOMIN);		break;
	case 0x2103: sp_map_key_clear(KEY_ZOOMOUT);	break;
	default:
		return 0;
	}
	return 1;
}

static const struct hid_device_id sp_devices[] = {
	{ HID_USB_DEVICE(USB_VENDOR_ID_SUNPLUS, USB_DEVICE_ID_SUNPLUS_WDESKTOP) },
	{ }
};
MODULE_DEVICE_TABLE(hid, sp_devices);

static struct hid_driver sp_driver = {
	.name = "sunplus",
	.id_table = sp_devices,
	.report_fixup = sp_report_fixup,
	.input_mapping = sp_input_mapping,
};

static int sp_init(void)
{
	return hid_register_driver(&sp_driver);
}

static void sp_exit(void)
{
	hid_unregister_driver(&sp_driver);
}

module_init(sp_init);
module_exit(sp_exit);
MODULE_LICENSE("GPL");

HID_COMPAT_LOAD_DRIVER(sunplus);
