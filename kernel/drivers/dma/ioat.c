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
 * Intel I/OAT DMA Linux driver
 * Copyright(c) 2007 - 2009 Intel Corporation.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms and conditions of the GNU General Public License,
 * version 2, as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin St - Fifth Floor, Boston, MA 02110-1301 USA.
 *
 * The full GNU General Public License is included in this distribution in
 * the file called "COPYING".
 *
 */

/*
 * This driver supports an Intel I/OAT DMA engine, which does asynchronous
 * copy operations.
 */

#include <linux/init.h>
#include <linux/module.h>
#include <linux/pci.h>
#include <linux/interrupt.h>
#include <linux/dca.h>
#include "ioatdma.h"
#include "ioatdma_registers.h"
#include "ioatdma_hw.h"

MODULE_VERSION(IOAT_DMA_VERSION);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Intel Corporation");

static struct pci_device_id ioat_pci_tbl[] = {
	/* I/OAT v1 platforms */
	{ PCI_DEVICE(PCI_VENDOR_ID_INTEL, PCI_DEVICE_ID_INTEL_IOAT) },
	{ PCI_DEVICE(PCI_VENDOR_ID_INTEL, PCI_DEVICE_ID_INTEL_IOAT_CNB)  },
	{ PCI_DEVICE(PCI_VENDOR_ID_INTEL, PCI_DEVICE_ID_INTEL_IOAT_SCNB) },
	{ PCI_DEVICE(PCI_VENDOR_ID_UNISYS, PCI_DEVICE_ID_UNISYS_DMA_DIRECTOR) },

	/* I/OAT v2 platforms */
	{ PCI_DEVICE(PCI_VENDOR_ID_INTEL, PCI_DEVICE_ID_INTEL_IOAT_SNB) },

	/* I/OAT v3 platforms */
	{ PCI_DEVICE(PCI_VENDOR_ID_INTEL, PCI_DEVICE_ID_INTEL_IOAT_TBG0) },
	{ PCI_DEVICE(PCI_VENDOR_ID_INTEL, PCI_DEVICE_ID_INTEL_IOAT_TBG1) },
	{ PCI_DEVICE(PCI_VENDOR_ID_INTEL, PCI_DEVICE_ID_INTEL_IOAT_TBG2) },
	{ PCI_DEVICE(PCI_VENDOR_ID_INTEL, PCI_DEVICE_ID_INTEL_IOAT_TBG3) },
	{ PCI_DEVICE(PCI_VENDOR_ID_INTEL, PCI_DEVICE_ID_INTEL_IOAT_TBG4) },
	{ PCI_DEVICE(PCI_VENDOR_ID_INTEL, PCI_DEVICE_ID_INTEL_IOAT_TBG5) },
	{ PCI_DEVICE(PCI_VENDOR_ID_INTEL, PCI_DEVICE_ID_INTEL_IOAT_TBG6) },
	{ PCI_DEVICE(PCI_VENDOR_ID_INTEL, PCI_DEVICE_ID_INTEL_IOAT_TBG7) },
	{ 0, }
};

struct ioat_device {
	struct pci_dev		*pdev;
	void __iomem		*iobase;
	struct ioatdma_device	*dma;
	struct dca_provider	*dca;
};

static int __devinit ioat_probe(struct pci_dev *pdev,
				const struct pci_device_id *id);
static void __devexit ioat_remove(struct pci_dev *pdev);

static int ioat_dca_enabled = 1;
module_param(ioat_dca_enabled, int, 0644);
MODULE_PARM_DESC(ioat_dca_enabled, "control support of dca service (default: 1)");

static struct pci_driver ioat_pci_driver = {
	.name		= "ioatdma",
	.id_table	= ioat_pci_tbl,
	.probe		= ioat_probe,
	.remove		= __devexit_p(ioat_remove),
};

static int __devinit ioat_probe(struct pci_dev *pdev,
				const struct pci_device_id *id)
{
	void __iomem *iobase;
	struct ioat_device *device;
	unsigned long mmio_start, mmio_len;
	int err;

	err = pci_enable_device(pdev);
	if (err)
		goto err_enable_device;

	err = pci_request_regions(pdev, ioat_pci_driver.name);
	if (err)
		goto err_request_regions;

	err = pci_set_dma_mask(pdev, DMA_64BIT_MASK);
	if (err)
		err = pci_set_dma_mask(pdev, DMA_32BIT_MASK);
	if (err)
		goto err_set_dma_mask;

	err = pci_set_consistent_dma_mask(pdev, DMA_64BIT_MASK);
	if (err)
		err = pci_set_consistent_dma_mask(pdev, DMA_32BIT_MASK);
	if (err)
		goto err_set_dma_mask;

	mmio_start = pci_resource_start(pdev, 0);
	mmio_len = pci_resource_len(pdev, 0);
	iobase = ioremap(mmio_start, mmio_len);
	if (!iobase) {
		err = -ENOMEM;
		goto err_ioremap;
	}

	device = kzalloc(sizeof(*device), GFP_KERNEL);
	if (!device) {
		err = -ENOMEM;
		goto err_kzalloc;
	}
	device->pdev = pdev;
	pci_set_drvdata(pdev, device);
	device->iobase = iobase;

	pci_set_master(pdev);

	switch (readb(iobase + IOAT_VER_OFFSET)) {
	case IOAT_VER_1_2:
		device->dma = ioat_dma_probe(pdev, iobase);
		if (device->dma && ioat_dca_enabled)
			device->dca = ioat_dca_init(pdev, iobase);
		break;
	case IOAT_VER_2_0:
		device->dma = ioat_dma_probe(pdev, iobase);
		if (device->dma && ioat_dca_enabled)
			device->dca = ioat2_dca_init(pdev, iobase);
		break;
	case IOAT_VER_3_0:
		device->dma = ioat_dma_probe(pdev, iobase);
		if (device->dma && ioat_dca_enabled)
			device->dca = ioat3_dca_init(pdev, iobase);
		break;
	default:
		err = -ENODEV;
		break;
	}
	if (!device->dma)
		err = -ENODEV;

	if (err)
		goto err_version;

	return 0;

err_version:
	kfree(device);
err_kzalloc:
	iounmap(iobase);
err_ioremap:
err_set_dma_mask:
	pci_release_regions(pdev);
	pci_disable_device(pdev);
err_request_regions:
err_enable_device:
	return err;
}

static void __devexit ioat_remove(struct pci_dev *pdev)
{
	struct ioat_device *device = pci_get_drvdata(pdev);

	dev_err(&pdev->dev, "Removing dma and dca services\n");
	if (device->dca) {
		unregister_dca_provider(device->dca);
		free_dca_provider(device->dca);
		device->dca = NULL;
	}

	if (device->dma) {
		ioat_dma_remove(device->dma);
		device->dma = NULL;
	}

	kfree(device);
}

static int __init ioat_init_module(void)
{
	return pci_register_driver(&ioat_pci_driver);
}
module_init(ioat_init_module);

static void __exit ioat_exit_module(void)
{
	pci_unregister_driver(&ioat_pci_driver);
}
module_exit(ioat_exit_module);
