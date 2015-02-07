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
 * Broadcom 43xx PCI-SSB bridge module
 *
 * This technically is a separate PCI driver module, but
 * because of its small size we include it in the SSB core
 * instead of creating a standalone module.
 *
 * Copyright 2007  Michael Buesch <mb@bu3sch.de>
 *
 * Licensed under the GNU/GPL. See COPYING for details.
 */

#include <linux/pci.h>
#include <linux/ssb/ssb.h>

#include "ssb_private.h"


static const struct pci_device_id b43_pci_bridge_tbl[] = {
	{ PCI_DEVICE(PCI_VENDOR_ID_BROADCOM, 0x4301) },
	{ PCI_DEVICE(PCI_VENDOR_ID_BROADCOM, 0x4307) },
	{ PCI_DEVICE(PCI_VENDOR_ID_BROADCOM, 0x4311) },
	{ PCI_DEVICE(PCI_VENDOR_ID_BROADCOM, 0x4312) },
	{ PCI_DEVICE(PCI_VENDOR_ID_BROADCOM, 0x4318) },
	{ PCI_DEVICE(PCI_VENDOR_ID_BROADCOM, 0x4319) },
	{ PCI_DEVICE(PCI_VENDOR_ID_BROADCOM, 0x4320) },
	{ PCI_DEVICE(PCI_VENDOR_ID_BROADCOM, 0x4321) },
	{ PCI_DEVICE(PCI_VENDOR_ID_BROADCOM, 0x4324) },
	{ PCI_DEVICE(PCI_VENDOR_ID_BROADCOM, 0x4325) },
	{ PCI_DEVICE(PCI_VENDOR_ID_BROADCOM, 0x4328) },
	{ PCI_DEVICE(PCI_VENDOR_ID_BROADCOM, 0x4329) },
	{ 0, },
};
MODULE_DEVICE_TABLE(pci, b43_pci_bridge_tbl);

static struct pci_driver b43_pci_bridge_driver = {
	.name = "b43-pci-bridge",
	.id_table = b43_pci_bridge_tbl,
};


int __init b43_pci_ssb_bridge_init(void)
{
	return ssb_pcihost_register(&b43_pci_bridge_driver);
}

void __exit b43_pci_ssb_bridge_exit(void)
{
	ssb_pcihost_unregister(&b43_pci_bridge_driver);
}
