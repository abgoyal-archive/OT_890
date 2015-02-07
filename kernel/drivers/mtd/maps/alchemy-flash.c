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
 * Flash memory access on AMD Alchemy evaluation boards
 *
 * (C) 2003, 2004 Pete Popov <ppopov@embeddedalley.com>
 */

#include <linux/init.h>
#include <linux/module.h>
#include <linux/types.h>
#include <linux/kernel.h>

#include <linux/mtd/mtd.h>
#include <linux/mtd/map.h>
#include <linux/mtd/partitions.h>

#include <asm/io.h>

#ifdef CONFIG_MIPS_PB1000
#define BOARD_MAP_NAME "Pb1000 Flash"
#define BOARD_FLASH_SIZE 0x00800000 /* 8MB */
#define BOARD_FLASH_WIDTH 4 /* 32-bits */
#endif

#ifdef CONFIG_MIPS_PB1500
#define BOARD_MAP_NAME "Pb1500 Flash"
#define BOARD_FLASH_SIZE 0x04000000 /* 64MB */
#define BOARD_FLASH_WIDTH 4 /* 32-bits */
#endif

#ifdef CONFIG_MIPS_PB1100
#define BOARD_MAP_NAME "Pb1100 Flash"
#define BOARD_FLASH_SIZE 0x04000000 /* 64MB */
#define BOARD_FLASH_WIDTH 4 /* 32-bits */
#endif

#ifdef CONFIG_MIPS_PB1550
#define BOARD_MAP_NAME "Pb1550 Flash"
#define BOARD_FLASH_SIZE 0x08000000 /* 128MB */
#define BOARD_FLASH_WIDTH 4 /* 32-bits */
#endif

#ifdef CONFIG_MIPS_PB1200
#define BOARD_MAP_NAME "Pb1200 Flash"
#define BOARD_FLASH_SIZE 0x08000000 /* 128MB */
#define BOARD_FLASH_WIDTH 2 /* 16-bits */
#endif

#ifdef CONFIG_MIPS_DB1000
#define BOARD_MAP_NAME "Db1000 Flash"
#define BOARD_FLASH_SIZE 0x02000000 /* 32MB */
#define BOARD_FLASH_WIDTH 4 /* 32-bits */
#endif

#ifdef CONFIG_MIPS_DB1500
#define BOARD_MAP_NAME "Db1500 Flash"
#define BOARD_FLASH_SIZE 0x02000000 /* 32MB */
#define BOARD_FLASH_WIDTH 4 /* 32-bits */
#endif

#ifdef CONFIG_MIPS_DB1100
#define BOARD_MAP_NAME "Db1100 Flash"
#define BOARD_FLASH_SIZE 0x02000000 /* 32MB */
#define BOARD_FLASH_WIDTH 4 /* 32-bits */
#endif

#ifdef CONFIG_MIPS_DB1550
#define BOARD_MAP_NAME "Db1550 Flash"
#define BOARD_FLASH_SIZE 0x08000000 /* 128MB */
#define BOARD_FLASH_WIDTH 4 /* 32-bits */
#endif

#ifdef CONFIG_MIPS_DB1200
#define BOARD_MAP_NAME "Db1200 Flash"
#define BOARD_FLASH_SIZE 0x04000000 /* 64MB */
#define BOARD_FLASH_WIDTH 2 /* 16-bits */
#endif

#ifdef CONFIG_MIPS_BOSPORUS
#define BOARD_MAP_NAME "Bosporus Flash"
#define BOARD_FLASH_SIZE 0x01000000 /* 16MB */
#define BOARD_FLASH_WIDTH 2 /* 16-bits */
#endif

#ifdef CONFIG_MIPS_MIRAGE
#define BOARD_MAP_NAME "Mirage Flash"
#define BOARD_FLASH_SIZE 0x04000000 /* 64MB */
#define BOARD_FLASH_WIDTH 4 /* 32-bits */
#define USE_LOCAL_ACCESSORS /* why? */
#endif

static struct map_info alchemy_map = {
	.name =	BOARD_MAP_NAME,
};

static struct mtd_partition alchemy_partitions[] = {
        {
                .name = "User FS",
                .size = BOARD_FLASH_SIZE - 0x00400000,
                .offset = 0x0000000
        },{
                .name = "YAMON",
                .size = 0x0100000,
		.offset = MTDPART_OFS_APPEND,
                .mask_flags = MTD_WRITEABLE
        },{
                .name = "raw kernel",
		.size = (0x300000 - 0x40000), /* last 256KB is yamon env */
		.offset = MTDPART_OFS_APPEND,
        }
};

static struct mtd_info *mymtd;

static int __init alchemy_mtd_init(void)
{
	struct mtd_partition *parts;
	int nb_parts = 0;
	unsigned long window_addr;
	unsigned long window_size;

	/* Default flash buswidth */
	alchemy_map.bankwidth = BOARD_FLASH_WIDTH;

	window_addr = 0x20000000 - BOARD_FLASH_SIZE;
	window_size = BOARD_FLASH_SIZE;

	/*
	 * Static partition definition selection
	 */
	parts = alchemy_partitions;
	nb_parts = ARRAY_SIZE(alchemy_partitions);
	alchemy_map.size = window_size;

	/*
	 * Now let's probe for the actual flash.  Do it here since
	 * specific machine settings might have been set above.
	 */
	printk(KERN_NOTICE BOARD_MAP_NAME ": probing %d-bit flash bus\n",
			alchemy_map.bankwidth*8);
	alchemy_map.virt = ioremap(window_addr, window_size);
	mymtd = do_map_probe("cfi_probe", &alchemy_map);
	if (!mymtd) {
		iounmap(alchemy_map.virt);
		return -ENXIO;
	}
	mymtd->owner = THIS_MODULE;

	add_mtd_partitions(mymtd, parts, nb_parts);
	return 0;
}

static void __exit alchemy_mtd_cleanup(void)
{
	if (mymtd) {
		del_mtd_partitions(mymtd);
		map_destroy(mymtd);
		iounmap(alchemy_map.virt);
	}
}

module_init(alchemy_mtd_init);
module_exit(alchemy_mtd_cleanup);

MODULE_AUTHOR("Embedded Alley Solutions, Inc");
MODULE_DESCRIPTION(BOARD_MAP_NAME " MTD driver");
MODULE_LICENSE("GPL");
