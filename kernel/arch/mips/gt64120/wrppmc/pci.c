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
 * pci.c: GT64120 PCI support.
 *
 * Copyright (C) 2006, Wind River System Inc. Rongkai.Zhan <rongkai.zhan@windriver.com>
 *
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 */
#include <linux/init.h>
#include <linux/ioport.h>
#include <linux/types.h>
#include <linux/pci.h>

#include <asm/gt64120.h>

extern struct pci_ops gt64xxx_pci0_ops;

static struct resource pci0_io_resource = {
	.name  = "pci_0 io",
	.start = GT_PCI_IO_BASE,
	.end   = GT_PCI_IO_BASE + GT_PCI_IO_SIZE - 1,
	.flags = IORESOURCE_IO,
};

static struct resource pci0_mem_resource = {
	.name  = "pci_0 memory",
	.start = GT_PCI_MEM_BASE,
	.end   = GT_PCI_MEM_BASE + GT_PCI_MEM_SIZE - 1,
	.flags = IORESOURCE_MEM,
};

static struct pci_controller hose_0 = {
	.pci_ops	= &gt64xxx_pci0_ops,
	.io_resource	= &pci0_io_resource,
	.mem_resource	= &pci0_mem_resource,
};

static int __init gt64120_pci_init(void)
{
	u32 tmp;

	tmp = GT_READ(GT_PCI0_CMD_OFS);		/* Huh??? -- Ralf  */
	tmp = GT_READ(GT_PCI0_BARE_OFS);

	/* reset the whole PCI I/O space range */
	ioport_resource.start = GT_PCI_IO_BASE;
	ioport_resource.end = GT_PCI_IO_BASE + GT_PCI_IO_SIZE - 1;

	register_pci_controller(&hose_0);
	return 0;
}

arch_initcall(gt64120_pci_init);
