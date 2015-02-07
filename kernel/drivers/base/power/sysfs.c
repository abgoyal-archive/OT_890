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
 * drivers/base/power/sysfs.c - sysfs entries for device PM
 */

#include <linux/device.h>
#include <linux/string.h>
#include "power.h"

/*
 *	wakeup - Report/change current wakeup option for device
 *
 *	Some devices support "wakeup" events, which are hardware signals
 *	used to activate devices from suspended or low power states.  Such
 *	devices have one of three values for the sysfs power/wakeup file:
 *
 *	 + "enabled\n" to issue the events;
 *	 + "disabled\n" not to do so; or
 *	 + "\n" for temporary or permanent inability to issue wakeup.
 *
 *	(For example, unconfigured USB devices can't issue wakeups.)
 *
 *	Familiar examples of devices that can issue wakeup events include
 *	keyboards and mice (both PS2 and USB styles), power buttons, modems,
 *	"Wake-On-LAN" Ethernet links, GPIO lines, and more.  Some events
 *	will wake the entire system from a suspend state; others may just
 *	wake up the device (if the system as a whole is already active).
 *	Some wakeup events use normal IRQ lines; other use special out
 *	of band signaling.
 *
 *	It is the responsibility of device drivers to enable (or disable)
 *	wakeup signaling as part of changing device power states, respecting
 *	the policy choices provided through the driver model.
 *
 *	Devices may not be able to generate wakeup events from all power
 *	states.  Also, the events may be ignored in some configurations;
 *	for example, they might need help from other devices that aren't
 *	active, or which may have wakeup disabled.  Some drivers rely on
 *	wakeup events internally (unless they are disabled), keeping
 *	their hardware in low power modes whenever they're unused.  This
 *	saves runtime power, without requiring system-wide sleep states.
 */

static const char enabled[] = "enabled";
static const char disabled[] = "disabled";

static ssize_t
wake_show(struct device * dev, struct device_attribute *attr, char * buf)
{
	return sprintf(buf, "%s\n", device_can_wakeup(dev)
		? (device_may_wakeup(dev) ? enabled : disabled)
		: "");
}

static ssize_t
wake_store(struct device * dev, struct device_attribute *attr,
	const char * buf, size_t n)
{
	char *cp;
	int len = n;

	if (!device_can_wakeup(dev))
		return -EINVAL;

	cp = memchr(buf, '\n', n);
	if (cp)
		len = cp - buf;
	if (len == sizeof enabled - 1
			&& strncmp(buf, enabled, sizeof enabled - 1) == 0)
		device_set_wakeup_enable(dev, 1);
	else if (len == sizeof disabled - 1
			&& strncmp(buf, disabled, sizeof disabled - 1) == 0)
		device_set_wakeup_enable(dev, 0);
	else
		return -EINVAL;
	return n;
}

static DEVICE_ATTR(wakeup, 0644, wake_show, wake_store);


static struct attribute * power_attrs[] = {
	&dev_attr_wakeup.attr,
	NULL,
};
static struct attribute_group pm_attr_group = {
	.name	= "power",
	.attrs	= power_attrs,
};

int dpm_sysfs_add(struct device * dev)
{
	return sysfs_create_group(&dev->kobj, &pm_attr_group);
}

void dpm_sysfs_remove(struct device * dev)
{
	sysfs_remove_group(&dev->kobj, &pm_attr_group);
}
