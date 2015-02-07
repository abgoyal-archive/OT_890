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
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 *
 * Copyright (C) 2004 by Ralf Baechle (ralf@linux-mips.org)
 */
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/pci.h>
#include <asm/titan_dep.h>

extern struct pci_ops titan_pci_ops;

static struct resource py_mem_resource = {
	.start	= 0xe0000000UL,
	.end	= 0xe3ffffffUL,
	.name	= "Titan PCI MEM",
	.flags	= IORESOURCE_MEM
};

/*
 * PMON really reserves 16MB of I/O port space but that's stupid, nothing
 * needs that much since allocations are limited to 256 bytes per device
 * anyway.  So we just claim 64kB here.
 */
#define TITAN_IO_SIZE	0x0000ffffUL
#define TITAN_IO_BASE	0xe8000000UL

static struct resource py_io_resource = {
	.start	= 0x00001000UL,
	.end	= TITAN_IO_SIZE - 1,
	.name	= "Titan IO MEM",
	.flags	= IORESOURCE_IO,
};

static struct pci_controller py_controller = {
	.pci_ops	= &titan_pci_ops,
	.mem_resource	= &py_mem_resource,
	.mem_offset	= 0x00000000UL,
	.io_resource	= &py_io_resource,
	.io_offset	= 0x00000000UL
};

static char ioremap_failed[] __initdata = "Could not ioremap I/O port range";

static int __init pmc_yosemite_setup(void)
{
	unsigned long io_v_base;

	io_v_base = (unsigned long) ioremap(TITAN_IO_BASE, TITAN_IO_SIZE);
	if (!io_v_base)
		panic(ioremap_failed);

	set_io_port_base(io_v_base);
	TITAN_WRITE(RM9000x2_OCD_LKM7, TITAN_READ(RM9000x2_OCD_LKM7) | 1);

	ioport_resource.end = TITAN_IO_SIZE - 1;

	register_pci_controller(&py_controller);

	return 0;
}

arch_initcall(pmc_yosemite_setup);
