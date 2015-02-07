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
 *  thermal.h  ($Revision$)
 *
 *  Copyright (C) 2008  Intel Corp
 *  Copyright (C) 2008  Zhang Rui <rui.zhang@intel.com>
 *  Copyright (C) 2008  Sujith Thomas <sujith.thomas@intel.com>
 *
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; version 2 of the License.
 *
 *  This program is distributed in the hope that it will be useful, but
 *  WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program; if not, write to the Free Software Foundation, Inc.,
 *  59 Temple Place, Suite 330, Boston, MA 02111-1307 USA.
 *
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */

#ifndef __THERMAL_H__
#define __THERMAL_H__

#include <linux/idr.h>
#include <linux/device.h>

struct thermal_zone_device;
struct thermal_cooling_device;

struct thermal_zone_device_ops {
	int (*bind) (struct thermal_zone_device *,
		     struct thermal_cooling_device *);
	int (*unbind) (struct thermal_zone_device *,
		       struct thermal_cooling_device *);
	int (*get_temp) (struct thermal_zone_device *, char *);
	int (*get_mode) (struct thermal_zone_device *, char *);
	int (*set_mode) (struct thermal_zone_device *, const char *);
	int (*get_trip_type) (struct thermal_zone_device *, int, char *);
	int (*get_trip_temp) (struct thermal_zone_device *, int, char *);
	int (*get_crit_temp) (struct thermal_zone_device *, unsigned long *);
};

struct thermal_cooling_device_ops {
	int (*get_max_state) (struct thermal_cooling_device *, char *);
	int (*get_cur_state) (struct thermal_cooling_device *, char *);
	int (*set_cur_state) (struct thermal_cooling_device *, unsigned int);
};

#define THERMAL_TRIPS_NONE -1
#define THERMAL_MAX_TRIPS 12
#define THERMAL_NAME_LENGTH 20
struct thermal_cooling_device {
	int id;
	char type[THERMAL_NAME_LENGTH];
	struct device device;
	void *devdata;
	struct thermal_cooling_device_ops *ops;
	struct list_head node;
};

#define KELVIN_TO_CELSIUS(t)	(long)(((long)t-2732 >= 0) ?	\
				((long)t-2732+5)/10 : ((long)t-2732-5)/10)
#define CELSIUS_TO_KELVIN(t)	((t)*10+2732)

#if defined(CONFIG_THERMAL_HWMON)
/* thermal zone devices with the same type share one hwmon device */
struct thermal_hwmon_device {
	char type[THERMAL_NAME_LENGTH];
	struct device *device;
	int count;
	struct list_head tz_list;
	struct list_head node;
};

struct thermal_hwmon_attr {
	struct device_attribute attr;
	char name[16];
};
#endif

struct thermal_zone_device {
	int id;
	char type[THERMAL_NAME_LENGTH];
	struct device device;
	void *devdata;
	int trips;
	struct thermal_zone_device_ops *ops;
	struct list_head cooling_devices;
	struct idr idr;
	struct mutex lock;	/* protect cooling devices list */
	struct list_head node;
#if defined(CONFIG_THERMAL_HWMON)
	struct list_head hwmon_node;
	struct thermal_hwmon_device *hwmon;
	struct thermal_hwmon_attr temp_input;	/* hwmon sys attr */
	struct thermal_hwmon_attr temp_crit;	/* hwmon sys attr */
#endif
};

struct thermal_zone_device *thermal_zone_device_register(char *, int, void *,
							 struct
							 thermal_zone_device_ops
							 *);
void thermal_zone_device_unregister(struct thermal_zone_device *);

int thermal_zone_bind_cooling_device(struct thermal_zone_device *, int,
				     struct thermal_cooling_device *);
int thermal_zone_unbind_cooling_device(struct thermal_zone_device *, int,
				       struct thermal_cooling_device *);
struct thermal_cooling_device *thermal_cooling_device_register(char *, void *,
							       struct
							       thermal_cooling_device_ops
							       *);
void thermal_cooling_device_unregister(struct thermal_cooling_device *);

#endif /* __THERMAL_H__ */
