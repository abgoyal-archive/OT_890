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
 *  Copyright © 2001 Flaga hf. Medical Devices, Kári Davíðsson <kd@flaga.is>
 *
 *  This program is free software; you can redistribute  it and/or modify it
 *  under  the terms of  the GNU General  Public License as published by the
 *  Free Software Foundation;  either version 2 of the  License, or (at your
 *  option) any later version.
 *
 *  THIS  SOFTWARE  IS PROVIDED   ``AS  IS'' AND   ANY  EXPRESS OR IMPLIED
 *  WARRANTIES,   INCLUDING, BUT NOT  LIMITED  TO, THE IMPLIED WARRANTIES OF
 *  MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.  IN
 *  NO  EVENT  SHALL   THE AUTHOR  BE    LIABLE FOR ANY   DIRECT, INDIRECT,
 *  INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 *  NOT LIMITED   TO, PROCUREMENT OF  SUBSTITUTE GOODS  OR SERVICES; LOSS OF
 *  USE, DATA,  OR PROFITS; OR  BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 *  ANY THEORY OF LIABILITY, WHETHER IN  CONTRACT, STRICT LIABILITY, OR TORT
 *  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 *  THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *  You should have received a copy of the  GNU General Public License along
 *  with this program; if not, write  to the Free Software Foundation, Inc.,
 *  675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include <linux/module.h>
#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <asm/io.h>
#include <linux/mtd/mtd.h>
#include <linux/mtd/map.h>
#include <linux/mtd/partitions.h>


/* We split the flash chip up into four parts.
 * 1: bootloader firts 128k			(0x00000000 - 0x0001FFFF) size 0x020000
 * 2: kernel 640k					(0x00020000 - 0x000BFFFF) size 0x0A0000
 * 3: compressed 1536k root ramdisk	(0x000C0000 - 0x0023FFFF) size 0x180000
 * 4: writeable diskpartition (jffs)(0x00240000 - 0x003FFFFF) size 0x1C0000
 */

#define FLASH_PHYS_ADDR 0x40000000
#define FLASH_SIZE 0x400000

#define FLASH_PARTITION0_ADDR 0x00000000
#define FLASH_PARTITION0_SIZE 0x00020000

#define FLASH_PARTITION1_ADDR 0x00020000
#define FLASH_PARTITION1_SIZE 0x000A0000

#define FLASH_PARTITION2_ADDR 0x000C0000
#define FLASH_PARTITION2_SIZE 0x00180000

#define FLASH_PARTITION3_ADDR 0x00240000
#define FLASH_PARTITION3_SIZE 0x001C0000


struct map_info flagadm_map = {
		.name =		"FlagaDM flash device",
		.size =		FLASH_SIZE,
		.bankwidth =	2,
};

struct mtd_partition flagadm_parts[] = {
	{
		.name =		"Bootloader",
		.offset	=	FLASH_PARTITION0_ADDR,
		.size =		FLASH_PARTITION0_SIZE
	},
	{
		.name =		"Kernel image",
		.offset =	FLASH_PARTITION1_ADDR,
		.size =		FLASH_PARTITION1_SIZE
	},
	{
		.name =		"Initial ramdisk image",
		.offset =	FLASH_PARTITION2_ADDR,
		.size =		FLASH_PARTITION2_SIZE
	},
	{
		.name =		"Persistent storage",
		.offset =	FLASH_PARTITION3_ADDR,
		.size =		FLASH_PARTITION3_SIZE
	}
};

#define PARTITION_COUNT ARRAY_SIZE(flagadm_parts)

static struct mtd_info *mymtd;

static int __init init_flagadm(void)
{
	printk(KERN_NOTICE "FlagaDM flash device: %x at %x\n",
			FLASH_SIZE, FLASH_PHYS_ADDR);

	flagadm_map.phys = FLASH_PHYS_ADDR;
	flagadm_map.virt = ioremap(FLASH_PHYS_ADDR,
					FLASH_SIZE);

	if (!flagadm_map.virt) {
		printk("Failed to ioremap\n");
		return -EIO;
	}

	simple_map_init(&flagadm_map);

	mymtd = do_map_probe("cfi_probe", &flagadm_map);
	if (mymtd) {
		mymtd->owner = THIS_MODULE;
		add_mtd_partitions(mymtd, flagadm_parts, PARTITION_COUNT);
		printk(KERN_NOTICE "FlagaDM flash device initialized\n");
		return 0;
	}

	iounmap((void *)flagadm_map.virt);
	return -ENXIO;
}

static void __exit cleanup_flagadm(void)
{
	if (mymtd) {
		del_mtd_partitions(mymtd);
		map_destroy(mymtd);
	}
	if (flagadm_map.virt) {
		iounmap((void *)flagadm_map.virt);
		flagadm_map.virt = 0;
	}
}

module_init(init_flagadm);
module_exit(cleanup_flagadm);


MODULE_LICENSE("GPL");
MODULE_AUTHOR("Kári Davíðsson <kd@flaga.is>");
MODULE_DESCRIPTION("MTD map driver for Flaga digital module");
