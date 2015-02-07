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
 * IBM PowerPC Virtual I/O Infrastructure Support.
 *
 * Copyright (c) 2003 IBM Corp.
 *  Dave Engebretsen engebret@us.ibm.com
 *  Santiago Leon santil@us.ibm.com
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version
 * 2 of the License, or (at your option) any later version.
 */

#ifndef _ASM_POWERPC_VIO_H
#define _ASM_POWERPC_VIO_H
#ifdef __KERNEL__

#include <linux/init.h>
#include <linux/errno.h>
#include <linux/device.h>
#include <linux/dma-mapping.h>
#include <linux/mod_devicetable.h>

#include <asm/hvcall.h>
#include <asm/scatterlist.h>

/*
 * Architecture-specific constants for drivers to
 * extract attributes of the device using vio_get_attribute()
 */
#define VETH_MAC_ADDR "local-mac-address"
#define VETH_MCAST_FILTER_SIZE "ibm,mac-address-filters"

/* End architecture-specific constants */

#define h_vio_signal(ua, mode) \
  plpar_hcall_norets(H_VIO_SIGNAL, ua, mode)

#define VIO_IRQ_DISABLE		0UL
#define VIO_IRQ_ENABLE		1UL

/*
 * VIO CMO minimum entitlement for all devices and spare entitlement
 */
#define VIO_CMO_MIN_ENT 1562624

struct iommu_table;

/**
 * vio_dev - This structure is used to describe virtual I/O devices.
 *
 * @desired: set from return of driver's get_desired_dma() function
 * @entitled: bytes of IO data that has been reserved for this device.
 * @allocated: bytes of IO data currently in use by the device.
 * @allocs_failed: number of DMA failures due to insufficient entitlement.
 */
struct vio_dev {
	const char *name;
	const char *type;
	uint32_t unit_address;
	unsigned int irq;
	struct {
		size_t desired;
		size_t entitled;
		size_t allocated;
		atomic_t allocs_failed;
	} cmo;
	struct device dev;
};

struct vio_driver {
	const struct vio_device_id *id_table;
	int (*probe)(struct vio_dev *dev, const struct vio_device_id *id);
	int (*remove)(struct vio_dev *dev);
	/* A driver must have a get_desired_dma() function to
	 * be loaded in a CMO environment if it uses DMA.
	 */
	unsigned long (*get_desired_dma)(struct vio_dev *dev);
	struct device_driver driver;
};

extern int vio_register_driver(struct vio_driver *drv);
extern void vio_unregister_driver(struct vio_driver *drv);

extern int vio_cmo_entitlement_update(size_t);
extern void vio_cmo_set_dev_desired(struct vio_dev *viodev, size_t desired);

extern void __devinit vio_unregister_device(struct vio_dev *dev);

struct device_node;

extern struct vio_dev *vio_register_device_node(
		struct device_node *node_vdev);
extern const void *vio_get_attribute(struct vio_dev *vdev, char *which,
		int *length);
#ifdef CONFIG_PPC_PSERIES
extern struct vio_dev *vio_find_node(struct device_node *vnode);
extern int vio_enable_interrupts(struct vio_dev *dev);
extern int vio_disable_interrupts(struct vio_dev *dev);
#else
static inline int vio_enable_interrupts(struct vio_dev *dev)
{
	return 0;
}
#endif

static inline struct vio_driver *to_vio_driver(struct device_driver *drv)
{
	return container_of(drv, struct vio_driver, driver);
}

static inline struct vio_dev *to_vio_dev(struct device *dev)
{
	return container_of(dev, struct vio_dev, dev);
}

#endif /* __KERNEL__ */
#endif /* _ASM_POWERPC_VIO_H */
