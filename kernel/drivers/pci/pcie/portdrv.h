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
 * File:	portdrv.h
 * Purpose:	PCI Express Port Bus Driver's Internal Data Structures
 *
 * Copyright (C) 2004 Intel
 * Copyright (C) Tom Long Nguyen (tom.l.nguyen@intel.com)
 */

#ifndef _PORTDRV_H_
#define _PORTDRV_H_

#include <linux/compiler.h>

#if !defined(PCI_CAP_ID_PME)
#define PCI_CAP_ID_PME			1
#endif

#if !defined(PCI_CAP_ID_EXP)
#define PCI_CAP_ID_EXP			0x10
#endif

#define PORT_TYPE_MASK			0xf
#define PORT_TO_SLOT_MASK		0x100
#define SLOT_HP_CAPABLE_MASK		0x40
#define PCIE_CAPABILITIES_REG		0x2
#define PCIE_SLOT_CAPABILITIES_REG	0x14
#define PCIE_PORT_DEVICE_MAXSERVICES	4

#define get_descriptor_id(type, service) (((type - 4) << 4) | service)

struct pcie_port_device_ext {
	int interrupt_mode;	/* [0:INTx | 1:MSI | 2:MSI-X] */
};

extern struct bus_type pcie_port_bus_type;
extern int pcie_port_device_probe(struct pci_dev *dev);
extern int pcie_port_device_register(struct pci_dev *dev);
#ifdef CONFIG_PM
extern int pcie_port_device_suspend(struct pci_dev *dev, pm_message_t state);
extern int pcie_port_device_resume(struct pci_dev *dev);
#endif
extern void pcie_port_device_remove(struct pci_dev *dev);
extern int __must_check pcie_port_bus_register(void);
extern void pcie_port_bus_unregister(void);

#endif /* _PORTDRV_H_ */
