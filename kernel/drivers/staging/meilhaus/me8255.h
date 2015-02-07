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
 * @file me8255.h
 *
 * @brief Meilhaus PIO 8255 implementation.
 * @note Copyright (C) 2006 Meilhaus Electronic GmbH (support@meilhaus.de)
 * @author Guenter Gebhardt
 */

#ifndef _ME8255_H_
#define _ME8255_H_

#include "mesubdevice.h"
#include "meslock.h"

#ifdef __KERNEL__

/**
 * @brief The 8255 subdevice class.
 */
typedef struct me8255_subdevice {
	/* Inheritance */
	me_subdevice_t base;			/**< The subdevice base class. */

	/* Attributes */
	spinlock_t subdevice_lock;		/**< Spin lock to protect the subdevice from concurrent access. */

	int *ctrl_reg_mirror;			/**< Pointer to mirror of the control register. */
	spinlock_t *ctrl_reg_lock;		/**< Spin lock to protect #ctrl_reg and #ctrl_reg_mirror from concurrent access. */

	uint32_t device_id;				/**< The PCI device id of the device holding the 8255 chip. */
	int me8255_idx;					/**< The index of the 8255 chip on the device. */
	int dio_idx;					/**< The index of the DIO port on the 8255 chip. */

	unsigned long port_reg;			/**< Register to read or write a value from or to the port respectively. */
	unsigned long ctrl_reg;			/**< Register to configure the 8255 modes. */
} me8255_subdevice_t;

/**
 * @brief The constructor to generate a 8255 instance.
 *
 * @param device_id The kind of Meilhaus device holding the 8255.
 * @param reg_base The register base address of the device as returned by the PCI BIOS.
 * @param me8255_idx The index of the 8255 chip on the Meilhaus device.
 * @param dio_idx The index of the counter inside a 8255 chip.
 * @param ctr_reg_mirror Pointer to mirror of control register.
 * @param ctrl_reg_lock Pointer to spin lock protecting the 8255 control register and #ctrl_reg_mirror from concurrent access.
 *
 * @return Pointer to new instance on success.\n
 * NULL on error.
 */
me8255_subdevice_t *me8255_constructor(uint32_t device_id,
				       uint32_t reg_base,
				       unsigned int me8255_idx,
				       unsigned int dio_idx,
				       int *ctrl_reg_mirror,
				       spinlock_t * ctrl_reg_lock);

#endif
#endif
