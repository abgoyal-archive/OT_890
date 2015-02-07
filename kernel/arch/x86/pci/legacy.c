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
 * legacy.c - traditional, old school PCI bus probing
 */
#include <linux/init.h>
#include <linux/pci.h>
#include <asm/pci_x86.h>

/*
 * Discover remaining PCI buses in case there are peer host bridges.
 * We use the number of last PCI bus provided by the PCI BIOS.
 */
static void __devinit pcibios_fixup_peer_bridges(void)
{
	int n, devfn;
	long node;

	if (pcibios_last_bus <= 0 || pcibios_last_bus > 0xff)
		return;
	DBG("PCI: Peer bridge fixup\n");

	for (n=0; n <= pcibios_last_bus; n++) {
		u32 l;
		if (pci_find_bus(0, n))
			continue;
		node = get_mp_bus_to_node(n);
		for (devfn = 0; devfn < 256; devfn += 8) {
			if (!raw_pci_read(0, n, devfn, PCI_VENDOR_ID, 2, &l) &&
			    l != 0x0000 && l != 0xffff) {
				DBG("Found device at %02x:%02x [%04x]\n", n, devfn, l);
				printk(KERN_INFO "PCI: Discovered peer bus %02x\n", n);
				pci_scan_bus_on_node(n, &pci_root_ops, node);
				break;
			}
		}
	}
}

static int __init pci_legacy_init(void)
{
	if (!raw_pci_ops) {
		printk("PCI: System does not support PCI\n");
		return 0;
	}

	if (pcibios_scanned++)
		return 0;

	printk("PCI: Probing PCI hardware\n");
	pci_root_bus = pcibios_scan_root(0);
	if (pci_root_bus)
		pci_bus_add_devices(pci_root_bus);

	pcibios_fixup_peer_bridges();

	return 0;
}

int __init pci_subsys_init(void)
{
#ifdef CONFIG_X86_NUMAQ
	pci_numaq_init();
#endif
#ifdef CONFIG_ACPI
	pci_acpi_init();
#endif
#ifdef CONFIG_X86_VISWS
	pci_visws_init();
#endif
	pci_legacy_init();
	pcibios_irq_init();
	pcibios_init();

	return 0;
}
subsys_initcall(pci_subsys_init);
