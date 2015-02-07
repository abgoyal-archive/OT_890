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
 * All the USB notify logic
 *
 * (C) Copyright 2005 Greg Kroah-Hartman <gregkh@suse.de>
 *
 * notifier functions originally based on those in kernel/sys.c
 * but fixed up to not be so broken.
 *
 */


#include <linux/kernel.h>
#include <linux/notifier.h>
#include <linux/usb.h>
#include <linux/mutex.h>
#include "usb.h"

static BLOCKING_NOTIFIER_HEAD(usb_notifier_list);

/**
 * usb_register_notify - register a notifier callback whenever a usb change happens
 * @nb: pointer to the notifier block for the callback events.
 *
 * These changes are either USB devices or busses being added or removed.
 */
void usb_register_notify(struct notifier_block *nb)
{
	blocking_notifier_chain_register(&usb_notifier_list, nb);
}
EXPORT_SYMBOL_GPL(usb_register_notify);

/**
 * usb_unregister_notify - unregister a notifier callback
 * @nb: pointer to the notifier block for the callback events.
 *
 * usb_register_notify() must have been previously called for this function
 * to work properly.
 */
void usb_unregister_notify(struct notifier_block *nb)
{
	blocking_notifier_chain_unregister(&usb_notifier_list, nb);
}
EXPORT_SYMBOL_GPL(usb_unregister_notify);


void usb_notify_add_device(struct usb_device *udev)
{
	blocking_notifier_call_chain(&usb_notifier_list, USB_DEVICE_ADD, udev);
}

void usb_notify_remove_device(struct usb_device *udev)
{
	/* Protect against simultaneous usbfs open */
	mutex_lock(&usbfs_mutex);
	blocking_notifier_call_chain(&usb_notifier_list,
			USB_DEVICE_REMOVE, udev);
	mutex_unlock(&usbfs_mutex);
}

void usb_notify_add_bus(struct usb_bus *ubus)
{
	blocking_notifier_call_chain(&usb_notifier_list, USB_BUS_ADD, ubus);
}

void usb_notify_remove_bus(struct usb_bus *ubus)
{
	blocking_notifier_call_chain(&usb_notifier_list, USB_BUS_REMOVE, ubus);
}
