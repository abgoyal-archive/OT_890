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
 * Common code to handle map devices which are simple ROM
 * (C) 2000 Red Hat. GPL'd.
 */

#include <linux/module.h>
#include <linux/types.h>
#include <linux/kernel.h>
#include <asm/io.h>
#include <asm/byteorder.h>
#include <linux/errno.h>
#include <linux/slab.h>
#include <linux/init.h>
#include <linux/mtd/mtd.h>
#include <linux/mtd/map.h>
#include <linux/mtd/compatmac.h>

static int maprom_read (struct mtd_info *, loff_t, size_t, size_t *, u_char *);
static int maprom_write (struct mtd_info *, loff_t, size_t, size_t *, const u_char *);
static void maprom_nop (struct mtd_info *);
static struct mtd_info *map_rom_probe(struct map_info *map);
static int maprom_erase (struct mtd_info *mtd, struct erase_info *info);

static struct mtd_chip_driver maprom_chipdrv = {
	.probe	= map_rom_probe,
	.name	= "map_rom",
	.module	= THIS_MODULE
};

static struct mtd_info *map_rom_probe(struct map_info *map)
{
	struct mtd_info *mtd;

	mtd = kzalloc(sizeof(*mtd), GFP_KERNEL);
	if (!mtd)
		return NULL;

	map->fldrv = &maprom_chipdrv;
	mtd->priv = map;
	mtd->name = map->name;
	mtd->type = MTD_ROM;
	mtd->size = map->size;
	mtd->read = maprom_read;
	mtd->write = maprom_write;
	mtd->sync = maprom_nop;
	mtd->erase = maprom_erase;
	mtd->flags = MTD_CAP_ROM;
	mtd->erasesize = map->size;
	mtd->writesize = 1;

	__module_get(THIS_MODULE);
	return mtd;
}


static int maprom_read (struct mtd_info *mtd, loff_t from, size_t len, size_t *retlen, u_char *buf)
{
	struct map_info *map = mtd->priv;

	map_copy_from(map, buf, from, len);
	*retlen = len;
	return 0;
}

static void maprom_nop(struct mtd_info *mtd)
{
	/* Nothing to see here */
}

static int maprom_write (struct mtd_info *mtd, loff_t to, size_t len, size_t *retlen, const u_char *buf)
{
	printk(KERN_NOTICE "maprom_write called\n");
	return -EIO;
}

static int maprom_erase (struct mtd_info *mtd, struct erase_info *info)
{
	/* We do our best 8) */
	return -EROFS;
}

static int __init map_rom_init(void)
{
	register_mtd_chip_driver(&maprom_chipdrv);
	return 0;
}

static void __exit map_rom_exit(void)
{
	unregister_mtd_chip_driver(&maprom_chipdrv);
}

module_init(map_rom_init);
module_exit(map_rom_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("David Woodhouse <dwmw2@infradead.org>");
MODULE_DESCRIPTION("MTD chip driver for ROM chips");
