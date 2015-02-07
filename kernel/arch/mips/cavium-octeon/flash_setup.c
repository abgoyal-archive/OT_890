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
 *   Octeon Bootbus flash setup
 *
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 *
 * Copyright (C) 2007, 2008 Cavium Networks
 */
#include <linux/kernel.h>
#include <linux/mtd/mtd.h>
#include <linux/mtd/map.h>
#include <linux/mtd/partitions.h>

#include <asm/octeon/octeon.h>

static struct map_info flash_map;
static struct mtd_info *mymtd;
#ifdef CONFIG_MTD_PARTITIONS
static int nr_parts;
static struct mtd_partition *parts;
static const char *part_probe_types[] = {
	"cmdlinepart",
#ifdef CONFIG_MTD_REDBOOT_PARTS
	"RedBoot",
#endif
	NULL
};
#endif

/**
 * Module/ driver initialization.
 *
 * Returns Zero on success
 */
static int __init flash_init(void)
{
	/*
	 * Read the bootbus region 0 setup to determine the base
	 * address of the flash.
	 */
	union cvmx_mio_boot_reg_cfgx region_cfg;
	region_cfg.u64 = cvmx_read_csr(CVMX_MIO_BOOT_REG_CFGX(0));
	if (region_cfg.s.en) {
		/*
		 * The bootloader always takes the flash and sets its
		 * address so the entire flash fits below
		 * 0x1fc00000. This way the flash aliases to
		 * 0x1fc00000 for booting. Software can access the
		 * full flash at the true address, while core boot can
		 * access 4MB.
		 */
		/* Use this name so old part lines work */
		flash_map.name = "phys_mapped_flash";
		flash_map.phys = region_cfg.s.base << 16;
		flash_map.size = 0x1fc00000 - flash_map.phys;
		flash_map.bankwidth = 1;
		flash_map.virt = ioremap(flash_map.phys, flash_map.size);
		pr_notice("Bootbus flash: Setting flash for %luMB flash at "
			  "0x%08lx\n", flash_map.size >> 20, flash_map.phys);
		simple_map_init(&flash_map);
		mymtd = do_map_probe("cfi_probe", &flash_map);
		if (mymtd) {
			mymtd->owner = THIS_MODULE;

#ifdef CONFIG_MTD_PARTITIONS
			nr_parts = parse_mtd_partitions(mymtd,
							part_probe_types,
							&parts, 0);
			if (nr_parts > 0)
				add_mtd_partitions(mymtd, parts, nr_parts);
			else
				add_mtd_device(mymtd);
#else
			add_mtd_device(mymtd);
#endif
		} else {
			pr_err("Failed to register MTD device for flash\n");
		}
	}
	return 0;
}

late_initcall(flash_init);
