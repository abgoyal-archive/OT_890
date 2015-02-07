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
 *  drivers/mtd/nand/spia.c
 *
 *  Copyright (C) 2000 Steven J. Hill (sjhill@realitydiluted.com)
 *
 *
 *	10-29-2001 TG	change to support hardwarespecific access
 *			to controllines	(due to change in nand.c)
 *			page_cache added
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 *  Overview:
 *   This is a device driver for the NAND flash device found on the
 *   SPIA board which utilizes the Toshiba TC58V64AFT part. This is
 *   a 64Mibit (8MiB x 8 bits) NAND flash device.
 */

#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/slab.h>
#include <linux/module.h>
#include <linux/mtd/mtd.h>
#include <linux/mtd/nand.h>
#include <linux/mtd/partitions.h>
#include <asm/io.h>

/*
 * MTD structure for SPIA board
 */
static struct mtd_info *spia_mtd = NULL;

/*
 * Values specific to the SPIA board (used with EP7212 processor)
 */
#define SPIA_IO_BASE	0xd0000000	/* Start of EP7212 IO address space */
#define SPIA_FIO_BASE	0xf0000000	/* Address where flash is mapped */
#define SPIA_PEDR	0x0080	/*
				 * IO offset to Port E data register
				 * where the CLE, ALE and NCE pins
				 * are wired to.
				 */
#define SPIA_PEDDR	0x00c0	/*
				 * IO offset to Port E data direction
				 * register so we can control the IO
				 * lines.
				 */

/*
 * Module stuff
 */

static int spia_io_base = SPIA_IO_BASE;
static int spia_fio_base = SPIA_FIO_BASE;
static int spia_pedr = SPIA_PEDR;
static int spia_peddr = SPIA_PEDDR;

module_param(spia_io_base, int, 0);
module_param(spia_fio_base, int, 0);
module_param(spia_pedr, int, 0);
module_param(spia_peddr, int, 0);

/*
 * Define partitions for flash device
 */
static const struct mtd_partition partition_info[] = {
	{
	 .name = "SPIA flash partition 1",
	 .offset = 0,
	 .size = 2 * 1024 * 1024},
	{
	 .name = "SPIA flash partition 2",
	 .offset = 2 * 1024 * 1024,
	 .size = 6 * 1024 * 1024}
};

#define NUM_PARTITIONS 2

/*
 *	hardware specific access to control-lines
 *
 *	ctrl:
 *	NAND_CNE: bit 0 -> bit 2
 *	NAND_CLE: bit 1 -> bit 0
 *	NAND_ALE: bit 2 -> bit 1
 */
static void spia_hwcontrol(struct mtd_info *mtd, int cmd)
{
	struct nand_chip *chip = mtd->priv;

	if (ctrl & NAND_CTRL_CHANGE) {
		void __iomem *addr = spia_io_base + spia_pedr;
		unsigned char bits;

		bits = (ctrl & NAND_CNE) << 2;
		bits |= (ctrl & NAND_CLE | NAND_ALE) >> 1;
		writeb((readb(addr) & ~0x7) | bits, addr);
	}

	if (cmd != NAND_CMD_NONE)
		writeb(cmd, chip->IO_ADDR_W);
}

/*
 * Main initialization routine
 */
static int __init spia_init(void)
{
	struct nand_chip *this;

	/* Allocate memory for MTD device structure and private data */
	spia_mtd = kmalloc(sizeof(struct mtd_info) + sizeof(struct nand_chip), GFP_KERNEL);
	if (!spia_mtd) {
		printk("Unable to allocate SPIA NAND MTD device structure.\n");
		return -ENOMEM;
	}

	/* Get pointer to private data */
	this = (struct nand_chip *)(&spia_mtd[1]);

	/* Initialize structures */
	memset(spia_mtd, 0, sizeof(struct mtd_info));
	memset(this, 0, sizeof(struct nand_chip));

	/* Link the private data with the MTD structure */
	spia_mtd->priv = this;
	spia_mtd->owner = THIS_MODULE;

	/*
	 * Set GPIO Port E control register so that the pins are configured
	 * to be outputs for controlling the NAND flash.
	 */
	(*(volatile unsigned char *)(spia_io_base + spia_peddr)) = 0x07;

	/* Set address of NAND IO lines */
	this->IO_ADDR_R = (void __iomem *)spia_fio_base;
	this->IO_ADDR_W = (void __iomem *)spia_fio_base;
	/* Set address of hardware control function */
	this->cmd_ctrl = spia_hwcontrol;
	/* 15 us command delay time */
	this->chip_delay = 15;

	/* Scan to find existence of the device */
	if (nand_scan(spia_mtd, 1)) {
		kfree(spia_mtd);
		return -ENXIO;
	}

	/* Register the partitions */
	add_mtd_partitions(spia_mtd, partition_info, NUM_PARTITIONS);

	/* Return happy */
	return 0;
}

module_init(spia_init);

/*
 * Clean up routine
 */
static void __exit spia_cleanup(void)
{
	/* Release resources, unregister device */
	nand_release(spia_mtd);

	/* Free the MTD device structure */
	kfree(spia_mtd);
}

module_exit(spia_cleanup);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Steven J. Hill <sjhill@realitydiluted.com");
MODULE_DESCRIPTION("Board-specific glue layer for NAND flash on SPIA board");
