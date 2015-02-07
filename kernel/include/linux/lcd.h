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
 * LCD Lowlevel Control Abstraction
 *
 * Copyright (C) 2003,2004 Hewlett-Packard Company
 *
 */

#ifndef _LINUX_LCD_H
#define _LINUX_LCD_H

#include <linux/device.h>
#include <linux/mutex.h>
#include <linux/notifier.h>
#include <linux/fb.h>

/* Notes on locking:
 *
 * lcd_device->ops_lock is an internal backlight lock protecting the ops
 * field and no code outside the core should need to touch it.
 *
 * Access to set_power() is serialised by the update_lock mutex since
 * most drivers seem to need this and historically get it wrong.
 *
 * Most drivers don't need locking on their get_power() method.
 * If yours does, you need to implement it in the driver. You can use the
 * update_lock mutex if appropriate.
 *
 * Any other use of the locks below is probably wrong.
 */

struct lcd_device;
struct fb_info;

struct lcd_properties {
	/* The maximum value for contrast (read-only) */
	int max_contrast;
};

struct lcd_ops {
	/* Get the LCD panel power status (0: full on, 1..3: controller
	   power on, flat panel power off, 4: full off), see FB_BLANK_XXX */
	int (*get_power)(struct lcd_device *);
	/* Enable or disable power to the LCD (0: on; 4: off, see FB_BLANK_XXX) */
	int (*set_power)(struct lcd_device *, int power);
	/* Get the current contrast setting (0-max_contrast) */
	int (*get_contrast)(struct lcd_device *);
	/* Set LCD panel contrast */
        int (*set_contrast)(struct lcd_device *, int contrast);
	/* Set LCD panel mode (resolutions ...) */
	int (*set_mode)(struct lcd_device *, struct fb_videomode *);
	/* Check if given framebuffer device is the one LCD is bound to;
	   return 0 if not, !=0 if it is. If NULL, lcd always matches the fb. */
	int (*check_fb)(struct lcd_device *, struct fb_info *);
};

struct lcd_device {
	struct lcd_properties props;
	/* This protects the 'ops' field. If 'ops' is NULL, the driver that
	   registered this device has been unloaded, and if class_get_devdata()
	   points to something in the body of that driver, it is also invalid. */
	struct mutex ops_lock;
	/* If this is NULL, the backing module is unloaded */
	struct lcd_ops *ops;
	/* Serialise access to set_power method */
	struct mutex update_lock;
	/* The framebuffer notifier block */
	struct notifier_block fb_notif;

	struct device dev;
};

static inline void lcd_set_power(struct lcd_device *ld, int power)
{
	mutex_lock(&ld->update_lock);
	if (ld->ops && ld->ops->set_power)
		ld->ops->set_power(ld, power);
	mutex_unlock(&ld->update_lock);
}

extern struct lcd_device *lcd_device_register(const char *name,
	struct device *parent, void *devdata, struct lcd_ops *ops);
extern void lcd_device_unregister(struct lcd_device *ld);

#define to_lcd_device(obj) container_of(obj, struct lcd_device, dev)

static inline void * lcd_get_data(struct lcd_device *ld_dev)
{
	return dev_get_drvdata(&ld_dev->dev);
}


#endif
