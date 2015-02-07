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
 * RapidIO interconnect services
 *
 * Copyright 2005 MontaVista Software, Inc.
 * Matt Porter <mporter@kernel.crashing.org>
 *
 * This program is free software; you can redistribute  it and/or modify it
 * under  the terms of  the GNU General  Public License as published by the
 * Free Software Foundation;  either version 2 of the  License, or (at your
 * option) any later version.
 */

#include <linux/device.h>
#include <linux/list.h>
#include <linux/rio.h>

/* Functions internal to the RIO core code */

extern u32 rio_mport_get_feature(struct rio_mport *mport, int local, u16 destid,
				 u8 hopcount, int ftr);
extern int rio_create_sysfs_dev_files(struct rio_dev *rdev);
extern int rio_enum_mport(struct rio_mport *mport);
extern int rio_disc_mport(struct rio_mport *mport);

/* Structures internal to the RIO core code */
extern struct device_attribute rio_dev_attrs[];
extern spinlock_t rio_global_list_lock;

extern struct rio_route_ops __start_rio_route_ops[];
extern struct rio_route_ops __end_rio_route_ops[];

/* Helpers internal to the RIO core code */
#define DECLARE_RIO_ROUTE_SECTION(section, vid, did, add_hook, get_hook)  \
	static struct rio_route_ops __rio_route_ops __used   \
	__section(section)= { vid, did, add_hook, get_hook };

/**
 * DECLARE_RIO_ROUTE_OPS - Registers switch routing operations
 * @vid: RIO vendor ID
 * @did: RIO device ID
 * @add_hook: Callback that adds a route entry
 * @get_hook: Callback that gets a route entry
 *
 * Manipulating switch route tables in RIO is switch specific. This
 * registers a switch by vendor and device ID with two callbacks for
 * modifying and retrieving route entries in a switch. A &struct
 * rio_route_ops is initialized with the ops and placed into a
 * RIO-specific kernel section.
 */
#define DECLARE_RIO_ROUTE_OPS(vid, did, add_hook, get_hook)		\
	DECLARE_RIO_ROUTE_SECTION(.rio_route_ops,			\
			vid, did, add_hook, get_hook)

#define RIO_GET_DID(size, x)	(size ? (x & 0xffff) : ((x & 0x00ff0000) >> 16))
#define RIO_SET_DID(size, x)	(size ? (x & 0xffff) : ((x & 0x000000ff) << 16))
