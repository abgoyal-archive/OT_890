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


#ifndef I7300_IDLE_H
#define I7300_IDLE_H

#include <linux/pci.h>

/*
 * I/O AT controls (PCI bus 0 device 8 function 0)
 * DIMM controls (PCI bus 0 device 16 function 1)
 */
#define IOAT_BUS 0
#define IOAT_DEVFN PCI_DEVFN(8, 0)
#define MEMCTL_BUS 0
#define MEMCTL_DEVFN PCI_DEVFN(16, 1)

struct fbd_ioat {
	unsigned int vendor;
	unsigned int ioat_dev;
};

/*
 * The i5000 chip-set has the same hooks as the i7300
 * but support is disabled by default because this driver
 * has not been validated on that platform.
 */
#define SUPPORT_I5000 0

static const struct fbd_ioat fbd_ioat_list[] = {
	{PCI_VENDOR_ID_INTEL, PCI_DEVICE_ID_INTEL_IOAT_CNB},
#if SUPPORT_I5000
	{PCI_VENDOR_ID_INTEL, PCI_DEVICE_ID_INTEL_IOAT},
#endif
	{0, 0}
};

/* table of devices that work with this driver */
static const struct pci_device_id pci_tbl[] = {
	{ PCI_DEVICE(PCI_VENDOR_ID_INTEL, PCI_DEVICE_ID_INTEL_FBD_CNB) },
#if SUPPORT_I5000
	{ PCI_DEVICE(PCI_VENDOR_ID_INTEL, PCI_DEVICE_ID_INTEL_5000_ERR) },
#endif
	{ } /* Terminating entry */
};

/* Check for known platforms with I/O-AT */
static inline int i7300_idle_platform_probe(struct pci_dev **fbd_dev,
						struct pci_dev **ioat_dev)
{
	int i;
	struct pci_dev *memdev, *dmadev;

	memdev = pci_get_bus_and_slot(MEMCTL_BUS, MEMCTL_DEVFN);
	if (!memdev)
		return -ENODEV;

	for (i = 0; pci_tbl[i].vendor != 0; i++) {
		if (memdev->vendor == pci_tbl[i].vendor &&
		    memdev->device == pci_tbl[i].device) {
			break;
		}
	}
	if (pci_tbl[i].vendor == 0)
		return -ENODEV;

	dmadev = pci_get_bus_and_slot(IOAT_BUS, IOAT_DEVFN);
	if (!dmadev)
		return -ENODEV;

	for (i = 0; fbd_ioat_list[i].vendor != 0; i++) {
		if (dmadev->vendor == fbd_ioat_list[i].vendor &&
		    dmadev->device == fbd_ioat_list[i].ioat_dev) {
			if (fbd_dev)
				*fbd_dev = memdev;
			if (ioat_dev)
				*ioat_dev = dmadev;

			return 0;
		}
	}
	return -ENODEV;
}

#endif
