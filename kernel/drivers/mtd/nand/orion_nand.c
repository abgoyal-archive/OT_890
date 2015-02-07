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
 * drivers/mtd/nand/orion_nand.c
 *
 * NAND support for Marvell Orion SoC platforms
 *
 * Tzachi Perelstein <tzachi@marvell.com>
 *
 * This file is licensed under  the terms of the GNU General Public
 * License version 2. This program is licensed "as is" without any
 * warranty of any kind, whether express or implied.
 */

#include <linux/slab.h>
#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/mtd/mtd.h>
#include <linux/mtd/nand.h>
#include <linux/mtd/partitions.h>
#include <asm/io.h>
#include <asm/sizes.h>
#include <mach/hardware.h>
#include <plat/orion_nand.h>

#ifdef CONFIG_MTD_CMDLINE_PARTS
static const char *part_probes[] = { "cmdlinepart", NULL };
#endif

static void orion_nand_cmd_ctrl(struct mtd_info *mtd, int cmd, unsigned int ctrl)
{
	struct nand_chip *nc = mtd->priv;
	struct orion_nand_data *board = nc->priv;
	u32 offs;

	if (cmd == NAND_CMD_NONE)
		return;

	if (ctrl & NAND_CLE)
		offs = (1 << board->cle);
	else if (ctrl & NAND_ALE)
		offs = (1 << board->ale);
	else
		return;

	if (nc->options & NAND_BUSWIDTH_16)
		offs <<= 1;

	writeb(cmd, nc->IO_ADDR_W + offs);
}

static int __init orion_nand_probe(struct platform_device *pdev)
{
	struct mtd_info *mtd;
	struct nand_chip *nc;
	struct orion_nand_data *board;
	void __iomem *io_base;
	int ret = 0;
#ifdef CONFIG_MTD_PARTITIONS
	struct mtd_partition *partitions = NULL;
	int num_part = 0;
#endif

	nc = kzalloc(sizeof(struct nand_chip) + sizeof(struct mtd_info), GFP_KERNEL);
	if (!nc) {
		printk(KERN_ERR "orion_nand: failed to allocate device structure.\n");
		ret = -ENOMEM;
		goto no_res;
	}
	mtd = (struct mtd_info *)(nc + 1);

	io_base = ioremap(pdev->resource[0].start,
			pdev->resource[0].end - pdev->resource[0].start + 1);
	if (!io_base) {
		printk(KERN_ERR "orion_nand: ioremap failed\n");
		ret = -EIO;
		goto no_res;
	}

	board = pdev->dev.platform_data;

	mtd->priv = nc;
	mtd->owner = THIS_MODULE;

	nc->priv = board;
	nc->IO_ADDR_R = nc->IO_ADDR_W = io_base;
	nc->cmd_ctrl = orion_nand_cmd_ctrl;
	nc->ecc.mode = NAND_ECC_SOFT;

	if (board->chip_delay)
		nc->chip_delay = board->chip_delay;

	if (board->width == 16)
		nc->options |= NAND_BUSWIDTH_16;

	platform_set_drvdata(pdev, mtd);

	if (nand_scan(mtd, 1)) {
		ret = -ENXIO;
		goto no_dev;
	}

#ifdef CONFIG_MTD_PARTITIONS
#ifdef CONFIG_MTD_CMDLINE_PARTS
	mtd->name = "orion_nand";
	num_part = parse_mtd_partitions(mtd, part_probes, &partitions, 0);
#endif
	/* If cmdline partitions have been passed, let them be used */
	if (num_part <= 0) {
		num_part = board->nr_parts;
		partitions = board->parts;
	}

	if (partitions && num_part > 0)
		ret = add_mtd_partitions(mtd, partitions, num_part);
	else
		ret = add_mtd_device(mtd);
#else
	ret = add_mtd_device(mtd);
#endif

	if (ret) {
		nand_release(mtd);
		goto no_dev;
	}

	return 0;

no_dev:
	platform_set_drvdata(pdev, NULL);
	iounmap(io_base);
no_res:
	kfree(nc);

	return ret;
}

static int __devexit orion_nand_remove(struct platform_device *pdev)
{
	struct mtd_info *mtd = platform_get_drvdata(pdev);
	struct nand_chip *nc = mtd->priv;

	nand_release(mtd);

	iounmap(nc->IO_ADDR_W);

	kfree(nc);

	return 0;
}

static struct platform_driver orion_nand_driver = {
	.probe		= orion_nand_probe,
	.remove		= __devexit_p(orion_nand_remove),
	.driver		= {
		.name	= "orion_nand",
		.owner	= THIS_MODULE,
	},
};

static int __init orion_nand_init(void)
{
	return platform_driver_register(&orion_nand_driver);
}

static void __exit orion_nand_exit(void)
{
	platform_driver_unregister(&orion_nand_driver);
}

module_init(orion_nand_init);
module_exit(orion_nand_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Tzachi Perelstein");
MODULE_DESCRIPTION("NAND glue for Orion platforms");
MODULE_ALIAS("platform:orion_nand");
