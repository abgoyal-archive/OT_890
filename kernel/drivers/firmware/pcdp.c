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
 * Parse the EFI PCDP table to locate the console device.
 *
 * (c) Copyright 2002, 2003, 2004 Hewlett-Packard Development Company, L.P.
 *	Khalid Aziz <khalid.aziz@hp.com>
 *	Alex Williamson <alex.williamson@hp.com>
 *	Bjorn Helgaas <bjorn.helgaas@hp.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#include <linux/acpi.h>
#include <linux/console.h>
#include <linux/efi.h>
#include <linux/serial.h>
#include <linux/serial_8250.h>
#include <asm/vga.h>
#include "pcdp.h"

static int __init
setup_serial_console(struct pcdp_uart *uart)
{
#ifdef CONFIG_SERIAL_8250_CONSOLE
	int mmio;
	static char options[64], *p = options;
	char parity;

	mmio = (uart->addr.space_id == ACPI_ADR_SPACE_SYSTEM_MEMORY);
	p += sprintf(p, "uart8250,%s,0x%lx",
		mmio ? "mmio" : "io", uart->addr.address);
	if (uart->baud) {
		p += sprintf(p, ",%lu", uart->baud);
		if (uart->bits) {
			switch (uart->parity) {
			    case 0x2: parity = 'e'; break;
			    case 0x3: parity = 'o'; break;
			    default:  parity = 'n';
			}
			p += sprintf(p, "%c%d", parity, uart->bits);
		}
	}

	add_preferred_console("uart", 8250, &options[9]);
	return setup_early_serial8250_console(options);
#else
	return -ENODEV;
#endif
}

static int __init
setup_vga_console(struct pcdp_device *dev)
{
#if defined(CONFIG_VT) && defined(CONFIG_VGA_CONSOLE)
	u8 *if_ptr;

	if_ptr = ((u8 *)dev + sizeof(struct pcdp_device));
	if (if_ptr[0] == PCDP_IF_PCI) {
		struct pcdp_if_pci if_pci;

		/* struct copy since ifptr might not be correctly aligned */

		memcpy(&if_pci, if_ptr, sizeof(if_pci));

		if (if_pci.trans & PCDP_PCI_TRANS_IOPORT)
			vga_console_iobase = if_pci.ioport_tra;

		if (if_pci.trans & PCDP_PCI_TRANS_MMIO)
			vga_console_membase = if_pci.mmio_tra;
	}

	if (efi_mem_type(vga_console_membase + 0xA0000) == EFI_CONVENTIONAL_MEMORY) {
		printk(KERN_ERR "PCDP: VGA selected, but frame buffer is not MMIO!\n");
		return -ENODEV;
	}

	conswitchp = &vga_con;
	printk(KERN_INFO "PCDP: VGA console\n");
	return 0;
#else
	return -ENODEV;
#endif
}

int __init
efi_setup_pcdp_console(char *cmdline)
{
	struct pcdp *pcdp;
	struct pcdp_uart *uart;
	struct pcdp_device *dev, *end;
	int i, serial = 0;
	int rc = -ENODEV;

	if (efi.hcdp == EFI_INVALID_TABLE_ADDR)
		return -ENODEV;

	pcdp = ioremap(efi.hcdp, 4096);
	printk(KERN_INFO "PCDP: v%d at 0x%lx\n", pcdp->rev, efi.hcdp);

	if (strstr(cmdline, "console=hcdp")) {
		if (pcdp->rev < 3)
			serial = 1;
	} else if (strstr(cmdline, "console=")) {
		printk(KERN_INFO "Explicit \"console=\"; ignoring PCDP\n");
		goto out;
	}

	if (pcdp->rev < 3 && efi_uart_console_only())
		serial = 1;

	for (i = 0, uart = pcdp->uart; i < pcdp->num_uarts; i++, uart++) {
		if (uart->flags & PCDP_UART_PRIMARY_CONSOLE || serial) {
			if (uart->type == PCDP_CONSOLE_UART) {
				rc = setup_serial_console(uart);
				goto out;
			}
		}
	}

	end = (struct pcdp_device *) ((u8 *) pcdp + pcdp->length);
	for (dev = (struct pcdp_device *) (pcdp->uart + pcdp->num_uarts);
	     dev < end;
	     dev = (struct pcdp_device *) ((u8 *) dev + dev->length)) {
		if (dev->flags & PCDP_PRIMARY_CONSOLE) {
			if (dev->type == PCDP_CONSOLE_VGA) {
				rc = setup_vga_console(dev);
				goto out;
			}
		}
	}

out:
	iounmap(pcdp);
	return rc;
}
