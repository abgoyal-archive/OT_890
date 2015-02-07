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
 * platform_device.h - generic, centralized driver model
 *
 * Copyright (c) 2001-2003 Patrick Mochel <mochel@osdl.org>
 *
 * This file is released under the GPLv2
 *
 * See Documentation/driver-model/ for more information.
 */

#ifndef _PLATFORM_DEVICE_H_
#define _PLATFORM_DEVICE_H_

#include <linux/device.h>

struct platform_device {
	const char	* name;
	int		id;
	struct device	dev;
	u32		num_resources;
	struct resource	* resource;
};

#define to_platform_device(x) container_of((x), struct platform_device, dev)

extern int platform_device_register(struct platform_device *);
extern void platform_device_unregister(struct platform_device *);

extern struct bus_type platform_bus_type;
extern struct device platform_bus;

extern struct resource *platform_get_resource(struct platform_device *, unsigned int, unsigned int);
extern int platform_get_irq(struct platform_device *, unsigned int);
extern struct resource *platform_get_resource_byname(struct platform_device *, unsigned int, char *);
extern int platform_get_irq_byname(struct platform_device *, char *);
extern int platform_add_devices(struct platform_device **, int);

extern struct platform_device *platform_device_register_simple(const char *, int id,
					struct resource *, unsigned int);
extern struct platform_device *platform_device_register_data(struct device *,
		const char *, int, const void *, size_t);

extern struct platform_device *platform_device_alloc(const char *name, int id);
extern int platform_device_add_resources(struct platform_device *pdev, struct resource *res, unsigned int num);
extern int platform_device_add_data(struct platform_device *pdev, const void *data, size_t size);
extern int platform_device_add(struct platform_device *pdev);
extern void platform_device_del(struct platform_device *pdev);
extern void platform_device_put(struct platform_device *pdev);

struct platform_driver {
	int (*probe)(struct platform_device *);
	int (*remove)(struct platform_device *);
	void (*shutdown)(struct platform_device *);
	int (*suspend)(struct platform_device *, pm_message_t state);
	int (*suspend_late)(struct platform_device *, pm_message_t state);
	int (*resume_early)(struct platform_device *);
	int (*resume)(struct platform_device *);
	struct device_driver driver;
};

extern int platform_driver_register(struct platform_driver *);
extern void platform_driver_unregister(struct platform_driver *);

/* non-hotpluggable platform devices may use this so that probe() and
 * its support may live in __init sections, conserving runtime memory.
 */
extern int platform_driver_probe(struct platform_driver *driver,
		int (*probe)(struct platform_device *));

#define platform_get_drvdata(_dev)	dev_get_drvdata(&(_dev)->dev)
#define platform_set_drvdata(_dev,data)	dev_set_drvdata(&(_dev)->dev, (data))

#endif /* _PLATFORM_DEVICE_H_ */
