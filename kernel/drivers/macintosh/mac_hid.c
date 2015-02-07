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
 * drivers/macintosh/mac_hid.c
 *
 * HID support stuff for Macintosh computers.
 *
 * Copyright (C) 2000 Franz Sirl.
 *
 * This file will soon be removed in favor of an uinput userspace tool.
 */

#include <linux/init.h>
#include <linux/proc_fs.h>
#include <linux/sysctl.h>
#include <linux/input.h>
#include <linux/module.h>
#include <linux/kbd_kern.h>


static struct input_dev *emumousebtn;
static int emumousebtn_input_register(void);
static int mouse_emulate_buttons;
static int mouse_button2_keycode = KEY_RIGHTCTRL;	/* right control key */
static int mouse_button3_keycode = KEY_RIGHTALT;	/* right option key */
static int mouse_last_keycode;

#if defined(CONFIG_SYSCTL)
/* file(s) in /proc/sys/dev/mac_hid */
static ctl_table mac_hid_files[] = {
	{
		.ctl_name	= DEV_MAC_HID_MOUSE_BUTTON_EMULATION,
		.procname	= "mouse_button_emulation",
		.data		= &mouse_emulate_buttons,
		.maxlen		= sizeof(int),
		.mode		= 0644,
		.proc_handler	= &proc_dointvec,
	},
	{
		.ctl_name	= DEV_MAC_HID_MOUSE_BUTTON2_KEYCODE,
		.procname	= "mouse_button2_keycode",
		.data		= &mouse_button2_keycode,
		.maxlen		= sizeof(int),
		.mode		= 0644,
		.proc_handler	= &proc_dointvec,
	},
	{
		.ctl_name	= DEV_MAC_HID_MOUSE_BUTTON3_KEYCODE,
		.procname	= "mouse_button3_keycode",
		.data		= &mouse_button3_keycode,
		.maxlen		= sizeof(int),
		.mode		= 0644,
		.proc_handler	= &proc_dointvec,
	},
	{ .ctl_name = 0 }
};

/* dir in /proc/sys/dev */
static ctl_table mac_hid_dir[] = {
	{
		.ctl_name	= DEV_MAC_HID,
		.procname	= "mac_hid",
		.maxlen		= 0,
		.mode		= 0555,
		.child		= mac_hid_files,
	},
	{ .ctl_name = 0 }
};

/* /proc/sys/dev itself, in case that is not there yet */
static ctl_table mac_hid_root_dir[] = {
	{
		.ctl_name	= CTL_DEV,
		.procname	= "dev",
		.maxlen		= 0,
		.mode		= 0555,
		.child		= mac_hid_dir,
	},
	{ .ctl_name = 0 }
};

static struct ctl_table_header *mac_hid_sysctl_header;

#endif /* endif CONFIG_SYSCTL */

int mac_hid_mouse_emulate_buttons(int caller, unsigned int keycode, int down)
{
	switch (caller) {
	case 1:
		/* Called from keyboard.c */
		if (mouse_emulate_buttons
		    && (keycode == mouse_button2_keycode
			|| keycode == mouse_button3_keycode)) {
			if (mouse_emulate_buttons == 1) {
				input_report_key(emumousebtn,
						 keycode == mouse_button2_keycode ? BTN_MIDDLE : BTN_RIGHT,
						 down);
				input_sync(emumousebtn);
				return 1;
			}
			mouse_last_keycode = down ? keycode : 0;
		}
		break;
	}
	return 0;
}

static struct lock_class_key emumousebtn_event_class;
static struct lock_class_key emumousebtn_mutex_class;

static int emumousebtn_input_register(void)
{
	int ret;

	emumousebtn = input_allocate_device();
	if (!emumousebtn)
		return -ENOMEM;

	lockdep_set_class(&emumousebtn->event_lock, &emumousebtn_event_class);
	lockdep_set_class(&emumousebtn->mutex, &emumousebtn_mutex_class);

	emumousebtn->name = "Macintosh mouse button emulation";
	emumousebtn->id.bustype = BUS_ADB;
	emumousebtn->id.vendor = 0x0001;
	emumousebtn->id.product = 0x0001;
	emumousebtn->id.version = 0x0100;

	emumousebtn->evbit[0] = BIT_MASK(EV_KEY) | BIT_MASK(EV_REL);
	emumousebtn->keybit[BIT_WORD(BTN_MOUSE)] = BIT_MASK(BTN_LEFT) |
		BIT_MASK(BTN_MIDDLE) | BIT_MASK(BTN_RIGHT);
	emumousebtn->relbit[0] = BIT_MASK(REL_X) | BIT_MASK(REL_Y);

	ret = input_register_device(emumousebtn);
	if (ret)
		input_free_device(emumousebtn);

	return ret;
}

static int __init mac_hid_init(void)
{
	int err;

	err = emumousebtn_input_register();
	if (err)
		return err;

#if defined(CONFIG_SYSCTL)
	mac_hid_sysctl_header = register_sysctl_table(mac_hid_root_dir);
#endif /* CONFIG_SYSCTL */

	return 0;
}

device_initcall(mac_hid_init);
