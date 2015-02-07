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
 * PCI IRQ failure handing code
 *
 * Copyright (c) 2008 James Bottomley <James.Bottomley@HansenPartnership.com>
 */

#include <linux/acpi.h>
#include <linux/device.h>
#include <linux/kernel.h>
#include <linux/pci.h>

static void pci_note_irq_problem(struct pci_dev *pdev, const char *reason)
{
	struct pci_dev *parent = to_pci_dev(pdev->dev.parent);

	dev_printk(KERN_ERR, &pdev->dev,
		   "Potentially misrouted IRQ (Bridge %s %04x:%04x)\n",
		   dev_name(&parent->dev), parent->vendor, parent->device);
	dev_printk(KERN_ERR, &pdev->dev, "%s\n", reason);
	dev_printk(KERN_ERR, &pdev->dev, "Please report to linux-kernel@vger.kernel.org\n");
	WARN_ON(1);
}

/**
 * pci_lost_interrupt - reports a lost PCI interrupt
 * @pdev:	device whose interrupt is lost
 * 
 * The primary function of this routine is to report a lost interrupt
 * in a standard way which users can recognise (instead of blaming the
 * driver).
 *
 * Returns:
 *  a suggestion for fixing it (although the driver is not required to
 * act on this).
 */
enum pci_lost_interrupt_reason pci_lost_interrupt(struct pci_dev *pdev)
{
	if (pdev->msi_enabled || pdev->msix_enabled) {
		enum pci_lost_interrupt_reason ret;

		if (pdev->msix_enabled) {
			pci_note_irq_problem(pdev, "MSIX routing failure");
			ret = PCI_LOST_IRQ_DISABLE_MSIX;
		} else {
			pci_note_irq_problem(pdev, "MSI routing failure");
			ret = PCI_LOST_IRQ_DISABLE_MSI;
		}
		return ret;
	}
#ifdef CONFIG_ACPI
	if (!(acpi_disabled || acpi_noirq)) {
		pci_note_irq_problem(pdev, "Potential ACPI misrouting please reboot with acpi=noirq");
		/* currently no way to fix acpi on the fly */
		return PCI_LOST_IRQ_DISABLE_ACPI;
	}
#endif
	pci_note_irq_problem(pdev, "unknown cause (not MSI or ACPI)");
	return PCI_LOST_IRQ_NO_INFORMATION;
}
EXPORT_SYMBOL(pci_lost_interrupt);
