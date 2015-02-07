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
 * File		pci-acpi.h
 *
 * Copyright (C) 2004 Intel
 * Copyright (C) Tom Long Nguyen (tom.l.nguyen@intel.com)
 */

#ifndef _PCI_ACPI_H_
#define _PCI_ACPI_H_

#include <linux/acpi.h>

#define OSC_QUERY_TYPE			0
#define OSC_SUPPORT_TYPE 		1
#define OSC_CONTROL_TYPE		2
#define OSC_SUPPORT_MASKS		0x1f

/*
 * _OSC DW0 Definition 
 */
#define OSC_QUERY_ENABLE		1
#define OSC_REQUEST_ERROR		2
#define OSC_INVALID_UUID_ERROR		4
#define OSC_INVALID_REVISION_ERROR	8
#define OSC_CAPABILITIES_MASK_ERROR	16

/*
 * _OSC DW1 Definition (OS Support Fields)
 */
#define OSC_EXT_PCI_CONFIG_SUPPORT		1
#define OSC_ACTIVE_STATE_PWR_SUPPORT 		2
#define OSC_CLOCK_PWR_CAPABILITY_SUPPORT	4
#define OSC_PCI_SEGMENT_GROUPS_SUPPORT		8
#define OSC_MSI_SUPPORT				16

/*
 * _OSC DW1 Definition (OS Control Fields)
 */
#define OSC_PCI_EXPRESS_NATIVE_HP_CONTROL	1
#define OSC_SHPC_NATIVE_HP_CONTROL 		2
#define OSC_PCI_EXPRESS_PME_CONTROL		4
#define OSC_PCI_EXPRESS_AER_CONTROL		8
#define OSC_PCI_EXPRESS_CAP_STRUCTURE_CONTROL	16

#define OSC_CONTROL_MASKS 	(OSC_PCI_EXPRESS_NATIVE_HP_CONTROL | 	\
				OSC_SHPC_NATIVE_HP_CONTROL | 		\
				OSC_PCI_EXPRESS_PME_CONTROL |		\
				OSC_PCI_EXPRESS_AER_CONTROL |		\
				OSC_PCI_EXPRESS_CAP_STRUCTURE_CONTROL)

#ifdef CONFIG_ACPI
extern acpi_status pci_osc_control_set(acpi_handle handle, u32 flags);
int pci_acpi_osc_support(acpi_handle handle, u32 flags);
static inline acpi_handle acpi_find_root_bridge_handle(struct pci_dev *pdev)
{
	/* Find root host bridge */
	while (pdev->bus->self)
		pdev = pdev->bus->self;

	return acpi_get_pci_rootbridge_handle(pci_domain_nr(pdev->bus),
			pdev->bus->number);
}

static inline acpi_handle acpi_pci_get_bridge_handle(struct pci_bus *pbus)
{
	int seg = pci_domain_nr(pbus), busnr = pbus->number;
	struct pci_dev *bridge = pbus->self;
	if (bridge)
		return DEVICE_ACPI_HANDLE(&(bridge->dev));
	return acpi_get_pci_rootbridge_handle(seg, busnr);
}
#else
#if !defined(AE_ERROR)
typedef u32 		acpi_status;
#define AE_ERROR      	(acpi_status) (0x0001)
#endif    
static inline acpi_status pci_osc_control_set(acpi_handle handle, u32 flags)
{return AE_ERROR;}
static inline acpi_handle acpi_find_root_bridge_handle(struct pci_dev *pdev)
{ return NULL; }
#endif

#endif	/* _PCI_ACPI_H_ */
