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
 *  HID driver for TopSeed Cyberlink remote
 *
 *  Copyright (c) 2008 Lev Babiev
 *  based on hid-cherry driver
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

#define ts_map_key_clear(c)	hid_map_usage_clear(hi, usage, bit, max, \
					EV_KEY, (c))
static int ts_input_mapping(struct hid_device *hdev, struct hid_input *hi,
		struct hid_field *field, struct hid_usage *usage,
		unsigned long **bit, int *max)
{
	if ((usage->hid & HID_USAGE_PAGE) != 0x0ffbc0000)
		return 0;

	switch (usage->hid & HID_USAGE) {
        case 0x00d: ts_map_key_clear(KEY_HOME);           break;
        case 0x024: ts_map_key_clear(KEY_MENU);           break;
        case 0x025: ts_map_key_clear(KEY_TV);             break;
        case 0x048: ts_map_key_clear(KEY_RED);            break;
        case 0x047: ts_map_key_clear(KEY_GREEN);          break;
        case 0x049: ts_map_key_clear(KEY_YELLOW);         break;
        case 0x04a: ts_map_key_clear(KEY_BLUE);           break;
        case 0x04b: ts_map_key_clear(KEY_ANGLE);          break;
        case 0x04c: ts_map_key_clear(KEY_LANGUAGE);       break;
        case 0x04d: ts_map_key_clear(KEY_SUBTITLE);       break;
        case 0x031: ts_map_key_clear(KEY_AUDIO);          break;
        case 0x032: ts_map_key_clear(KEY_TEXT);           break;
        case 0x033: ts_map_key_clear(KEY_CHANNEL);        break;
	default:
		return 0;
	}

	return 1;
}

static const struct hid_device_id ts_devices[] = {
	{ HID_USB_DEVICE(USB_VENDOR_ID_TOPSEED, USB_DEVICE_ID_TOPSEED_CYBERLINK) },
	{ }
};
MODULE_DEVICE_TABLE(hid, ts_devices);

static struct hid_driver ts_driver = {
	.name = "topseed",
	.id_table = ts_devices,
	.input_mapping = ts_input_mapping,
};

static int ts_init(void)
{
	return hid_register_driver(&ts_driver);
}

static void ts_exit(void)
{
	hid_unregister_driver(&ts_driver);
}

module_init(ts_init);
module_exit(ts_exit);
MODULE_LICENSE("GPL");

HID_COMPAT_LOAD_DRIVER(topseed);
