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
 * D-Box 2 flash driver
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

/* partition_info gives details on the logical partitions that the split the
 * single flash device into. If the size if zero we use up to the end of the
 * device. */
static struct mtd_partition partition_info[]= {
	{
	.name		= "BR bootloader",
	.size		= 128 * 1024,
	.offset		= 0,
	.mask_flags	= MTD_WRITEABLE
	},
	{
	.name		= "FLFS (U-Boot)",
	.size		= 128 * 1024,
	.offset		= MTDPART_OFS_APPEND,
	.mask_flags	= 0
	},
	{
	.name		= "Root (SquashFS)",
	.size		= 7040 * 1024,
	.offset		= MTDPART_OFS_APPEND,
	.mask_flags	= 0
	},
	{
	.name		= "var (JFFS2)",
	.size		= 896 * 1024,
	.offset		= MTDPART_OFS_APPEND,
	.mask_flags	= 0
	},
	{
	.name		= "Flash without bootloader",
	.size		= MTDPART_SIZ_FULL,
	.offset		= 128 * 1024,
	.mask_flags	= 0
	},
	{
	.name		= "Complete Flash",
	.size		= MTDPART_SIZ_FULL,
	.offset		= 0,
	.mask_flags	= MTD_WRITEABLE
	}
};

#define NUM_PARTITIONS ARRAY_SIZE(partition_info)

#define WINDOW_ADDR 0x10000000
#define WINDOW_SIZE 0x800000

static struct mtd_info *mymtd;


struct map_info dbox2_flash_map = {
	.name		= "D-Box 2 flash memory",
	.size		= WINDOW_SIZE,
	.bankwidth	= 4,
	.phys		= WINDOW_ADDR,
};

static int __init init_dbox2_flash(void)
{
       	printk(KERN_NOTICE "D-Box 2 flash driver (size->0x%X mem->0x%X)\n", WINDOW_SIZE, WINDOW_ADDR);
	dbox2_flash_map.virt = ioremap(WINDOW_ADDR, WINDOW_SIZE);

	if (!dbox2_flash_map.virt) {
		printk("Failed to ioremap\n");
		return -EIO;
	}
	simple_map_init(&dbox2_flash_map);

	// Probe for dual Intel 28F320 or dual AMD
	mymtd = do_map_probe("cfi_probe", &dbox2_flash_map);
	if (!mymtd) {
	    // Probe for single Intel 28F640
	    dbox2_flash_map.bankwidth = 2;

	    mymtd = do_map_probe("cfi_probe", &dbox2_flash_map);
	}

	if (mymtd) {
		mymtd->owner = THIS_MODULE;

                /* Create MTD devices for each partition. */
	        add_mtd_partitions(mymtd, partition_info, NUM_PARTITIONS);

		return 0;
	}

	iounmap((void *)dbox2_flash_map.virt);
	return -ENXIO;
}

static void __exit cleanup_dbox2_flash(void)
{
	if (mymtd) {
		del_mtd_partitions(mymtd);
		map_destroy(mymtd);
	}
	if (dbox2_flash_map.virt) {
		iounmap((void *)dbox2_flash_map.virt);
		dbox2_flash_map.virt = 0;
	}
}

module_init(init_dbox2_flash);
module_exit(cleanup_dbox2_flash);


MODULE_LICENSE("GPL");
MODULE_AUTHOR("Kári Davíðsson <kd@flaga.is>, Bastian Blank <waldi@tuxbox.org>, Alexander Wild <wild@te-elektronik.com>");
MODULE_DESCRIPTION("MTD map driver for D-Box 2 board");
