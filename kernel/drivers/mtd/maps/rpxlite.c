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
 * Handle mapping of the flash on the RPX Lite and CLLF boards
 */

#include <linux/module.h>
#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <asm/io.h>
#include <linux/mtd/mtd.h>
#include <linux/mtd/map.h>


#define WINDOW_ADDR 0xfe000000
#define WINDOW_SIZE 0x800000

static struct mtd_info *mymtd;

static struct map_info rpxlite_map = {
	.name = "RPX",
	.size = WINDOW_SIZE,
	.bankwidth = 4,
	.phys = WINDOW_ADDR,
};

static int __init init_rpxlite(void)
{
	printk(KERN_NOTICE "RPX Lite or CLLF flash device: %x at %x\n", WINDOW_SIZE*4, WINDOW_ADDR);
	rpxlite_map.virt = ioremap(WINDOW_ADDR, WINDOW_SIZE * 4);

	if (!rpxlite_map.virt) {
		printk("Failed to ioremap\n");
		return -EIO;
	}
	simple_map_init(&rpxlite_map);
	mymtd = do_map_probe("cfi_probe", &rpxlite_map);
	if (mymtd) {
		mymtd->owner = THIS_MODULE;
		add_mtd_device(mymtd);
		return 0;
	}

	iounmap((void *)rpxlite_map.virt);
	return -ENXIO;
}

static void __exit cleanup_rpxlite(void)
{
	if (mymtd) {
		del_mtd_device(mymtd);
		map_destroy(mymtd);
	}
	if (rpxlite_map.virt) {
		iounmap((void *)rpxlite_map.virt);
		rpxlite_map.virt = 0;
	}
}

module_init(init_rpxlite);
module_exit(cleanup_rpxlite);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Arnold Christensen <AKC@pel.dk>");
MODULE_DESCRIPTION("MTD map driver for RPX Lite and CLLF boards");
