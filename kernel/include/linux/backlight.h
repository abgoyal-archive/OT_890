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
 * Backlight Lowlevel Control Abstraction
 *
 * Copyright (C) 2003,2004 Hewlett-Packard Company
 *
 */

#ifndef _LINUX_BACKLIGHT_H
#define _LINUX_BACKLIGHT_H

#include <linux/device.h>
#include <linux/mutex.h>
#include <linux/notifier.h>

/* Notes on locking:
 *
 * backlight_device->ops_lock is an internal backlight lock protecting the
 * ops pointer and no code outside the core should need to touch it.
 *
 * Access to update_status() is serialised by the update_lock mutex since
 * most drivers seem to need this and historically get it wrong.
 *
 * Most drivers don't need locking on their get_brightness() method.
 * If yours does, you need to implement it in the driver. You can use the
 * update_lock mutex if appropriate.
 *
 * Any other use of the locks below is probably wrong.
 */

struct backlight_device;
struct fb_info;

struct backlight_ops {
	unsigned int options;

#define BL_CORE_SUSPENDRESUME	(1 << 0)

	/* Notify the backlight driver some property has changed */
	int (*update_status)(struct backlight_device *);
	/* Return the current backlight brightness (accounting for power,
	   fb_blank etc.) */
	int (*get_brightness)(struct backlight_device *);
	/* Check if given framebuffer device is the one bound to this backlight;
	   return 0 if not, !=0 if it is. If NULL, backlight always matches the fb. */
	int (*check_fb)(struct fb_info *);
};

/* This structure defines all the properties of a backlight */
struct backlight_properties {
	/* Current User requested brightness (0 - max_brightness) */
	int brightness;
	/* Maximal value for brightness (read-only) */
	int max_brightness;
	/* Current FB Power mode (0: full on, 1..3: power saving
	   modes; 4: full off), see FB_BLANK_XXX */
	int power;
	/* FB Blanking active? (values as for power) */
	/* Due to be removed, please use (state & BL_CORE_FBBLANK) */
	int fb_blank;
	/* Flags used to signal drivers of state changes */
	/* Upper 4 bits are reserved for driver internal use */
	unsigned int state;

#define BL_CORE_SUSPENDED	(1 << 0)	/* backlight is suspended */
#define BL_CORE_FBBLANK		(1 << 1)	/* backlight is under an fb blank event */
#define BL_CORE_DRIVER4		(1 << 28)	/* reserved for driver specific use */
#define BL_CORE_DRIVER3		(1 << 29)	/* reserved for driver specific use */
#define BL_CORE_DRIVER2		(1 << 30)	/* reserved for driver specific use */
#define BL_CORE_DRIVER1		(1 << 31)	/* reserved for driver specific use */

};

struct backlight_device {
	/* Backlight properties */
	struct backlight_properties props;

	/* Serialise access to update_status method */
	struct mutex update_lock;

	/* This protects the 'ops' field. If 'ops' is NULL, the driver that
	   registered this device has been unloaded, and if class_get_devdata()
	   points to something in the body of that driver, it is also invalid. */
	struct mutex ops_lock;
	struct backlight_ops *ops;

	/* The framebuffer notifier block */
	struct notifier_block fb_notif;

	struct device dev;
};

static inline void backlight_update_status(struct backlight_device *bd)
{
	mutex_lock(&bd->update_lock);
	if (bd->ops && bd->ops->update_status)
		bd->ops->update_status(bd);
	mutex_unlock(&bd->update_lock);
}

extern struct backlight_device *backlight_device_register(const char *name,
	struct device *dev, void *devdata, struct backlight_ops *ops);
extern void backlight_device_unregister(struct backlight_device *bd);

#define to_backlight_device(obj) container_of(obj, struct backlight_device, dev)

static inline void * bl_get_data(struct backlight_device *bl_dev)
{
	return dev_get_drvdata(&bl_dev->dev);
}

struct generic_bl_info {
	const char *name;
	int max_intensity;
	int default_intensity;
	int limit_mask;
	void (*set_bl_intensity)(int intensity);
	void (*kick_battery)(void);
};

#endif
