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
 *  Amiga Buddha, Catweasel and X-Surf IDE Driver
 *
 *	Copyright (C) 1997, 2001 by Geert Uytterhoeven and others
 *
 *  This driver was written based on the specifications in README.buddha and
 *  the X-Surf info from Inside_XSurf.txt available at
 *  http://www.jschoenfeld.com
 *
 *  This file is subject to the terms and conditions of the GNU General Public
 *  License.  See the file COPYING in the main directory of this archive for
 *  more details.
 *
 *  TODO:
 *    - test it :-)
 *    - tune the timings using the speed-register
 */

#include <linux/types.h>
#include <linux/mm.h>
#include <linux/interrupt.h>
#include <linux/blkdev.h>
#include <linux/zorro.h>
#include <linux/ide.h>
#include <linux/init.h>

#include <asm/amigahw.h>
#include <asm/amigaints.h>


    /*
     *  The Buddha has 2 IDE interfaces, the Catweasel has 3, X-Surf has 2
     */

#define BUDDHA_NUM_HWIFS	2
#define CATWEASEL_NUM_HWIFS	3
#define XSURF_NUM_HWIFS         2

#define MAX_NUM_HWIFS		3

    /*
     *  Bases of the IDE interfaces (relative to the board address)
     */

#define BUDDHA_BASE1	0x800
#define BUDDHA_BASE2	0xa00
#define BUDDHA_BASE3	0xc00

#define XSURF_BASE1     0xb000 /* 2.5" Interface */
#define XSURF_BASE2     0xd000 /* 3.5" Interface */

static u_int buddha_bases[CATWEASEL_NUM_HWIFS] __initdata = {
    BUDDHA_BASE1, BUDDHA_BASE2, BUDDHA_BASE3
};

static u_int xsurf_bases[XSURF_NUM_HWIFS] __initdata = {
     XSURF_BASE1, XSURF_BASE2
};

    /*
     *  Offsets from one of the above bases
     */

#define BUDDHA_CONTROL	0x11a

    /*
     *  Other registers
     */

#define BUDDHA_IRQ1	0xf00		/* MSB = 1, Harddisk is source of */
#define BUDDHA_IRQ2	0xf40		/* interrupt */
#define BUDDHA_IRQ3	0xf80

#define XSURF_IRQ1      0x7e
#define XSURF_IRQ2      0x7e

static int buddha_irqports[CATWEASEL_NUM_HWIFS] __initdata = {
    BUDDHA_IRQ1, BUDDHA_IRQ2, BUDDHA_IRQ3
};

static int xsurf_irqports[XSURF_NUM_HWIFS] __initdata = {
    XSURF_IRQ1, XSURF_IRQ2
};

#define BUDDHA_IRQ_MR	0xfc0		/* master interrupt enable */


    /*
     *  Board information
     */

typedef enum BuddhaType_Enum {
    BOARD_BUDDHA, BOARD_CATWEASEL, BOARD_XSURF
} BuddhaType;

static const char *buddha_board_name[] = { "Buddha", "Catweasel", "X-Surf" };

    /*
     *  Check and acknowledge the interrupt status
     */

static int buddha_ack_intr(ide_hwif_t *hwif)
{
    unsigned char ch;

    ch = z_readb(hwif->io_ports.irq_addr);
    if (!(ch & 0x80))
	    return 0;
    return 1;
}

static int xsurf_ack_intr(ide_hwif_t *hwif)
{
    unsigned char ch;

    ch = z_readb(hwif->io_ports.irq_addr);
    /* X-Surf needs a 0 written to IRQ register to ensure ISA bit A11 stays at 0 */
    z_writeb(0, hwif->io_ports.irq_addr);
    if (!(ch & 0x80))
	    return 0;
    return 1;
}

static void __init buddha_setup_ports(hw_regs_t *hw, unsigned long base,
				      unsigned long ctl, unsigned long irq_port,
				      ide_ack_intr_t *ack_intr)
{
	int i;

	memset(hw, 0, sizeof(*hw));

	hw->io_ports.data_addr = base;

	for (i = 1; i < 8; i++)
		hw->io_ports_array[i] = base + 2 + i * 4;

	hw->io_ports.ctl_addr = ctl;
	hw->io_ports.irq_addr = irq_port;

	hw->irq = IRQ_AMIGA_PORTS;
	hw->ack_intr = ack_intr;

	hw->chipset = ide_generic;
}

    /*
     *  Probe for a Buddha or Catweasel IDE interface
     */

static int __init buddha_init(void)
{
	struct zorro_dev *z = NULL;
	u_long buddha_board = 0;
	BuddhaType type;
	int buddha_num_hwifs, i;

	while ((z = zorro_find_device(ZORRO_WILDCARD, z))) {
		unsigned long board;
		hw_regs_t hw[MAX_NUM_HWIFS], *hws[] = { NULL, NULL, NULL, NULL };

		if (z->id == ZORRO_PROD_INDIVIDUAL_COMPUTERS_BUDDHA) {
			buddha_num_hwifs = BUDDHA_NUM_HWIFS;
			type=BOARD_BUDDHA;
		} else if (z->id == ZORRO_PROD_INDIVIDUAL_COMPUTERS_CATWEASEL) {
			buddha_num_hwifs = CATWEASEL_NUM_HWIFS;
			type=BOARD_CATWEASEL;
		} else if (z->id == ZORRO_PROD_INDIVIDUAL_COMPUTERS_X_SURF) {
			buddha_num_hwifs = XSURF_NUM_HWIFS;
			type=BOARD_XSURF;
		} else 
			continue;
		
		board = z->resource.start;

/*
 * FIXME: we now have selectable mmio v/s iomio transports.
 */

		if(type != BOARD_XSURF) {
			if (!request_mem_region(board+BUDDHA_BASE1, 0x800, "IDE"))
				continue;
		} else {
			if (!request_mem_region(board+XSURF_BASE1, 0x1000, "IDE"))
				continue;
			if (!request_mem_region(board+XSURF_BASE2, 0x1000, "IDE"))
				goto fail_base2;
			if (!request_mem_region(board+XSURF_IRQ1, 0x8, "IDE")) {
				release_mem_region(board+XSURF_BASE2, 0x1000);
fail_base2:
				release_mem_region(board+XSURF_BASE1, 0x1000);
				continue;
			}
		}	  
		buddha_board = ZTWO_VADDR(board);
		
		/* write to BUDDHA_IRQ_MR to enable the board IRQ */
		/* X-Surf doesn't have this.  IRQs are always on */
		if (type != BOARD_XSURF)
			z_writeb(0, buddha_board+BUDDHA_IRQ_MR);

		printk(KERN_INFO "ide: %s IDE controller\n",
				 buddha_board_name[type]);

		for (i = 0; i < buddha_num_hwifs; i++) {
			unsigned long base, ctl, irq_port;
			ide_ack_intr_t *ack_intr;

			if (type != BOARD_XSURF) {
				base = buddha_board + buddha_bases[i];
				ctl = base + BUDDHA_CONTROL;
				irq_port = buddha_board + buddha_irqports[i];
				ack_intr = buddha_ack_intr;
			} else {
				base = buddha_board + xsurf_bases[i];
				/* X-Surf has no CS1* (Control/AltStat) */
				ctl = 0;
				irq_port = buddha_board + xsurf_irqports[i];
				ack_intr = xsurf_ack_intr;
			}

			buddha_setup_ports(&hw[i], base, ctl, irq_port,
					   ack_intr);

			hws[i] = &hw[i];
		}

		ide_host_add(NULL, hws, NULL);
	}

	return 0;
}

module_init(buddha_init);

MODULE_LICENSE("GPL");
