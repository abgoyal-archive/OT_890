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
 * Access ACPI _OSC method
 *
 * Copyright (C) 2006 Intel Corp.
 *	Tom Long Nguyen (tom.l.nguyen@intel.com)
 *	Zhang Yanmin (yanmin.zhang@intel.com)
 *
 */

#include <linux/module.h>
#include <linux/pci.h>
#include <linux/kernel.h>
#include <linux/errno.h>
#include <linux/pm.h>
#include <linux/suspend.h>
#include <linux/acpi.h>
#include <linux/pci-acpi.h>
#include <linux/delay.h>
#include "aerdrv.h"

/**
 * aer_osc_setup - run ACPI _OSC method
 * @pciedev: pcie_device which AER is being enabled on
 *
 * @return: Zero on success. Nonzero otherwise.
 *
 * Invoked when PCIE bus loads AER service driver. To avoid conflict with
 * BIOS AER support requires BIOS to yield AER control to OS native driver.
 **/
int aer_osc_setup(struct pcie_device *pciedev)
{
	acpi_status status = AE_NOT_FOUND;
	struct pci_dev *pdev = pciedev->port;
	acpi_handle handle = NULL;

	if (acpi_pci_disabled)
		return -1;

	handle = acpi_find_root_bridge_handle(pdev);
	if (handle) {
		status = pci_osc_control_set(handle,
					OSC_PCI_EXPRESS_AER_CONTROL |
					OSC_PCI_EXPRESS_CAP_STRUCTURE_CONTROL);
	}

	if (ACPI_FAILURE(status)) {
		dev_printk(KERN_DEBUG, &pciedev->device, "AER service couldn't "
			   "init device: %s\n",
			   (status == AE_SUPPORT || status == AE_NOT_FOUND) ?
			   "no _OSC support" : "_OSC failed");
		return -1;
	}

	return 0;
}
