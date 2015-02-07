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

#include <linux/kernel.h>
#include <linux/pci.h>
#include <linux/interrupt.h>
#include <linux/mm.h>
#include <linux/init.h>
#include <linux/ioport.h>
#include <linux/io.h>

#include <asm/system.h>

#include <asm/mach/pci.h>

#define MAX_SLOTS		7

#define CONFIG_CMD(bus, devfn, where)   (0x80000000 | (bus->number << 16) | (devfn << 8) | (where & ~3))

static int
via82c505_read_config(struct pci_bus *bus, unsigned int devfn, int where,
		      int size, u32 *value)
{
	outl(CONFIG_CMD(bus,devfn,where),0xCF8);
	switch (size) {
	case 1:
		*value=inb(0xCFC + (where&3));
		break;
	case 2:
		*value=inw(0xCFC + (where&2));
		break;
	case 4:
		*value=inl(0xCFC);
		break;
	}
	return PCIBIOS_SUCCESSFUL;
}

static int
via82c505_write_config(struct pci_bus *bus, unsigned int devfn, int where,
		       int size, u32 value)
{
	outl(CONFIG_CMD(bus,devfn,where),0xCF8);
	switch (size) {
	case 1:
		outb(value, 0xCFC + (where&3));
		break;
	case 2:
		outw(value, 0xCFC + (where&2));
		break;
	case 4:
		outl(value, 0xCFC);
		break;
	}
	return PCIBIOS_SUCCESSFUL;
}

static struct pci_ops via82c505_ops = {
	.read	= via82c505_read_config,
	.write	= via82c505_write_config,
};

void __init via82c505_preinit(void)
{
	printk(KERN_DEBUG "PCI: VIA 82c505\n");
	if (!request_region(0xA8,2,"via config")) {
		printk(KERN_WARNING"VIA 82c505: Unable to request region 0xA8\n");
		return;
	}
	if (!request_region(0xCF8,8,"pci config")) {
		printk(KERN_WARNING"VIA 82c505: Unable to request region 0xCF8\n");
		release_region(0xA8, 2);
		return;
	}

	/* Enable compatible Mode */
	outb(0x96,0xA8);
	outb(0x18,0xA9);
	outb(0x93,0xA8);
	outb(0xd0,0xA9);

}

int __init via82c505_setup(int nr, struct pci_sys_data *sys)
{
	return (nr == 0);
}

struct pci_bus * __init via82c505_scan_bus(int nr, struct pci_sys_data *sysdata)
{
	if (nr == 0)
		return pci_scan_bus(0, &via82c505_ops, sysdata);

	return NULL;
}
