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
 *	arch/mips/pci/fixup-sb1250.c
 *
 *	Copyright (C) 2004, 2006  MIPS Technologies, Inc.  All rights reserved.
 *	    Author:	Maciej W. Rozycki <macro@mips.com>
 *
 *	This program is free software; you can redistribute it and/or
 *	modify it under the terms of the GNU General Public License
 *	as published by the Free Software Foundation; either version
 *	2 of the License, or (at your option) any later version.
 */

#include <linux/init.h>
#include <linux/pci.h>

/*
 * Set the BCM1250, etc. PCI host bridge's TRDY timeout
 * to the finite max.
 */
static void __init quirk_sb1250_pci(struct pci_dev *dev)
{
	pci_write_config_byte(dev, 0x40, 0xff);
}
DECLARE_PCI_FIXUP_EARLY(PCI_VENDOR_ID_SIBYTE, PCI_DEVICE_ID_BCM1250_PCI,
			quirk_sb1250_pci);

/*
 * The BCM1250, etc. PCI/HT bridge reports as a host bridge.
 */
static void __init quirk_sb1250_ht(struct pci_dev *dev)
{
	dev->class = PCI_CLASS_BRIDGE_PCI << 8;
}
DECLARE_PCI_FIXUP_EARLY(PCI_VENDOR_ID_SIBYTE, PCI_DEVICE_ID_BCM1250_HT,
			quirk_sb1250_ht);

/*
 * Set the SP1011 HT/PCI bridge's TRDY timeout to the finite max.
 */
static void __init quirk_sp1011(struct pci_dev *dev)
{
	pci_write_config_byte(dev, 0x64, 0xff);
}
DECLARE_PCI_FIXUP_EARLY(PCI_VENDOR_ID_SIPACKETS, PCI_DEVICE_ID_SP1011,
			quirk_sp1011);
