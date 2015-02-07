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
 * include/linux/superhyway.h
 *
 * SuperHyway Bus definitions
 *
 * Copyright (C) 2004, 2005  Paul Mundt <lethal@linux-sh.org>
 *
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 */
#ifndef __LINUX_SUPERHYWAY_H
#define __LINUX_SUPERHYWAY_H

#include <linux/device.h>

/*
 * SuperHyway IDs
 */
#define SUPERHYWAY_DEVICE_ID_SH5_DMAC	0x0183

struct superhyway_vcr_info {
	u8	perr_flags;	/* P-port Error flags */
	u8	merr_flags;	/* Module Error flags */
	u16	mod_vers;	/* Module Version */
	u16	mod_id;		/* Module ID */
	u8	bot_mb;		/* Bottom Memory block */
	u8	top_mb;		/* Top Memory block */
};

struct superhyway_ops {
	int (*read_vcr)(unsigned long base, struct superhyway_vcr_info *vcr);
	int (*write_vcr)(unsigned long base, struct superhyway_vcr_info vcr);
};

struct superhyway_bus {
	struct superhyway_ops *ops;
};

extern struct superhyway_bus superhyway_channels[];

struct superhyway_device_id {
	unsigned int id;
	unsigned long driver_data;
};

struct superhyway_device;
extern struct bus_type superhyway_bus_type;

struct superhyway_driver {
	char *name;

	const struct superhyway_device_id *id_table;
	struct device_driver drv;

	int (*probe)(struct superhyway_device *dev, const struct superhyway_device_id *id);
	void (*remove)(struct superhyway_device *dev);
};

#define to_superhyway_driver(d)	container_of((d), struct superhyway_driver, drv)

struct superhyway_device {
	char name[32];

	struct device dev;

	struct superhyway_device_id id;
	struct superhyway_driver *drv;
	struct superhyway_bus *bus;

	int num_resources;
	struct resource *resource;
	struct superhyway_vcr_info vcr;
};

#define to_superhyway_device(d)	container_of((d), struct superhyway_device, dev)

#define superhyway_get_drvdata(d)	dev_get_drvdata(&(d)->dev)
#define superhyway_set_drvdata(d,p)	dev_set_drvdata(&(d)->dev, (p))

static inline int
superhyway_read_vcr(struct superhyway_device *dev, unsigned long base,
		    struct superhyway_vcr_info *vcr)
{
	return dev->bus->ops->read_vcr(base, vcr);
}

static inline int
superhyway_write_vcr(struct superhyway_device *dev, unsigned long base,
		     struct superhyway_vcr_info vcr)
{
	return dev->bus->ops->write_vcr(base, vcr);
}

extern int superhyway_scan_bus(struct superhyway_bus *);

/* drivers/sh/superhyway/superhyway.c */
int superhyway_register_driver(struct superhyway_driver *);
void superhyway_unregister_driver(struct superhyway_driver *);
int superhyway_add_device(unsigned long base, struct superhyway_device *, struct superhyway_bus *);
int superhyway_add_devices(struct superhyway_bus *bus, struct superhyway_device **devices, int nr_devices);

/* drivers/sh/superhyway/superhyway-sysfs.c */
extern struct device_attribute superhyway_dev_attrs[];

#endif /* __LINUX_SUPERHYWAY_H */

