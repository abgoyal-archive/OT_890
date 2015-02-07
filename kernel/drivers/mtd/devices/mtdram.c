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
 * mtdram - a test mtd device
 * Author: Alexander Larsson <alex@cendio.se>
 *
 * Copyright (c) 1999 Alexander Larsson <alex@cendio.se>
 * Copyright (c) 2005 Joern Engel <joern@wh.fh-wedel.de>
 *
 * This code is GPL
 *
 */

#include <linux/module.h>
#include <linux/slab.h>
#include <linux/ioport.h>
#include <linux/vmalloc.h>
#include <linux/init.h>
#include <linux/mtd/compatmac.h>
#include <linux/mtd/mtd.h>
#include <linux/mtd/mtdram.h>

static unsigned long total_size = CONFIG_MTDRAM_TOTAL_SIZE;
static unsigned long erase_size = CONFIG_MTDRAM_ERASE_SIZE;
#define MTDRAM_TOTAL_SIZE (total_size * 1024)
#define MTDRAM_ERASE_SIZE (erase_size * 1024)

#ifdef MODULE
module_param(total_size, ulong, 0);
MODULE_PARM_DESC(total_size, "Total device size in KiB");
module_param(erase_size, ulong, 0);
MODULE_PARM_DESC(erase_size, "Device erase block size in KiB");
#endif

// We could store these in the mtd structure, but we only support 1 device..
static struct mtd_info *mtd_info;

static int ram_erase(struct mtd_info *mtd, struct erase_info *instr)
{
	if (instr->addr + instr->len > mtd->size)
		return -EINVAL;

	memset((char *)mtd->priv + instr->addr, 0xff, instr->len);

	instr->state = MTD_ERASE_DONE;
	mtd_erase_callback(instr);

	return 0;
}

static int ram_point(struct mtd_info *mtd, loff_t from, size_t len,
		size_t *retlen, void **virt, resource_size_t *phys)
{
	if (from + len > mtd->size)
		return -EINVAL;

	/* can we return a physical address with this driver? */
	if (phys)
		return -EINVAL;

	*virt = mtd->priv + from;
	*retlen = len;
	return 0;
}

static void ram_unpoint(struct mtd_info *mtd, loff_t from, size_t len)
{
}

static int ram_read(struct mtd_info *mtd, loff_t from, size_t len,
		size_t *retlen, u_char *buf)
{
	if (from + len > mtd->size)
		return -EINVAL;

	memcpy(buf, mtd->priv + from, len);

	*retlen = len;
	return 0;
}

static int ram_write(struct mtd_info *mtd, loff_t to, size_t len,
		size_t *retlen, const u_char *buf)
{
	if (to + len > mtd->size)
		return -EINVAL;

	memcpy((char *)mtd->priv + to, buf, len);

	*retlen = len;
	return 0;
}

static void __exit cleanup_mtdram(void)
{
	if (mtd_info) {
		del_mtd_device(mtd_info);
		vfree(mtd_info->priv);
		kfree(mtd_info);
	}
}

int mtdram_init_device(struct mtd_info *mtd, void *mapped_address,
		unsigned long size, char *name)
{
	memset(mtd, 0, sizeof(*mtd));

	/* Setup the MTD structure */
	mtd->name = name;
	mtd->type = MTD_RAM;
	mtd->flags = MTD_CAP_RAM;
	mtd->size = size;
	mtd->writesize = 1;
	mtd->erasesize = MTDRAM_ERASE_SIZE;
	mtd->priv = mapped_address;

	mtd->owner = THIS_MODULE;
	mtd->erase = ram_erase;
	mtd->point = ram_point;
	mtd->unpoint = ram_unpoint;
	mtd->read = ram_read;
	mtd->write = ram_write;

	if (add_mtd_device(mtd)) {
		return -EIO;
	}

	return 0;
}

static int __init init_mtdram(void)
{
	void *addr;
	int err;

	if (!total_size)
		return -EINVAL;

	/* Allocate some memory */
	mtd_info = kmalloc(sizeof(struct mtd_info), GFP_KERNEL);
	if (!mtd_info)
		return -ENOMEM;

	addr = vmalloc(MTDRAM_TOTAL_SIZE);
	if (!addr) {
		kfree(mtd_info);
		mtd_info = NULL;
		return -ENOMEM;
	}
	err = mtdram_init_device(mtd_info, addr, MTDRAM_TOTAL_SIZE, "mtdram test device");
	if (err) {
		vfree(addr);
		kfree(mtd_info);
		mtd_info = NULL;
		return err;
	}
	memset(mtd_info->priv, 0xff, MTDRAM_TOTAL_SIZE);
	return err;
}

module_init(init_mtdram);
module_exit(cleanup_mtdram);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Alexander Larsson <alexl@redhat.com>");
MODULE_DESCRIPTION("Simulated MTD driver for testing");
