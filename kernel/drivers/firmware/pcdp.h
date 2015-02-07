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
 * Definitions for PCDP-defined console devices
 *
 * v1.0a: http://www.dig64.org/specifications/DIG64_HCDPv10a_01.pdf
 * v2.0:  http://www.dig64.org/specifications/DIG64_PCDPv20.pdf
 *
 * (c) Copyright 2002, 2004 Hewlett-Packard Development Company, L.P.
 *	Khalid Aziz <khalid.aziz@hp.com>
 *	Bjorn Helgaas <bjorn.helgaas@hp.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#define PCDP_CONSOLE			0
#define PCDP_DEBUG			1
#define PCDP_CONSOLE_OUTPUT		2
#define PCDP_CONSOLE_INPUT		3

#define PCDP_UART			(0 << 3)
#define PCDP_VGA			(1 << 3)
#define PCDP_USB			(2 << 3)

/* pcdp_uart.type and pcdp_device.type */
#define PCDP_CONSOLE_UART		(PCDP_UART | PCDP_CONSOLE)
#define PCDP_DEBUG_UART			(PCDP_UART | PCDP_DEBUG)
#define PCDP_CONSOLE_VGA		(PCDP_VGA  | PCDP_CONSOLE_OUTPUT)
#define PCDP_CONSOLE_USB		(PCDP_USB  | PCDP_CONSOLE_INPUT)

/* pcdp_uart.flags */
#define PCDP_UART_EDGE_SENSITIVE	(1 << 0)
#define PCDP_UART_ACTIVE_LOW		(1 << 1)
#define PCDP_UART_PRIMARY_CONSOLE	(1 << 2)
#define PCDP_UART_IRQ			(1 << 6) /* in pci_func for rev < 3 */
#define PCDP_UART_PCI			(1 << 7) /* in pci_func for rev < 3 */

struct pcdp_uart {
	u8				type;
	u8				bits;
	u8				parity;
	u8				stop_bits;
	u8				pci_seg;
	u8				pci_bus;
	u8				pci_dev;
	u8				pci_func;
	u64				baud;
	struct acpi_generic_address	addr;
	u16				pci_dev_id;
	u16				pci_vendor_id;
	u32				gsi;
	u32				clock_rate;
	u8				pci_prog_intfc;
	u8				flags;
	u16				conout_index;
	u32				reserved;
} __attribute__((packed));

#define PCDP_IF_PCI	1

/* pcdp_if_pci.trans */
#define PCDP_PCI_TRANS_IOPORT	0x02
#define PCDP_PCI_TRANS_MMIO	0x01

struct pcdp_if_pci {
	u8			interconnect;
	u8			reserved;
	u16			length;
	u8			segment;
	u8			bus;
	u8			dev;
	u8			fun;
	u16			dev_id;
	u16			vendor_id;
	u32			acpi_interrupt;
	u64			mmio_tra;
	u64			ioport_tra;
	u8			flags;
	u8			trans;
} __attribute__((packed));

struct pcdp_vga {
	u8			count;		/* address space descriptors */
} __attribute__((packed));

/* pcdp_device.flags */
#define PCDP_PRIMARY_CONSOLE	1

struct pcdp_device {
	u8			type;
	u8			flags;
	u16			length;
	u16			efi_index;
	/* next data is pcdp_if_pci or pcdp_if_acpi (not yet supported) */
	/* next data is device specific type (currently only pcdp_vga) */
} __attribute__((packed));

struct pcdp {
	u8			signature[4];
	u32			length;
	u8			rev;		/* PCDP v2.0 is rev 3 */
	u8			chksum;
	u8			oemid[6];
	u8			oem_tabid[8];
	u32			oem_rev;
	u8			creator_id[4];
	u32			creator_rev;
	u32			num_uarts;
	struct pcdp_uart	uart[0];	/* actual size is num_uarts */
	/* remainder of table is pcdp_device structures */
} __attribute__((packed));
