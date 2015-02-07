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
 * include/asm-xtensa/pci-bridge.h
 *
 * This file is subject to the terms and conditions of the GNU General
 * Public License.  See the file "COPYING" in the main directory of
 * this archive for more details.
 *
 * Copyright (C) 2005 Tensilica Inc.
 */

#ifndef _XTENSA_PCI_BRIDGE_H
#define _XTENSA_PCI_BRIDGE_H

#ifdef __KERNEL__

struct device_node;
struct pci_controller;

/*
 * pciauto_bus_scan() enumerates the pci space.
 */

extern int pciauto_bus_scan(struct pci_controller *, int);

struct pci_space {
	unsigned long start;
	unsigned long end;
	unsigned long base;
};

/*
 * Structure of a PCI controller (host bridge)
 */

struct pci_controller {
	int index;			/* used for pci_controller_num */
	struct pci_controller *next;
        struct pci_bus *bus;
	void *arch_data;

	int first_busno;
	int last_busno;

	struct pci_ops *ops;
	volatile unsigned int *cfg_addr;
	volatile unsigned char *cfg_data;

	/* Currently, we limit ourselves to 1 IO range and 3 mem
	 * ranges since the common pci_bus structure can't handle more
	 */
	struct resource	io_resource;
	struct resource mem_resources[3];
	int mem_resource_count;

	/* Host bridge I/O and Memory space
	 * Used for BAR placement algorithms
	 */
	struct pci_space io_space;
	struct pci_space mem_space;

	/* Return the interrupt number fo a device. */
	int (*map_irq)(struct pci_dev*, u8, u8);

};

static inline void pcibios_init_resource(struct resource *res,
		unsigned long start, unsigned long end, int flags, char *name)
{
	res->start = start;
	res->end = end;
	res->flags = flags;
	res->name = name;
	res->parent = NULL;
	res->sibling = NULL;
	res->child = NULL;
}


/* These are used for config access before all the PCI probing has been done. */
int early_read_config_byte(struct pci_controller*, int, int, int, u8*);
int early_read_config_word(struct pci_controller*, int, int, int, u16*);
int early_read_config_dword(struct pci_controller*, int, int, int, u32*);
int early_write_config_byte(struct pci_controller*, int, int, int, u8);
int early_write_config_word(struct pci_controller*, int, int, int, u16);
int early_write_config_dword(struct pci_controller*, int, int, int, u32);

#endif	/* __KERNEL__ */
#endif	/* _XTENSA_PCI_BRIDGE_H */
