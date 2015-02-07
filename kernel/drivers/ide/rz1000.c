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
 *  Copyright (C) 1995-1998  Linus Torvalds & author (see below)
 */

/*
 *  Principal Author:  mlord@pobox.com (Mark Lord)
 *
 *  See linux/MAINTAINERS for address of current maintainer.
 *
 *  This file provides support for disabling the buggy read-ahead
 *  mode of the RZ1000 IDE chipset, commonly used on Intel motherboards.
 *
 *  Dunno if this fixes both ports, or only the primary port (?).
 */

#include <linux/types.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/pci.h>
#include <linux/ide.h>
#include <linux/init.h>

#define DRV_NAME "rz1000"

static int __devinit rz1000_disable_readahead(struct pci_dev *dev)
{
	u16 reg;

	if (!pci_read_config_word (dev, 0x40, &reg) &&
	    !pci_write_config_word(dev, 0x40, reg & 0xdfff)) {
		printk(KERN_INFO "%s: disabled chipset read-ahead "
			"(buggy RZ1000/RZ1001)\n", pci_name(dev));
		return 0;
	} else {
		printk(KERN_INFO "%s: serialized, disabled unmasking "
			"(buggy RZ1000/RZ1001)\n", pci_name(dev));
		return 1;
	}
}

static const struct ide_port_info rz1000_chipset __devinitdata = {
	.name		= DRV_NAME,
	.host_flags	= IDE_HFLAG_NO_DMA,
};

static int __devinit rz1000_init_one(struct pci_dev *dev, const struct pci_device_id *id)
{
	struct ide_port_info d = rz1000_chipset;
	int rc;

	rc = pci_enable_device(dev);
	if (rc)
		return rc;

	if (rz1000_disable_readahead(dev)) {
		d.host_flags |= IDE_HFLAG_SERIALIZE;
		d.host_flags |= IDE_HFLAG_NO_UNMASK_IRQS;
	}

	return ide_pci_init_one(dev, &d, NULL);
}

static void rz1000_remove(struct pci_dev *dev)
{
	ide_pci_remove(dev);
	pci_disable_device(dev);
}

static const struct pci_device_id rz1000_pci_tbl[] = {
	{ PCI_VDEVICE(PCTECH, PCI_DEVICE_ID_PCTECH_RZ1000), 0 },
	{ PCI_VDEVICE(PCTECH, PCI_DEVICE_ID_PCTECH_RZ1001), 0 },
	{ 0, },
};
MODULE_DEVICE_TABLE(pci, rz1000_pci_tbl);

static struct pci_driver rz1000_pci_driver = {
	.name		= "RZ1000_IDE",
	.id_table	= rz1000_pci_tbl,
	.probe		= rz1000_init_one,
	.remove		= rz1000_remove,
};

static int __init rz1000_ide_init(void)
{
	return ide_pci_register_driver(&rz1000_pci_driver);
}

static void __exit rz1000_ide_exit(void)
{
	pci_unregister_driver(&rz1000_pci_driver);
}

module_init(rz1000_ide_init);
module_exit(rz1000_ide_exit);

MODULE_AUTHOR("Andre Hedrick");
MODULE_DESCRIPTION("PCI driver module for RZ1000 IDE");
MODULE_LICENSE("GPL");

