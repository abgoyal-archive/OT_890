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
 * Linux driver model AC97 bus interface
 *
 * Author:	Nicolas Pitre
 * Created:	Jan 14, 2005
 * Copyright:	(C) MontaVista Software Inc.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */

#include <linux/module.h>
#include <linux/init.h>
#include <linux/device.h>
#include <linux/string.h>
#include <sound/ac97_codec.h>

/*
 * Let drivers decide whether they want to support given codec from their
 * probe method.  Drivers have direct access to the struct snd_ac97 structure and may
 * decide based on the id field amongst other things.
 */
static int ac97_bus_match(struct device *dev, struct device_driver *drv)
{
	return 1;
}

#ifdef CONFIG_PM
static int ac97_bus_suspend(struct device *dev, pm_message_t state)
{
	int ret = 0;

	if (dev->driver && dev->driver->suspend)
		ret = dev->driver->suspend(dev, state);

	return ret;
}

static int ac97_bus_resume(struct device *dev)
{
	int ret = 0;

	if (dev->driver && dev->driver->resume)
		ret = dev->driver->resume(dev);

	return ret;
}
#endif /* CONFIG_PM */

struct bus_type ac97_bus_type = {
	.name		= "ac97",
	.match		= ac97_bus_match,
#ifdef CONFIG_PM
	.suspend	= ac97_bus_suspend,
	.resume		= ac97_bus_resume,
#endif /* CONFIG_PM */
};

static int __init ac97_bus_init(void)
{
	return bus_register(&ac97_bus_type);
}

subsys_initcall(ac97_bus_init);

static void __exit ac97_bus_exit(void)
{
	bus_unregister(&ac97_bus_type);
}

module_exit(ac97_bus_exit);

EXPORT_SYMBOL(ac97_bus_type);

MODULE_LICENSE("GPL");
