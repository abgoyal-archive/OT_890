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
 *  Input Power Event -> APM Bridge
 *
 *  Copyright (c) 2007 Richard Purdie
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License version 2 as
 *  published by the Free Software Foundation.
 *
 */

#include <linux/module.h>
#include <linux/input.h>
#include <linux/slab.h>
#include <linux/init.h>
#include <linux/tty.h>
#include <linux/delay.h>
#include <linux/pm.h>
#include <linux/apm-emulation.h>

static void system_power_event(unsigned int keycode)
{
	switch (keycode) {
	case KEY_SUSPEND:
		apm_queue_event(APM_USER_SUSPEND);

		printk(KERN_INFO "apm-power: Requesting system suspend...\n");
		break;
	default:
		break;
	}
}

static void apmpower_event(struct input_handle *handle, unsigned int type,
		        unsigned int code, int value)
{
	/* only react on key down events */
	if (value != 1)
		return;

	switch (type) {
	case EV_PWR:
		system_power_event(code);
		break;

	default:
		break;
	}
}

static int apmpower_connect(struct input_handler *handler,
					  struct input_dev *dev,
					  const struct input_device_id *id)
{
	struct input_handle *handle;
	int error;

	handle = kzalloc(sizeof(struct input_handle), GFP_KERNEL);
	if (!handle)
		return -ENOMEM;

	handle->dev = dev;
	handle->handler = handler;
	handle->name = "apm-power";

	error = input_register_handle(handle);
	if (error) {
		printk(KERN_ERR
			"apm-power: Failed to register input power handler, "
			"error %d\n", error);
		kfree(handle);
		return error;
	}

	error = input_open_device(handle);
	if (error) {
		printk(KERN_ERR
			"apm-power: Failed to open input power device, "
			"error %d\n", error);
		input_unregister_handle(handle);
		kfree(handle);
		return error;
	}

	return 0;
}

static void apmpower_disconnect(struct input_handle *handle)
{
	input_close_device(handle);
	input_unregister_handle(handle);
	kfree(handle);
}

static const struct input_device_id apmpower_ids[] = {
	{
		.flags = INPUT_DEVICE_ID_MATCH_EVBIT,
		.evbit = { BIT_MASK(EV_PWR) },
	},
	{ },
};

MODULE_DEVICE_TABLE(input, apmpower_ids);

static struct input_handler apmpower_handler = {
	.event =	apmpower_event,
	.connect =	apmpower_connect,
	.disconnect =	apmpower_disconnect,
	.name =		"apm-power",
	.id_table =	apmpower_ids,
};

static int __init apmpower_init(void)
{
	return input_register_handler(&apmpower_handler);
}

static void __exit apmpower_exit(void)
{
	input_unregister_handler(&apmpower_handler);
}

module_init(apmpower_init);
module_exit(apmpower_exit);

MODULE_AUTHOR("Richard Purdie <rpurdie@rpsys.net>");
MODULE_DESCRIPTION("Input Power Event -> APM Bridge");
MODULE_LICENSE("GPL");
