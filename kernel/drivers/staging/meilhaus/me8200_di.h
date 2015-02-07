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
 * @file me8200_di.h
 *
 * @brief ME-8200 digital input subdevice class.
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

#ifndef _ME8200_DI_H_
#define _ME8200_DI_H_

#include "mesubdevice.h"

#ifdef __KERNEL__

/**
 * @brief The template subdevice class.
 */
typedef struct me8200_di_subdevice {
	/* Inheritance */
	me_subdevice_t base;			/**< The subdevice base class. */

	/* Attributes */
	spinlock_t subdevice_lock;		/**< Spin lock to protect the subdevice from concurrent access. */
	spinlock_t *ctrl_reg_lock;
	spinlock_t *irq_ctrl_lock;
	spinlock_t *irq_mode_lock;

	unsigned int di_idx;
	unsigned int version;

	int irq;						/**< The number of the interrupt request. */
	volatile int rised;				/**< Flag to indicate if an interrupt occured. */
	uint status_flag;				/**< Default interupt status flag */
	uint status_value;				/**< Interupt status */
	uint status_value_edges;			/**< Extended interupt status */
	uint line_value;
	int count;						/**< Counts the number of interrupts occured. */
	uint8_t compare_value;
	uint8_t filtering_flag;

	wait_queue_head_t wait_queue;	/**< To wait on interrupts. */

	unsigned long port_reg;			/**< The digital input port. */
	unsigned long compare_reg;		/**< The register to hold the value to compare with. */
	unsigned long mask_reg;			/**< The register to hold the mask. */
	unsigned long irq_mode_reg;		/**< The interrupt mode register. */
	unsigned long irq_ctrl_reg;		/**< The interrupt control register. */
	unsigned long irq_status_reg;	/**< The interrupt status register. Also interrupt reseting register (firmware version 7 and later).*/
#ifdef MEDEBUG_DEBUG_REG
	unsigned long reg_base;
#endif
	unsigned long firmware_version_reg;	/**< The interrupt reseting register. */

	unsigned long irq_status_low_reg;	/**< The interrupt extended status register (low part). */
	unsigned long irq_status_high_reg;	/**< The interrupt extended status register (high part). */
} me8200_di_subdevice_t;

/**
 * @brief The constructor to generate a ME-8200 digital input subdevice instance.
 *
 * @param reg_base The register base address of the device as returned by the PCI BIOS.
 *
 * @return Pointer to new instance on success.\n
 * NULL on error.
 */
me8200_di_subdevice_t *me8200_di_constructor(uint32_t me8200_reg_base,
					     unsigned int di_idx,
					     int irq,
					     spinlock_t * irq_ctrl_lock,
					     spinlock_t * irq_mode_lock);

#endif
#endif
