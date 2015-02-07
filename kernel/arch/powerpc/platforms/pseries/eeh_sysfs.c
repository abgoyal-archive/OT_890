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
 * Sysfs entries for PCI Error Recovery for PAPR-compliant platform.
 * Copyright IBM Corporation 2007
 * Copyright Linas Vepstas <linas@austin.ibm.com> 2007
 *
 * All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or (at
 * your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE, GOOD TITLE or
 * NON INFRINGEMENT.  See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 * Send comments and feedback to Linas Vepstas <linas@austin.ibm.com>
 */
#include <linux/pci.h>
#include <asm/ppc-pci.h>
#include <asm/pci-bridge.h>
#include <linux/kobject.h>

/**
 * EEH_SHOW_ATTR -- create sysfs entry for eeh statistic
 * @_name: name of file in sysfs directory
 * @_memb: name of member in struct pci_dn to access
 * @_format: printf format for display
 *
 * All of the attributes look very similar, so just
 * auto-gen a cut-n-paste routine to display them.
 */
#define EEH_SHOW_ATTR(_name,_memb,_format)               \
static ssize_t eeh_show_##_name(struct device *dev,      \
		struct device_attribute *attr, char *buf)          \
{                                                        \
	struct pci_dev *pdev = to_pci_dev(dev);               \
	struct device_node *dn = pci_device_to_OF_node(pdev); \
	struct pci_dn *pdn;                                   \
	                                                      \
	if (!dn || PCI_DN(dn) == NULL)                        \
		return 0;                                          \
	                                                      \
	pdn = PCI_DN(dn);                                     \
	return sprintf(buf, _format "\n", pdn->_memb);        \
}                                                        \
static DEVICE_ATTR(_name, S_IRUGO, eeh_show_##_name, NULL);


EEH_SHOW_ATTR(eeh_mode, eeh_mode, "0x%x");
EEH_SHOW_ATTR(eeh_config_addr, eeh_config_addr, "0x%x");
EEH_SHOW_ATTR(eeh_pe_config_addr, eeh_pe_config_addr, "0x%x");
EEH_SHOW_ATTR(eeh_check_count, eeh_check_count, "%d");
EEH_SHOW_ATTR(eeh_freeze_count, eeh_freeze_count, "%d");
EEH_SHOW_ATTR(eeh_false_positives, eeh_false_positives, "%d");

void eeh_sysfs_add_device(struct pci_dev *pdev)
{
	int rc=0;

	rc += device_create_file(&pdev->dev, &dev_attr_eeh_mode);
	rc += device_create_file(&pdev->dev, &dev_attr_eeh_config_addr);
	rc += device_create_file(&pdev->dev, &dev_attr_eeh_pe_config_addr);
	rc += device_create_file(&pdev->dev, &dev_attr_eeh_check_count);
	rc += device_create_file(&pdev->dev, &dev_attr_eeh_false_positives);
	rc += device_create_file(&pdev->dev, &dev_attr_eeh_freeze_count);

	if (rc)
		printk(KERN_WARNING "EEH: Unable to create sysfs entries\n");
}

void eeh_sysfs_remove_device(struct pci_dev *pdev)
{
	device_remove_file(&pdev->dev, &dev_attr_eeh_mode);
	device_remove_file(&pdev->dev, &dev_attr_eeh_config_addr);
	device_remove_file(&pdev->dev, &dev_attr_eeh_pe_config_addr);
	device_remove_file(&pdev->dev, &dev_attr_eeh_check_count);
	device_remove_file(&pdev->dev, &dev_attr_eeh_false_positives);
	device_remove_file(&pdev->dev, &dev_attr_eeh_freeze_count);
}

