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
 * linux/arch/arm/mach-footbridge/netwinder-pci.c
 *
 * PCI bios-type initialisation for PCI machines
 *
 * Bits taken from various places.
 */
#include <linux/kernel.h>
#include <linux/pci.h>
#include <linux/init.h>

#include <asm/irq.h>
#include <asm/mach/pci.h>
#include <asm/mach-types.h>

/*
 * We now use the slot ID instead of the device identifiers to select
 * which interrupt is routed where.
 */
static int __init netwinder_map_irq(struct pci_dev *dev, u8 slot, u8 pin)
{
	switch (slot) {
	case 0:  /* host bridge */
		return 0;

	case 9:  /* CyberPro */
		return IRQ_NETWINDER_VGA;

	case 10: /* DC21143 */
		return IRQ_NETWINDER_ETHER100;

	case 12: /* Winbond 553 */
		return IRQ_ISA_HARDDISK1;

	case 13: /* Winbond 89C940F */
		return IRQ_NETWINDER_ETHER10;

	default:
		printk(KERN_ERR "PCI: unknown device in slot %s\n",
			pci_name(dev));
		return 0;
	}
}

static struct hw_pci netwinder_pci __initdata = {
	.swizzle		= pci_std_swizzle,
	.map_irq		= netwinder_map_irq,
	.nr_controllers		= 1,
	.setup			= dc21285_setup,
	.scan			= dc21285_scan_bus,
	.preinit		= dc21285_preinit,
	.postinit		= dc21285_postinit,
};

static int __init netwinder_pci_init(void)
{
	if (machine_is_netwinder())
		pci_common_init(&netwinder_pci);
	return 0;
}

subsys_initcall(netwinder_pci_init);
