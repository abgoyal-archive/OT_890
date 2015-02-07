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

/****************************************************************************/

/*
 *	uclinux.c -- generic memory mapped MTD driver for uclinux
 *
 *	(C) Copyright 2002, Greg Ungerer (gerg@snapgear.com)
 */

/****************************************************************************/

#include <linux/module.h>
#include <linux/types.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/mm.h>
#include <linux/major.h>
#include <linux/mtd/mtd.h>
#include <linux/mtd/map.h>
#include <linux/mtd/partitions.h>
#include <asm/io.h>

/****************************************************************************/

struct map_info uclinux_ram_map = {
	.name = "RAM",
};

struct mtd_info *uclinux_ram_mtdinfo;

/****************************************************************************/

struct mtd_partition uclinux_romfs[] = {
	{ .name = "ROMfs" }
};

#define	NUM_PARTITIONS	ARRAY_SIZE(uclinux_romfs)

/****************************************************************************/

int uclinux_point(struct mtd_info *mtd, loff_t from, size_t len,
	size_t *retlen, void **virt, resource_size_t *phys)
{
	struct map_info *map = mtd->priv;
	*virt = map->virt + from;
	if (phys)
		*phys = map->phys + from;
	*retlen = len;
	return(0);
}

/****************************************************************************/

static int __init uclinux_mtd_init(void)
{
	struct mtd_info *mtd;
	struct map_info *mapp;
	extern char _ebss;
	unsigned long addr = (unsigned long) &_ebss;

	mapp = &uclinux_ram_map;
	mapp->phys = addr;
	mapp->size = PAGE_ALIGN(ntohl(*((unsigned long *)(addr + 8))));
	mapp->bankwidth = 4;

	printk("uclinux[mtd]: RAM probe address=0x%x size=0x%x\n",
	       	(int) mapp->phys, (int) mapp->size);

	mapp->virt = ioremap_nocache(mapp->phys, mapp->size);

	if (mapp->virt == 0) {
		printk("uclinux[mtd]: ioremap_nocache() failed\n");
		return(-EIO);
	}

	simple_map_init(mapp);

	mtd = do_map_probe("map_ram", mapp);
	if (!mtd) {
		printk("uclinux[mtd]: failed to find a mapping?\n");
		iounmap(mapp->virt);
		return(-ENXIO);
	}

	mtd->owner = THIS_MODULE;
	mtd->point = uclinux_point;
	mtd->priv = mapp;

	uclinux_ram_mtdinfo = mtd;
	add_mtd_partitions(mtd, uclinux_romfs, NUM_PARTITIONS);

	return(0);
}

/****************************************************************************/

static void __exit uclinux_mtd_cleanup(void)
{
	if (uclinux_ram_mtdinfo) {
		del_mtd_partitions(uclinux_ram_mtdinfo);
		map_destroy(uclinux_ram_mtdinfo);
		uclinux_ram_mtdinfo = NULL;
	}
	if (uclinux_ram_map.virt) {
		iounmap((void *) uclinux_ram_map.virt);
		uclinux_ram_map.virt = 0;
	}
}

/****************************************************************************/

module_init(uclinux_mtd_init);
module_exit(uclinux_mtd_cleanup);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Greg Ungerer <gerg@snapgear.com>");
MODULE_DESCRIPTION("Generic RAM based MTD for uClinux");

/****************************************************************************/
