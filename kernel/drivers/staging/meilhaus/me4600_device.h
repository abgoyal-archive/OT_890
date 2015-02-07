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
 * @file me4600_device.h
 *
 * @brief ME-4600 device class.
 * @note Copyright (C) 2007 Meilhaus Electronic GmbH (support@meilhaus.de)
 * @author Guenter Gebhardt
 * @author Krzysztof Gantzke	(k.gantzke@meilhaus.de)
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

#ifndef _ME4600_DEVICE_H
#define _ME4600_DEVICE_H

#include <linux/pci.h>
#include <linux/spinlock.h>

#include "medevice.h"

#ifdef __KERNEL__

/**
 * @brief Structure holding ME-4600 device capabilities.
 */
typedef struct me4600_version {
	uint16_t device_id;
	unsigned int do_subdevices;
	unsigned int di_subdevices;
	unsigned int dio_subdevices;
	unsigned int ctr_subdevices;
	unsigned int ai_subdevices;
	unsigned int ai_channels;
	unsigned int ai_ranges;
	unsigned int ai_isolated;
	unsigned int ai_sh;
	unsigned int ao_subdevices;
	unsigned int ao_fifo;	//How many devices have FIFO
	unsigned int ext_irq_subdevices;
} me4600_version_t;

/**
 * @brief ME-4600 device capabilities.
 */
static me4600_version_t me4600_versions[] = {
	{PCI_DEVICE_ID_MEILHAUS_ME4610, 0, 0, 4, 3, 1, 16, 1, 0, 0, 0, 0, 1},

	{PCI_DEVICE_ID_MEILHAUS_ME4650, 0, 0, 4, 0, 1, 16, 4, 0, 0, 0, 0, 1},

	{PCI_DEVICE_ID_MEILHAUS_ME4660, 0, 0, 4, 3, 1, 16, 4, 0, 0, 2, 0, 1},
	{PCI_DEVICE_ID_MEILHAUS_ME4660I, 1, 1, 2, 3, 1, 16, 4, 1, 0, 2, 0, 1},
	{PCI_DEVICE_ID_MEILHAUS_ME4660S, 0, 0, 4, 3, 1, 16, 4, 0, 1, 2, 0, 1},
	{PCI_DEVICE_ID_MEILHAUS_ME4660IS, 1, 1, 2, 3, 1, 16, 4, 1, 1, 2, 0, 1},

	{PCI_DEVICE_ID_MEILHAUS_ME4670, 0, 0, 4, 3, 1, 32, 4, 0, 0, 4, 0, 1},
	{PCI_DEVICE_ID_MEILHAUS_ME4670I, 1, 1, 2, 3, 1, 32, 4, 1, 0, 4, 0, 1},
	{PCI_DEVICE_ID_MEILHAUS_ME4670S, 0, 0, 4, 3, 1, 32, 4, 0, 1, 4, 0, 1},
	{PCI_DEVICE_ID_MEILHAUS_ME4670IS, 1, 1, 2, 3, 1, 32, 4, 1, 1, 4, 0, 1},

	{PCI_DEVICE_ID_MEILHAUS_ME4680, 0, 0, 4, 3, 1, 32, 4, 0, 0, 4, 4, 1},
	{PCI_DEVICE_ID_MEILHAUS_ME4680I, 1, 1, 2, 3, 1, 32, 4, 1, 0, 4, 4, 1},
	{PCI_DEVICE_ID_MEILHAUS_ME4680S, 0, 0, 4, 3, 1, 32, 4, 0, 1, 4, 4, 1},
	{PCI_DEVICE_ID_MEILHAUS_ME4680IS, 1, 1, 2, 3, 1, 32, 4, 1, 1, 4, 4, 1},

	{0},
};

#define ME4600_DEVICE_VERSIONS (sizeof(me4600_versions) / sizeof(me4600_version_t) - 1)	/**< Returns the number of entries in #me4600_versions. */

/**
 * @brief Returns the index of the device entry in #me4600_versions.
 *
 * @param device_id The PCI device id of the device to query.
 * @return The index of the device in #me4600_versions.
 */
static inline unsigned int me4600_versions_get_device_index(uint16_t device_id)
{
	unsigned int i;
	for (i = 0; i < ME4600_DEVICE_VERSIONS; i++)
		if (me4600_versions[i].device_id == device_id)
			break;
	return i;
}

/**
 * @brief The ME-4600 device class structure.
 */
typedef struct me4600_device {
	me_device_t base;					/**< The Meilhaus device base class. */

	/* Child class attributes. */
	spinlock_t preload_reg_lock;		/**< Guards the preload register of the anaolog output devices. */
	unsigned int preload_flags;			/**< Used in conjunction with #preload_reg_lock. */
	spinlock_t dio_lock;				/**< Locks the control register of the digital input/output subdevices. */
	spinlock_t ai_ctrl_lock;			/**< Locks the control register of the analog input subdevice. */
	spinlock_t ctr_ctrl_reg_lock;		/**< Locks the counter control register. */
	spinlock_t ctr_clk_src_reg_lock;	/**< Not used on this device but needed for the me8254 subdevice constructor call. */
} me4600_device_t;

/**
 * @brief The ME-4600 device class constructor.
 *
 * @param pci_device The pci device structure given by the PCI subsystem.
 * @param me_bosch_fw If set the device shall use the bosch firmware. (Only for special BOSCH build)
 *
 * @return On succes a new ME-4600 device instance. \n
 *         NULL on error.
 */

#ifdef BOSCH
/**
 * @brief The ME-4600 device class constructor.
 *
 * @param pci_device The pci device structure given by the PCI subsystem.
 * @param me_bosch_fw If set the device shall use the bosch firmware.
 *
 * @return On succes a new ME-4600 device instance. \n
 *         NULL on error.
 */
me_device_t *me4600_pci_constructor(struct pci_dev *pci_device, int me_bosch_fw)
    __attribute__ ((weak));
#else //~BOSCH
/**
 * @brief The ME-4600 device class constructor.
 *
 * @param pci_device The pci device structure given by the PCI subsystem.
 *
 * @return On succes a new ME-4600 device instance. \n
 *         NULL on error.
 */
me_device_t *me4600_pci_constructor(struct pci_dev *pci_device)
    __attribute__ ((weak));
#endif

#endif
#endif
