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
 * Common PowerQUICC II code.
 *
 * Author: Scott Wood <scottwood@freescale.com>
 * Copyright (c) 2007 Freescale Semiconductor
 *
 * Based on code by Vitaly Bordug <vbordug@ru.mvista.com>
 * pq2_restart fix by Wade Farnsworth <wfarnsworth@mvista.com>
 * Copyright (c) 2006 MontaVista Software, Inc.
 *
 * This program is free software; you can redistribute  it and/or modify it
 * under  the terms of  the GNU General  Public License as published by the
 * Free Software Foundation;  either version 2 of the  License, or (at your
 * option) any later version.
 */

#include <asm/cpm2.h>
#include <asm/io.h>
#include <asm/pci-bridge.h>
#include <asm/system.h>

#include <platforms/82xx/pq2.h>

#define RMR_CSRE 0x00000001

void pq2_restart(char *cmd)
{
	local_irq_disable();
	setbits32(&cpm2_immr->im_clkrst.car_rmr, RMR_CSRE);

	/* Clear the ME,EE,IR & DR bits in MSR to cause checkstop */
	mtmsr(mfmsr() & ~(MSR_ME | MSR_EE | MSR_IR | MSR_DR));
	in_8(&cpm2_immr->im_clkrst.res[0]);

	panic("Restart failed\n");
}

#ifdef CONFIG_PCI
static int pq2_pci_exclude_device(struct pci_controller *hose,
                                  u_char bus, u8 devfn)
{
	if (bus == 0 && PCI_SLOT(devfn) == 0)
		return PCIBIOS_DEVICE_NOT_FOUND;
	else
		return PCIBIOS_SUCCESSFUL;
}

static void __init pq2_pci_add_bridge(struct device_node *np)
{
	struct pci_controller *hose;
	struct resource r;

	if (of_address_to_resource(np, 0, &r) || r.end - r.start < 0x10b)
		goto err;

	ppc_pci_add_flags(PPC_PCI_REASSIGN_ALL_BUS);

	hose = pcibios_alloc_controller(np);
	if (!hose)
		return;

	hose->dn = np;

	setup_indirect_pci(hose, r.start + 0x100, r.start + 0x104, 0);
	pci_process_bridge_OF_ranges(hose, np, 1);

	return;

err:
	printk(KERN_ERR "No valid PCI reg property in device tree\n");
}

void __init pq2_init_pci(void)
{
	struct device_node *np = NULL;

	ppc_md.pci_exclude_device = pq2_pci_exclude_device;

	while ((np = of_find_compatible_node(np, NULL, "fsl,pq2-pci")))
		pq2_pci_add_bridge(np);
}
#endif
