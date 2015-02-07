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
 * Flash and EPROM on Hitachi Solution Engine and similar boards.
 *
 * (C) 2001 Red Hat, Inc.
 *
 * GPL'd
 */

#include <linux/module.h>
#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <asm/io.h>
#include <linux/mtd/mtd.h>
#include <linux/mtd/map.h>
#include <linux/mtd/partitions.h>
#include <linux/errno.h>

static struct mtd_info *flash_mtd;
static struct mtd_info *eprom_mtd;

static struct mtd_partition *parsed_parts;

struct map_info soleng_eprom_map = {
	.name = "Solution Engine EPROM",
	.size = 0x400000,
	.bankwidth = 4,
};

struct map_info soleng_flash_map = {
	.name = "Solution Engine FLASH",
	.size = 0x400000,
	.bankwidth = 4,
};

static const char *probes[] = { "RedBoot", "cmdlinepart", NULL };

#ifdef CONFIG_MTD_SUPERH_RESERVE
static struct mtd_partition superh_se_partitions[] = {
	/* Reserved for boot code, read-only */
	{
		.name = "flash_boot",
		.offset = 0x00000000,
		.size = CONFIG_MTD_SUPERH_RESERVE,
		.mask_flags = MTD_WRITEABLE,
	},
	/* All else is writable (e.g. JFFS) */
	{
		.name = "Flash FS",
		.offset = MTDPART_OFS_NXTBLK,
		.size = MTDPART_SIZ_FULL,
	}
};
#endif /* CONFIG_MTD_SUPERH_RESERVE */

static int __init init_soleng_maps(void)
{
	int nr_parts = 0;

	/* First probe at offset 0 */
	soleng_flash_map.phys = 0;
	soleng_flash_map.virt = (void __iomem *)P2SEGADDR(0);
	soleng_eprom_map.phys = 0x01000000;
	soleng_eprom_map.virt = (void __iomem *)P1SEGADDR(0x01000000);
	simple_map_init(&soleng_eprom_map);
	simple_map_init(&soleng_flash_map);

	printk(KERN_NOTICE "Probing for flash chips at 0x00000000:\n");
	flash_mtd = do_map_probe("cfi_probe", &soleng_flash_map);
	if (!flash_mtd) {
		/* Not there. Try swapping */
		printk(KERN_NOTICE "Probing for flash chips at 0x01000000:\n");
		soleng_flash_map.phys = 0x01000000;
		soleng_flash_map.virt = P2SEGADDR(0x01000000);
		soleng_eprom_map.phys = 0;
		soleng_eprom_map.virt = P1SEGADDR(0);
		flash_mtd = do_map_probe("cfi_probe", &soleng_flash_map);
		if (!flash_mtd) {
			/* Eep. */
			printk(KERN_NOTICE "Flash chips not detected at either possible location.\n");
			return -ENXIO;
		}
	}
	printk(KERN_NOTICE "Solution Engine: Flash at 0x%08lx, EPROM at 0x%08lx\n",
	       soleng_flash_map.phys & 0x1fffffff,
	       soleng_eprom_map.phys & 0x1fffffff);
	flash_mtd->owner = THIS_MODULE;

	eprom_mtd = do_map_probe("map_rom", &soleng_eprom_map);
	if (eprom_mtd) {
		eprom_mtd->owner = THIS_MODULE;
		add_mtd_device(eprom_mtd);
	}

	nr_parts = parse_mtd_partitions(flash_mtd, probes, &parsed_parts, 0);

#ifdef CONFIG_MTD_SUPERH_RESERVE
	if (nr_parts <= 0) {
		printk(KERN_NOTICE "Using configured partition at 0x%08x.\n",
		       CONFIG_MTD_SUPERH_RESERVE);
		parsed_parts = superh_se_partitions;
		nr_parts = sizeof(superh_se_partitions)/sizeof(*parsed_parts);
	}
#endif /* CONFIG_MTD_SUPERH_RESERVE */

	if (nr_parts > 0)
		add_mtd_partitions(flash_mtd, parsed_parts, nr_parts);
	else
		add_mtd_device(flash_mtd);

	return 0;
}

static void __exit cleanup_soleng_maps(void)
{
	if (eprom_mtd) {
		del_mtd_device(eprom_mtd);
		map_destroy(eprom_mtd);
	}

	if (parsed_parts)
		del_mtd_partitions(flash_mtd);
	else
		del_mtd_device(flash_mtd);
	map_destroy(flash_mtd);
}

module_init(init_soleng_maps);
module_exit(cleanup_soleng_maps);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("David Woodhouse <dwmw2@infradead.org>");
MODULE_DESCRIPTION("MTD map driver for Hitachi SolutionEngine (and similar) boards");

