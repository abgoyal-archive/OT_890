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
 *  Include file for NEC VR4100 series PCI Control Unit.
 *
 *  Copyright (C) 2004-2005  Yoichi Yuasa <yoichi_yuasa@tripeaks.co.jp>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */
#ifndef __NEC_VR41XX_PCI_H
#define __NEC_VR41XX_PCI_H

#define PCI_MASTER_ADDRESS_MASK	0x7fffffffU

struct pci_master_address_conversion {
	uint32_t bus_base_address;
	uint32_t address_mask;
	uint32_t pci_base_address;
};

struct pci_target_address_conversion {
	uint32_t address_mask;
	uint32_t bus_base_address;
};

typedef enum {
	CANNOT_LOCK_FROM_DEVICE,
	CAN_LOCK_FROM_DEVICE,
} pci_exclusive_access_t;

struct pci_mailbox_address {
	uint32_t base_address;
};

struct pci_target_address_window {
	uint32_t base_address;
};

typedef enum {
	PCI_ARBITRATION_MODE_FAIR,
	PCI_ARBITRATION_MODE_ALTERNATE_0,
	PCI_ARBITRATION_MODE_ALTERNATE_B,
} pci_arbiter_priority_control_t;

typedef enum {
	PCI_TAKE_AWAY_GNT_DISABLE,
	PCI_TAKE_AWAY_GNT_ENABLE,
} pci_take_away_gnt_mode_t;

struct pci_controller_unit_setup {
	struct pci_master_address_conversion *master_memory1;
	struct pci_master_address_conversion *master_memory2;

	struct pci_target_address_conversion *target_memory1;
	struct pci_target_address_conversion *target_memory2;

	struct pci_master_address_conversion *master_io;

	pci_exclusive_access_t exclusive_access;

	uint32_t pci_clock_max;
	uint8_t wait_time_limit_from_irdy_to_trdy;	/* Only VR4122 is supported */

	struct pci_mailbox_address *mailbox;
	struct pci_target_address_window *target_window1;
	struct pci_target_address_window *target_window2;

	uint8_t master_latency_timer;
	uint8_t retry_limit;

	pci_arbiter_priority_control_t arbiter_priority_control;
	pci_take_away_gnt_mode_t take_away_gnt_mode;

	struct resource *mem_resource;
	struct resource *io_resource;
};

extern void vr41xx_pciu_setup(struct pci_controller_unit_setup *setup);

#endif /* __NEC_VR41XX_PCI_H */
