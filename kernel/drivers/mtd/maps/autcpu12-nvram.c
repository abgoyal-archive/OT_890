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
 * NV-RAM memory access on autcpu12
 * (C) 2002 Thomas Gleixner (gleixner@autronix.de)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

#include <linux/module.h>
#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/ioport.h>
#include <linux/init.h>
#include <asm/io.h>
#include <asm/sizes.h>
#include <mach/hardware.h>
#include <mach/autcpu12.h>
#include <linux/mtd/mtd.h>
#include <linux/mtd/map.h>
#include <linux/mtd/partitions.h>


static struct mtd_info *sram_mtd;

struct map_info autcpu12_sram_map = {
	.name = "SRAM",
	.size = 32768,
	.bankwidth = 4,
	.phys = 0x12000000,
};

static int __init init_autcpu12_sram (void)
{
	int err, save0, save1;

	autcpu12_sram_map.virt = ioremap(0x12000000, SZ_128K);
	if (!autcpu12_sram_map.virt) {
		printk("Failed to ioremap autcpu12 NV-RAM space\n");
		err = -EIO;
		goto out;
	}
	simple_map_init(&autcpu_sram_map);

	/*
	 * Check for 32K/128K
	 * read ofs 0
	 * read ofs 0x10000
	 * Write complement to ofs 0x100000
	 * Read	and check result on ofs 0x0
	 * Restore contents
	 */
	save0 = map_read32(&autcpu12_sram_map,0);
	save1 = map_read32(&autcpu12_sram_map,0x10000);
	map_write32(&autcpu12_sram_map,~save0,0x10000);
	/* if we find this pattern on 0x0, we have 32K size
	 * restore contents and exit
	 */
	if ( map_read32(&autcpu12_sram_map,0) != save0) {
		map_write32(&autcpu12_sram_map,save0,0x0);
		goto map;
	}
	/* We have a 128K found, restore 0x10000 and set size
	 * to 128K
	 */
	map_write32(&autcpu12_sram_map,save1,0x10000);
	autcpu12_sram_map.size = SZ_128K;

map:
	sram_mtd = do_map_probe("map_ram", &autcpu12_sram_map);
	if (!sram_mtd) {
		printk("NV-RAM probe failed\n");
		err = -ENXIO;
		goto out_ioremap;
	}

	sram_mtd->owner = THIS_MODULE;
	sram_mtd->erasesize = 16;

	if (add_mtd_device(sram_mtd)) {
		printk("NV-RAM device addition failed\n");
		err = -ENOMEM;
		goto out_probe;
	}

	printk("NV-RAM device size %ldKiB registered on AUTCPU12\n",autcpu12_sram_map.size/SZ_1K);

	return 0;

out_probe:
	map_destroy(sram_mtd);
	sram_mtd = 0;

out_ioremap:
	iounmap((void *)autcpu12_sram_map.virt);
out:
	return err;
}

static void __exit cleanup_autcpu12_maps(void)
{
	if (sram_mtd) {
		del_mtd_device(sram_mtd);
		map_destroy(sram_mtd);
		iounmap((void *)autcpu12_sram_map.virt);
	}
}

module_init(init_autcpu12_sram);
module_exit(cleanup_autcpu12_maps);

MODULE_AUTHOR("Thomas Gleixner");
MODULE_DESCRIPTION("autcpu12 NV-RAM map driver");
MODULE_LICENSE("GPL");
