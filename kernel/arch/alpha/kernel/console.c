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
 *	linux/arch/alpha/kernel/console.c
 *
 * Architecture-specific specific support for VGA device on 
 * non-0 I/O hose
 */

#include <linux/pci.h>
#include <linux/init.h>
#include <linux/tty.h>
#include <linux/console.h>
#include <linux/vt.h>
#include <asm/vga.h>
#include <asm/machvec.h>

#include "pci_impl.h"

#ifdef CONFIG_VGA_HOSE

struct pci_controller *pci_vga_hose;
static struct resource alpha_vga = {
	.name	= "alpha-vga+",
	.start	= 0x3C0,
	.end	= 0x3DF
};

static struct pci_controller * __init 
default_vga_hose_select(struct pci_controller *h1, struct pci_controller *h2)
{
	if (h2->index < h1->index)
		return h2;

	return h1;
}

void __init 
locate_and_init_vga(void *(*sel_func)(void *, void *))
{
	struct pci_controller *hose = NULL;
	struct pci_dev *dev = NULL;

	/* Default the select function */
	if (!sel_func) sel_func = (void *)default_vga_hose_select;

	/* Find the console VGA device */
	for(dev=NULL; (dev=pci_get_class(PCI_CLASS_DISPLAY_VGA << 8, dev));) {
		if (!hose)
			hose = dev->sysdata;
		else
			hose = sel_func(hose, dev->sysdata);
	}

	/* Did we already initialize the correct one? Is there one? */
	if (!hose || (conswitchp == &vga_con && pci_vga_hose == hose))
		return;

	/* Create a new VGA ioport resource WRT the hose it is on. */
	alpha_vga.start += hose->io_space->start;
	alpha_vga.end += hose->io_space->start;
	request_resource(hose->io_space, &alpha_vga);

	/* Set the VGA hose and init the new console. */
	pci_vga_hose = hose;
	take_over_console(&vga_con, 0, MAX_NR_CONSOLES-1, 1);
}

void __init
find_console_vga_hose(void)
{
	u64 *pu64 = (u64 *)((u64)hwrpb + hwrpb->ctbt_offset);

	if (pu64[7] == 3) {	/* TERM_TYPE == graphics */
		struct pci_controller *hose;
		int h = (pu64[30] >> 24) & 0xff;	/* console hose # */

		/*
		 * Our hose numbering DOES match the console's, so find
		 * the right one...
		 */
		for (hose = hose_head; hose; hose = hose->next) {
			if (hose->index == h) break;
		}

		if (hose) {
			printk("Console graphics on hose %d\n", h);
			pci_vga_hose = hose;
		}
	}
}

#endif
