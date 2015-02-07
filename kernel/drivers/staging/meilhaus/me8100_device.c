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

/**
 * @file me8100_device.c
 *
 * @brief ME-8100 device class implementation.
 * @note Copyright (C) 2007 Meilhaus Electronic GmbH (support@meilhaus.de)
 * @author Guenter Gebhardt
 */

/*
 * Copyright (C) 2007 Meilhaus Electronic GmbH (support@meilhaus.de)
 *
 * This file is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#ifndef __KERNEL__
#  define __KERNEL__
#endif

#ifndef MODULE
#  define MODULE
#endif

#include <linux/module.h>

#include <linux/pci.h>
#include <linux/slab.h>

#include "meids.h"
#include "meerror.h"
#include "mecommon.h"
#include "meinternal.h"

#include "medebug.h"
#include "medevice.h"
#include "me8100_device.h"
#include "mesubdevice.h"
#include "me8100_di.h"
#include "me8100_do.h"
#include "me8254.h"

me_device_t *me8100_pci_constructor(struct pci_dev *pci_device)
{
	me8100_device_t *me8100_device;
	me_subdevice_t *subdevice;
	unsigned int version_idx;
	int err;
	int i;

	PDEBUG("executed.\n");

	// Allocate structure for device instance.
	me8100_device = kmalloc(sizeof(me8100_device_t), GFP_KERNEL);

	if (!me8100_device) {
		PERROR("Cannot get memory for device instance.\n");
		return NULL;
	}

	memset(me8100_device, 0, sizeof(me8100_device_t));

	// Initialize base class structure.
	err = me_device_pci_init((me_device_t *) me8100_device, pci_device);

	if (err) {
		kfree(me8100_device);
		PERROR("Cannot initialize device base class.\n");
		return NULL;
	}

	/* Get the index in the device version information table. */
	version_idx =
	    me8100_versions_get_device_index(me8100_device->base.info.pci.
					     device_id);

	// Initialize spin lock .
	spin_lock_init(&me8100_device->dio_ctrl_reg_lock);
	spin_lock_init(&me8100_device->ctr_ctrl_reg_lock);
	spin_lock_init(&me8100_device->clk_src_reg_lock);

	// Create subdevice instances.

	for (i = 0; i < me8100_versions[version_idx].di_subdevices; i++) {
		subdevice =
		    (me_subdevice_t *) me8100_di_constructor(me8100_device->
							     base.info.pci.
							     reg_bases[2],
							     me8100_device->
							     base.info.pci.
							     reg_bases[1], i,
							     me8100_device->
							     base.irq,
							     &me8100_device->
							     dio_ctrl_reg_lock);

		if (!subdevice) {
			me_device_deinit((me_device_t *) me8100_device);
			kfree(me8100_device);
			PERROR("Cannot get memory for subdevice.\n");
			return NULL;
		}

		me_slist_add_subdevice_tail(&me8100_device->base.slist,
					    subdevice);
	}

	for (i = 0; i < me8100_versions[version_idx].do_subdevices; i++) {
		subdevice =
		    (me_subdevice_t *) me8100_do_constructor(me8100_device->
							     base.info.pci.
							     reg_bases[2], i,
							     &me8100_device->
							     dio_ctrl_reg_lock);

		if (!subdevice) {
			me_device_deinit((me_device_t *) me8100_device);
			kfree(me8100_device);
			PERROR("Cannot get memory for subdevice.\n");
			return NULL;
		}

		me_slist_add_subdevice_tail(&me8100_device->base.slist,
					    subdevice);
	}

	for (i = 0; i < me8100_versions[version_idx].ctr_subdevices; i++) {
		subdevice =
		    (me_subdevice_t *) me8254_constructor(me8100_device->base.
							  info.pci.device_id,
							  me8100_device->base.
							  info.pci.reg_bases[2],
							  0, i,
							  &me8100_device->
							  ctr_ctrl_reg_lock,
							  &me8100_device->
							  clk_src_reg_lock);

		if (!subdevice) {
			me_device_deinit((me_device_t *) me8100_device);
			kfree(me8100_device);
			PERROR("Cannot get memory for subdevice.\n");
			return NULL;
		}

		me_slist_add_subdevice_tail(&me8100_device->base.slist,
					    subdevice);
	}

	return (me_device_t *) me8100_device;
}

// Init and exit of module.

static int __init me8100_init(void)
{
	PDEBUG("executed.\n.");
	return ME_ERRNO_SUCCESS;
}

static void __exit me8100_exit(void)
{
	PDEBUG("executed.\n.");
}

module_init(me8100_init);

module_exit(me8100_exit);

// Administrative stuff for modinfo.
MODULE_AUTHOR("Guenter Gebhardt <g.gebhardt@meilhaus.de>");
MODULE_DESCRIPTION("Device Driver Module for Template Device");
MODULE_SUPPORTED_DEVICE("Meilhaus Template Devices");
MODULE_LICENSE("GPL");

// Export the constructor.
EXPORT_SYMBOL(me8100_pci_constructor);
