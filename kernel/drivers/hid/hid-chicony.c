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
 *  HID driver for some chicony "special" devices
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

#define ch_map_key_clear(c)	hid_map_usage_clear(hi, usage, bit, max, \
					EV_KEY, (c))
static int ch_input_mapping(struct hid_device *hdev, struct hid_input *hi,
		struct hid_field *field, struct hid_usage *usage,
		unsigned long **bit, int *max)
{
	if ((usage->hid & HID_USAGE_PAGE) != HID_UP_MSVENDOR)
		return 0;

	set_bit(EV_REP, hi->input->evbit);
	switch (usage->hid & HID_USAGE) {
	case 0xff01: ch_map_key_clear(BTN_1);	break;
	case 0xff02: ch_map_key_clear(BTN_2);	break;
	case 0xff03: ch_map_key_clear(BTN_3);	break;
	case 0xff04: ch_map_key_clear(BTN_4);	break;
	case 0xff05: ch_map_key_clear(BTN_5);	break;
	case 0xff06: ch_map_key_clear(BTN_6);	break;
	case 0xff07: ch_map_key_clear(BTN_7);	break;
	case 0xff08: ch_map_key_clear(BTN_8);	break;
	case 0xff09: ch_map_key_clear(BTN_9);	break;
	case 0xff0a: ch_map_key_clear(BTN_A);	break;
	case 0xff0b: ch_map_key_clear(BTN_B);	break;
	default:
		return 0;
	}
	return 1;
}

static const struct hid_device_id ch_devices[] = {
	{ HID_USB_DEVICE(USB_VENDOR_ID_CHICONY, USB_DEVICE_ID_CHICONY_TACTICAL_PAD) },
	{ }
};
MODULE_DEVICE_TABLE(hid, ch_devices);

static struct hid_driver ch_driver = {
	.name = "chicony",
	.id_table = ch_devices,
	.input_mapping = ch_input_mapping,
};

static int ch_init(void)
{
	return hid_register_driver(&ch_driver);
}

static void ch_exit(void)
{
	hid_unregister_driver(&ch_driver);
}

module_init(ch_init);
module_exit(ch_exit);
MODULE_LICENSE("GPL");

HID_COMPAT_LOAD_DRIVER(chicony);
