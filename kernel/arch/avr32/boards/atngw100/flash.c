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
 * ATNGW100 board-specific flash initialization
 *
 * Copyright (C) 2005-2006 Atmel Corporation
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */
#include <linux/init.h>
#include <linux/platform_device.h>
#include <linux/mtd/mtd.h>
#include <linux/mtd/partitions.h>
#include <linux/mtd/physmap.h>

#include <mach/smc.h>

static struct smc_timing flash_timing __initdata = {
	.ncs_read_setup		= 0,
	.nrd_setup		= 40,
	.ncs_write_setup	= 0,
	.nwe_setup		= 10,

	.ncs_read_pulse		= 80,
	.nrd_pulse		= 40,
	.ncs_write_pulse	= 65,
	.nwe_pulse		= 55,

	.read_cycle		= 120,
	.write_cycle		= 120,
};

static struct smc_config flash_config __initdata = {
	.bus_width		= 2,
	.nrd_controlled		= 1,
	.nwe_controlled		= 1,
	.byte_write		= 1,
};

static struct mtd_partition flash_parts[] = {
	{
		.name           = "u-boot",
		.offset         = 0x00000000,
		.size           = 0x00020000,           /* 128 KiB */
		.mask_flags     = MTD_WRITEABLE,
	},
	{
		.name           = "root",
		.offset         = 0x00020000,
		.size           = 0x007d0000,
	},
	{
		.name           = "env",
		.offset         = 0x007f0000,
		.size           = 0x00010000,
		.mask_flags     = MTD_WRITEABLE,
	},
};

static struct physmap_flash_data flash_data = {
	.width		= 2,
	.nr_parts	= ARRAY_SIZE(flash_parts),
	.parts		= flash_parts,
};

static struct resource flash_resource = {
	.start		= 0x00000000,
	.end		= 0x007fffff,
	.flags		= IORESOURCE_MEM,
};

static struct platform_device flash_device = {
	.name		= "physmap-flash",
	.id		= 0,
	.resource	= &flash_resource,
	.num_resources	= 1,
	.dev		= {
		.platform_data = &flash_data,
	},
};

/* This needs to be called after the SMC has been initialized */
static int __init atngw100_flash_init(void)
{
	int ret;

	smc_set_timing(&flash_config, &flash_timing);
	ret = smc_set_configuration(0, &flash_config);
	if (ret < 0) {
		printk(KERN_ERR "atngw100: failed to set NOR flash timing\n");
		return ret;
	}

	platform_device_register(&flash_device);

	return 0;
}
device_initcall(atngw100_flash_init);
