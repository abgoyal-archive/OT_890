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
 * Handle mapping of the flash on MBX860 boards
 *
 * Author:	Anton Todorov
 * Copyright:	(C) 2001 Emness Technology
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 */

#include <linux/module.h>
#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <asm/io.h>
#include <linux/mtd/mtd.h>
#include <linux/mtd/map.h>
#include <linux/mtd/partitions.h>


#define WINDOW_ADDR 0xfe000000
#define WINDOW_SIZE 0x00200000

/* Flash / Partition sizing */
#define MAX_SIZE_KiB              8192
#define BOOT_PARTITION_SIZE_KiB    512
#define KERNEL_PARTITION_SIZE_KiB 5632
#define APP_PARTITION_SIZE_KiB    2048

#define NUM_PARTITIONS 3

/* partition_info gives details on the logical partitions that the split the
 * single flash device into. If the size if zero we use up to the end of the
 * device. */
static struct mtd_partition partition_info[]={
	{ .name = "MBX flash BOOT partition",
	.offset = 0,
	.size =   BOOT_PARTITION_SIZE_KiB*1024 },
	{ .name = "MBX flash DATA partition",
	.offset = BOOT_PARTITION_SIZE_KiB*1024,
	.size = (KERNEL_PARTITION_SIZE_KiB)*1024 },
	{ .name = "MBX flash APPLICATION partition",
	.offset = (BOOT_PARTITION_SIZE_KiB+KERNEL_PARTITION_SIZE_KiB)*1024 }
};


static struct mtd_info *mymtd;

struct map_info mbx_map = {
	.name = "MBX flash",
	.size = WINDOW_SIZE,
	.phys = WINDOW_ADDR,
	.bankwidth = 4,
};

static int __init init_mbx(void)
{
	printk(KERN_NOTICE "Motorola MBX flash device: 0x%x at 0x%x\n", WINDOW_SIZE*4, WINDOW_ADDR);
	mbx_map.virt = ioremap(WINDOW_ADDR, WINDOW_SIZE * 4);

	if (!mbx_map.virt) {
		printk("Failed to ioremap\n");
		return -EIO;
	}
	simple_map_init(&mbx_map);

	mymtd = do_map_probe("jedec_probe", &mbx_map);
	if (mymtd) {
		mymtd->owner = THIS_MODULE;
		add_mtd_device(mymtd);
                add_mtd_partitions(mymtd, partition_info, NUM_PARTITIONS);
		return 0;
	}

	iounmap((void *)mbx_map.virt);
	return -ENXIO;
}

static void __exit cleanup_mbx(void)
{
	if (mymtd) {
		del_mtd_device(mymtd);
		map_destroy(mymtd);
	}
	if (mbx_map.virt) {
		iounmap((void *)mbx_map.virt);
		mbx_map.virt = 0;
	}
}

module_init(init_mbx);
module_exit(cleanup_mbx);

MODULE_AUTHOR("Anton Todorov <a.todorov@emness.com>");
MODULE_DESCRIPTION("MTD map driver for Motorola MBX860 board");
MODULE_LICENSE("GPL");
