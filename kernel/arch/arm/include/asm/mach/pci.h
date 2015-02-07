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
 *  arch/arm/include/asm/mach/pci.h
 *
 *  Copyright (C) 2000 Russell King
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

struct pci_sys_data;
struct pci_bus;

struct hw_pci {
	struct list_head buses;
	int		nr_controllers;
	int		(*setup)(int nr, struct pci_sys_data *);
	struct pci_bus *(*scan)(int nr, struct pci_sys_data *);
	void		(*preinit)(void);
	void		(*postinit)(void);
	u8		(*swizzle)(struct pci_dev *dev, u8 *pin);
	int		(*map_irq)(struct pci_dev *dev, u8 slot, u8 pin);
};

/*
 * Per-controller structure
 */
struct pci_sys_data {
	struct list_head node;
	int		busnr;		/* primary bus number			*/
	u64		mem_offset;	/* bus->cpu memory mapping offset	*/
	unsigned long	io_offset;	/* bus->cpu IO mapping offset		*/
	struct pci_bus	*bus;		/* PCI bus				*/
	struct resource *resource[3];	/* Primary PCI bus resources		*/
					/* Bridge swizzling			*/
	u8		(*swizzle)(struct pci_dev *, u8 *);
					/* IRQ mapping				*/
	int		(*map_irq)(struct pci_dev *, u8, u8);
	struct hw_pci	*hw;
};

/*
 * This is the standard PCI-PCI bridge swizzling algorithm.
 */
#define pci_std_swizzle pci_common_swizzle

/*
 * Call this with your hw_pci struct to initialise the PCI system.
 */
void pci_common_init(struct hw_pci *);

/*
 * PCI controllers
 */
extern int iop3xx_pci_setup(int nr, struct pci_sys_data *);
extern struct pci_bus *iop3xx_pci_scan_bus(int nr, struct pci_sys_data *);
extern void iop3xx_pci_preinit(void);
extern void iop3xx_pci_preinit_cond(void);

extern int dc21285_setup(int nr, struct pci_sys_data *);
extern struct pci_bus *dc21285_scan_bus(int nr, struct pci_sys_data *);
extern void dc21285_preinit(void);
extern void dc21285_postinit(void);

extern int via82c505_setup(int nr, struct pci_sys_data *);
extern struct pci_bus *via82c505_scan_bus(int nr, struct pci_sys_data *);
extern void via82c505_init(void *sysdata);

extern int pci_v3_setup(int nr, struct pci_sys_data *);
extern struct pci_bus *pci_v3_scan_bus(int nr, struct pci_sys_data *);
extern void pci_v3_preinit(void);
extern void pci_v3_postinit(void);
