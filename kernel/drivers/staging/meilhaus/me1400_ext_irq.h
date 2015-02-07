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
 * @file me1400_ext_irq.h
 *
 * @brief ME-1400 external interrupt implementation.
 * @note Copyright (C) 2006 Meilhaus Electronic GmbH (support@meilhaus.de)
 * @author Guenter Gebhardt
 */

#ifndef _ME1400_EXT_IRQ_H_
#define _ME1400_EXT_IRQ_H_

#include <linux/sched.h>

#include "mesubdevice.h"
#include "meslock.h"

#ifdef __KERNEL__

/**
 * @brief The ME-1400 external interrupt subdevice class.
 */
typedef struct me1400_ext_irq_subdevice {
	/* Inheritance */
	me_subdevice_t base;			/**< The subdevice base class. */

	/* Attributes */
	spinlock_t subdevice_lock;		/**< Spin lock to protect the subdevice from concurrent access. */
	spinlock_t *clk_src_reg_lock;	/**< Lock protecting the clock control register. */

	wait_queue_head_t wait_queue;	/**< Queue to put on threads waiting for an interrupt. */

	uint32_t device_id;				/**< The device id of the device holding the subdevice. */
	int irq;						/**< The irq number assigned by PCI BIOS. */
	int rised;						/**< If true an interrupt has occured. */
	unsigned int n;					/**< The number of interrupt since the driver was loaded. */

	unsigned long plx_intcs_reg;	/**< The PLX interrupt control and status register. */
	unsigned long ctrl_reg;			/**< The control register. */
#ifdef MEDEBUG_DEBUG_REG
	unsigned long reg_base;
#endif
} me1400_ext_irq_subdevice_t;

/**
 * @brief The constructor to generate a ME-1400 external interrupt instance.
 *
 * @param plx_reg_base The register base address of the PLX chip as returned by the PCI BIOS.
 * @param me1400_reg_base The register base address of the ME-1400 device as returned by the PCI BIOS.
 * @param irq The irq assigned by the PCI BIOS.
 *
 * @return Pointer to new instance on success.\n
 * NULL on error.
 */
me1400_ext_irq_subdevice_t *me1400_ext_irq_constructor(uint32_t device_id,
						       uint32_t plx_reg_base,
						       uint32_t me1400_reg_base,
						       spinlock_t *
						       clk_src_reg_lock,
						       int irq);

#endif
#endif
