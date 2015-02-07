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
 *  Atari Falcon IDE Driver
 *
 *     Created 12 Jul 1997 by Geert Uytterhoeven
 *
 *  This file is subject to the terms and conditions of the GNU General Public
 *  License.  See the file COPYING in the main directory of this archive for
 *  more details.
 */

#include <linux/module.h>
#include <linux/types.h>
#include <linux/mm.h>
#include <linux/interrupt.h>
#include <linux/blkdev.h>
#include <linux/ide.h>
#include <linux/init.h>

#include <asm/setup.h>
#include <asm/atarihw.h>
#include <asm/atariints.h>
#include <asm/atari_stdma.h>

#define DRV_NAME "falconide"

    /*
     *  Base of the IDE interface
     */

#define ATA_HD_BASE	0xfff00000

    /*
     *  Offsets from the above base
     */

#define ATA_HD_CONTROL	0x39

    /*
     *  falconide_intr_lock is used to obtain access to the IDE interrupt,
     *  which is shared between several drivers.
     */

int falconide_intr_lock;
EXPORT_SYMBOL(falconide_intr_lock);

static void falconide_input_data(ide_drive_t *drive, struct request *rq,
				 void *buf, unsigned int len)
{
	unsigned long data_addr = drive->hwif->io_ports.data_addr;

	if (drive->media == ide_disk && rq && rq->cmd_type == REQ_TYPE_FS)
		return insw(data_addr, buf, (len + 1) / 2);

	insw_swapw(data_addr, buf, (len + 1) / 2);
}

static void falconide_output_data(ide_drive_t *drive, struct request *rq,
				  void *buf, unsigned int len)
{
	unsigned long data_addr = drive->hwif->io_ports.data_addr;

	if (drive->media == ide_disk && rq && rq->cmd_type == REQ_TYPE_FS)
		return outsw(data_addr, buf, (len + 1) / 2);

	outsw_swapw(data_addr, buf, (len + 1) / 2);
}

/* Atari has a byte-swapped IDE interface */
static const struct ide_tp_ops falconide_tp_ops = {
	.exec_command		= ide_exec_command,
	.read_status		= ide_read_status,
	.read_altstatus		= ide_read_altstatus,

	.set_irq		= ide_set_irq,

	.tf_load		= ide_tf_load,
	.tf_read		= ide_tf_read,

	.input_data		= falconide_input_data,
	.output_data		= falconide_output_data,
};

static const struct ide_port_info falconide_port_info = {
	.tp_ops			= &falconide_tp_ops,
	.host_flags		= IDE_HFLAG_NO_DMA | IDE_HFLAG_SERIALIZE,
};

static void __init falconide_setup_ports(hw_regs_t *hw)
{
	int i;

	memset(hw, 0, sizeof(*hw));

	hw->io_ports.data_addr = ATA_HD_BASE;

	for (i = 1; i < 8; i++)
		hw->io_ports_array[i] = ATA_HD_BASE + 1 + i * 4;

	hw->io_ports.ctl_addr = ATA_HD_BASE + ATA_HD_CONTROL;

	hw->irq = IRQ_MFP_IDE;
	hw->ack_intr = NULL;

	hw->chipset = ide_generic;
}

    /*
     *  Probe for a Falcon IDE interface
     */

static int __init falconide_init(void)
{
	struct ide_host *host;
	hw_regs_t hw, *hws[] = { &hw, NULL, NULL, NULL };
	int rc;

	if (!MACH_IS_ATARI || !ATARIHW_PRESENT(IDE))
		return -ENODEV;

	printk(KERN_INFO "ide: Falcon IDE controller\n");

	if (!request_mem_region(ATA_HD_BASE, 0x40, DRV_NAME)) {
		printk(KERN_ERR "%s: resources busy\n", DRV_NAME);
		return -EBUSY;
	}

	falconide_setup_ports(&hw);

	host = ide_host_alloc(&falconide_port_info, hws);
	if (host == NULL) {
		rc = -ENOMEM;
		goto err;
	}

	ide_get_lock(NULL, NULL);
	rc = ide_host_register(host, &falconide_port_info, hws);
	ide_release_lock();

	if (rc)
		goto err_free;

	return 0;
err_free:
	ide_host_free(host);
err:
	release_mem_region(ATA_HD_BASE, 0x40);
	return rc;
}

module_init(falconide_init);

MODULE_LICENSE("GPL");
