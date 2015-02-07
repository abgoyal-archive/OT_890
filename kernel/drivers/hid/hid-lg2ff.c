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
 *  Force feedback support for Logitech Rumblepad 2
 *
 *  Copyright (c) 2008 Anssi Hannula <anssi.hannula@gmail.com>
 */

/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 */


#include <linux/input.h>
#include <linux/usb.h>
#include <linux/hid.h>

#include "usbhid/usbhid.h"
#include "hid-lg.h"

struct lg2ff_device {
	struct hid_report *report;
};

static int play_effect(struct input_dev *dev, void *data,
			 struct ff_effect *effect)
{
	struct hid_device *hid = input_get_drvdata(dev);
	struct lg2ff_device *lg2ff = data;
	int weak, strong;

	strong = effect->u.rumble.strong_magnitude;
	weak = effect->u.rumble.weak_magnitude;

	if (weak || strong) {
		weak = weak * 0xff / 0xffff;
		strong = strong * 0xff / 0xffff;

		lg2ff->report->field[0]->value[0] = 0x51;
		lg2ff->report->field[0]->value[2] = weak;
		lg2ff->report->field[0]->value[4] = strong;
	} else {
		lg2ff->report->field[0]->value[0] = 0xf3;
		lg2ff->report->field[0]->value[2] = 0x00;
		lg2ff->report->field[0]->value[4] = 0x00;
	}

	usbhid_submit_report(hid, lg2ff->report, USB_DIR_OUT);
	return 0;
}

int lg2ff_init(struct hid_device *hid)
{
	struct lg2ff_device *lg2ff;
	struct hid_report *report;
	struct hid_input *hidinput = list_entry(hid->inputs.next,
						struct hid_input, list);
	struct list_head *report_list =
			&hid->report_enum[HID_OUTPUT_REPORT].report_list;
	struct input_dev *dev = hidinput->input;
	int error;

	if (list_empty(report_list)) {
		dev_err(&hid->dev, "no output report found\n");
		return -ENODEV;
	}

	report = list_entry(report_list->next, struct hid_report, list);

	if (report->maxfield < 1) {
		dev_err(&hid->dev, "output report is empty\n");
		return -ENODEV;
	}
	if (report->field[0]->report_count < 7) {
		dev_err(&hid->dev, "not enough values in the field\n");
		return -ENODEV;
	}

	lg2ff = kmalloc(sizeof(struct lg2ff_device), GFP_KERNEL);
	if (!lg2ff)
		return -ENOMEM;

	set_bit(FF_RUMBLE, dev->ffbit);

	error = input_ff_create_memless(dev, lg2ff, play_effect);
	if (error) {
		kfree(lg2ff);
		return error;
	}

	lg2ff->report = report;
	report->field[0]->value[0] = 0xf3;
	report->field[0]->value[1] = 0x00;
	report->field[0]->value[2] = 0x00;
	report->field[0]->value[3] = 0x00;
	report->field[0]->value[4] = 0x00;
	report->field[0]->value[5] = 0x00;
	report->field[0]->value[6] = 0x00;

	usbhid_submit_report(hid, report, USB_DIR_OUT);

	dev_info(&hid->dev, "Force feedback for Logitech Rumblepad 2 by "
	       "Anssi Hannula <anssi.hannula@gmail.com>\n");

	return 0;
}
