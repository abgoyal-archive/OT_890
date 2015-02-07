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
 * Flash memory access on Hynix GMS30C7201/HMS30C7202 based
 * evaluation boards
 *
 * (C) 2002 Jungjun Kim <jungjun.kim@hynix.com>
 *     2003 Thomas Gleixner <tglx@linutronix.de>
 */

#include <linux/module.h>
#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/errno.h>
#include <linux/slab.h>

#include <linux/mtd/mtd.h>
#include <linux/mtd/map.h>
#include <linux/mtd/partitions.h>
#include <mach/hardware.h>
#include <asm/io.h>

static struct mtd_info *mymtd;

static struct map_info h720x_map = {
	.name =		"H720X",
	.bankwidth =	4,
	.size =		H720X_FLASH_SIZE,
	.phys =		H720X_FLASH_PHYS,
};

static struct mtd_partition h720x_partitions[] = {
        {
                .name = "ArMon",
                .size = 0x00080000,
                .offset = 0,
                .mask_flags = MTD_WRITEABLE
        },{
                .name = "Env",
                .size = 0x00040000,
                .offset = 0x00080000,
                .mask_flags = MTD_WRITEABLE
        },{
                .name = "Kernel",
                .size = 0x00180000,
                .offset = 0x000c0000,
                .mask_flags = MTD_WRITEABLE
        },{
                .name = "Ramdisk",
                .size = 0x00400000,
                .offset = 0x00240000,
                .mask_flags = MTD_WRITEABLE
        },{
                .name = "jffs2",
                .size = MTDPART_SIZ_FULL,
                .offset = MTDPART_OFS_APPEND
        }
};

#define NUM_PARTITIONS ARRAY_SIZE(h720x_partitions)

static int                   nr_mtd_parts;
static struct mtd_partition *mtd_parts;
static const char *probes[] = { "cmdlinepart", NULL };

/*
 * Initialize FLASH support
 */
static int __init h720x_mtd_init(void)
{

	char	*part_type = NULL;

	h720x_map.virt = ioremap(h720x_map.phys, h720x_map.size);

	if (!h720x_map.virt) {
		printk(KERN_ERR "H720x-MTD: ioremap failed\n");
		return -EIO;
	}

	simple_map_init(&h720x_map);

	// Probe for flash bankwidth 4
	printk (KERN_INFO "H720x-MTD probing 32bit FLASH\n");
	mymtd = do_map_probe("cfi_probe", &h720x_map);
	if (!mymtd) {
		printk (KERN_INFO "H720x-MTD probing 16bit FLASH\n");
	    // Probe for bankwidth 2
	    h720x_map.bankwidth = 2;
	    mymtd = do_map_probe("cfi_probe", &h720x_map);
	}

	if (mymtd) {
		mymtd->owner = THIS_MODULE;

#ifdef CONFIG_MTD_PARTITIONS
		nr_mtd_parts = parse_mtd_partitions(mymtd, probes, &mtd_parts, 0);
		if (nr_mtd_parts > 0)
			part_type = "command line";
#endif
		if (nr_mtd_parts <= 0) {
			mtd_parts = h720x_partitions;
			nr_mtd_parts = NUM_PARTITIONS;
			part_type = "builtin";
		}
		printk(KERN_INFO "Using %s partition table\n", part_type);
		add_mtd_partitions(mymtd, mtd_parts, nr_mtd_parts);
		return 0;
	}

	iounmap((void *)h720x_map.virt);
	return -ENXIO;
}

/*
 * Cleanup
 */
static void __exit h720x_mtd_cleanup(void)
{

	if (mymtd) {
		del_mtd_partitions(mymtd);
		map_destroy(mymtd);
	}

	/* Free partition info, if commandline partition was used */
	if (mtd_parts && (mtd_parts != h720x_partitions))
		kfree (mtd_parts);

	if (h720x_map.virt) {
		iounmap((void *)h720x_map.virt);
		h720x_map.virt = 0;
	}
}


module_init(h720x_mtd_init);
module_exit(h720x_mtd_cleanup);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Thomas Gleixner <tglx@linutronix.de>");
MODULE_DESCRIPTION("MTD map driver for Hynix evaluation boards");
