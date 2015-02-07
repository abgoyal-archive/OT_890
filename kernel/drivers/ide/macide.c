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
 *  Macintosh IDE Driver
 *
 *     Copyright (C) 1998 by Michael Schmitz
 *
 *  This driver was written based on information obtained from the MacOS IDE
 *  driver binary by Mikael Forselius
 *
 *  This file is subject to the terms and conditions of the GNU General Public
 *  License.  See the file COPYING in the main directory of this archive for
 *  more details.
 */

#include <linux/types.h>
#include <linux/mm.h>
#include <linux/interrupt.h>
#include <linux/blkdev.h>
#include <linux/delay.h>
#include <linux/ide.h>

#include <asm/macintosh.h>
#include <asm/macints.h>
#include <asm/mac_baboon.h>

#define IDE_BASE 0x50F1A000	/* Base address of IDE controller */

/*
 * Generic IDE registers as offsets from the base
 * These match MkLinux so they should be correct.
 */

#define IDE_CONTROL	0x38	/* control/altstatus */

/*
 * Mac-specific registers
 */

/*
 * this register is odd; it doesn't seem to do much and it's
 * not word-aligned like virtually every other hardware register
 * on the Mac...
 */

#define IDE_IFR		0x101	/* (0x101) IDE interrupt flags on Quadra:
				 *
				 * Bit 0+1: some interrupt flags
				 * Bit 2+3: some interrupt enable
				 * Bit 4:   ??
				 * Bit 5:   IDE interrupt flag (any hwif)
				 * Bit 6:   maybe IDE interrupt enable (any hwif) ??
				 * Bit 7:   Any interrupt condition
				 */

volatile unsigned char *ide_ifr = (unsigned char *) (IDE_BASE + IDE_IFR);

int macide_ack_intr(ide_hwif_t* hwif)
{
	if (*ide_ifr & 0x20) {
		*ide_ifr &= ~0x20;
		return 1;
	}
	return 0;
}

static void __init macide_setup_ports(hw_regs_t *hw, unsigned long base,
				      int irq, ide_ack_intr_t *ack_intr)
{
	int i;

	memset(hw, 0, sizeof(*hw));

	for (i = 0; i < 8; i++)
		hw->io_ports_array[i] = base + i * 4;

	hw->io_ports.ctl_addr = base + IDE_CONTROL;

	hw->irq = irq;
	hw->ack_intr = ack_intr;

	hw->chipset = ide_generic;
}

static const char *mac_ide_name[] =
	{ "Quadra", "Powerbook", "Powerbook Baboon" };

/*
 * Probe for a Macintosh IDE interface
 */

static int __init macide_init(void)
{
	ide_ack_intr_t *ack_intr;
	unsigned long base;
	int irq;
	hw_regs_t hw, *hws[] = { &hw, NULL, NULL, NULL };

	if (!MACH_IS_MAC)
		return -ENODEV;

	switch (macintosh_config->ide_type) {
	case MAC_IDE_QUADRA:
		base = IDE_BASE;
		ack_intr = macide_ack_intr;
		irq = IRQ_NUBUS_F;
		break;
	case MAC_IDE_PB:
		base = IDE_BASE;
		ack_intr = macide_ack_intr;
		irq = IRQ_NUBUS_C;
		break;
	case MAC_IDE_BABOON:
		base = BABOON_BASE;
		ack_intr = NULL;
		irq = IRQ_BABOON_1;
		break;
	default:
		return -ENODEV;
	}

	printk(KERN_INFO "ide: Macintosh %s IDE controller\n",
			 mac_ide_name[macintosh_config->ide_type - 1]);

	macide_setup_ports(&hw, base, irq, ack_intr);

	return ide_host_add(NULL, hws, NULL);
}

module_init(macide_init);

MODULE_LICENSE("GPL");
