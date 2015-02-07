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
 * File:	pcieport_if.h
 * Purpose:	PCI Express Port Bus Driver's IF Data Structure
 *
 * Copyright (C) 2004 Intel
 * Copyright (C) Tom Long Nguyen (tom.l.nguyen@intel.com)
 */

#ifndef _PCIEPORT_IF_H_
#define _PCIEPORT_IF_H_

/* Port Type */
#define PCIE_RC_PORT			4	/* Root port of RC */
#define PCIE_SW_UPSTREAM_PORT		5	/* Upstream port of Switch */
#define PCIE_SW_DOWNSTREAM_PORT		6	/* Downstream port of Switch */
#define PCIE_ANY_PORT			7

/* Service Type */
#define PCIE_PORT_SERVICE_PME		1	/* Power Management Event */
#define PCIE_PORT_SERVICE_AER		2	/* Advanced Error Reporting */
#define PCIE_PORT_SERVICE_HP		4	/* Native Hotplug */
#define PCIE_PORT_SERVICE_VC		8	/* Virtual Channel */

/* Root/Upstream/Downstream Port's Interrupt Mode */
#define PCIE_PORT_INTx_MODE		0
#define PCIE_PORT_MSI_MODE		1
#define PCIE_PORT_MSIX_MODE		2

struct pcie_port_service_id {
	__u32 vendor, device;		/* Vendor and device ID or PCI_ANY_ID*/
	__u32 subvendor, subdevice;	/* Subsystem ID's or PCI_ANY_ID */
	__u32 class, class_mask;	/* (class,subclass,prog-if) triplet */
	__u32 port_type, service_type;	/* Port Entity */
	kernel_ulong_t driver_data;
};

struct pcie_device {
	int 		irq;	    /* Service IRQ/MSI/MSI-X Vector */
	int 		interrupt_mode;	/* [0:INTx | 1:MSI | 2:MSI-X] */	
	struct pcie_port_service_id id;	/* Service ID */
	struct pci_dev	*port;	    /* Root/Upstream/Downstream Port */
	void		*priv_data; /* Service Private Data */
	struct device	device;     /* Generic Device Interface */
};
#define to_pcie_device(d) container_of(d, struct pcie_device, device)

static inline void set_service_data(struct pcie_device *dev, void *data)
{
	dev->priv_data = data;
}

static inline void* get_service_data(struct pcie_device *dev)
{
	return dev->priv_data;
}

struct pcie_port_service_driver {
	const char *name;
	int (*probe) (struct pcie_device *dev, 
		const struct pcie_port_service_id *id);
	void (*remove) (struct pcie_device *dev);
	int (*suspend) (struct pcie_device *dev, pm_message_t state);
	int (*resume) (struct pcie_device *dev);

	/* Service Error Recovery Handler */
	struct pci_error_handlers *err_handler;

	/* Link Reset Capability - AER service driver specific */
	pci_ers_result_t (*reset_link) (struct pci_dev *dev);

	const struct pcie_port_service_id *id_table;
	struct device_driver driver;
};
#define to_service_driver(d) \
	container_of(d, struct pcie_port_service_driver, driver)

extern int pcie_port_service_register(struct pcie_port_service_driver *new);
extern void pcie_port_service_unregister(struct pcie_port_service_driver *new);

#endif /* _PCIEPORT_IF_H_ */
