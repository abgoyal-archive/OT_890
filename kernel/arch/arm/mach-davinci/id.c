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
 * Davinci CPU identification code
 *
 * Copyright (C) 2006 Komal Shah <komal_shah802003@yahoo.com>
 *
 * Derived from OMAP1 CPU identification code.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/io.h>

#define JTAG_ID_BASE		0x01c40028

struct davinci_id {
	u8	variant;	/* JTAG ID bits 31:28 */
	u16	part_no;	/* JTAG ID bits 27:12 */
	u32	manufacturer;	/* JTAG ID bits 11:1 */
	u32	type;		/* Cpu id bits [31:8], cpu class bits [7:0] */
};

/* Register values to detect the DaVinci version */
static struct davinci_id davinci_ids[] __initdata = {
	{
		/* DM6446 */
		.part_no      = 0xb700,
		.variant      = 0x0,
		.manufacturer = 0x017,
		.type	      = 0x64460000,
	},
};

/*
 * Get Device Part No. from JTAG ID register
 */
static u16 __init davinci_get_part_no(void)
{
	u32 dev_id, part_no;

	dev_id = davinci_readl(JTAG_ID_BASE);

	part_no = ((dev_id >> 12) & 0xffff);

	return part_no;
}

/*
 * Get Device Revision from JTAG ID register
 */
static u8 __init davinci_get_variant(void)
{
	u32 variant;

	variant = davinci_readl(JTAG_ID_BASE);

	variant = (variant >> 28) & 0xf;

	return variant;
}

void __init davinci_check_revision(void)
{
	int i;
	u16 part_no;
	u8 variant;

	part_no = davinci_get_part_no();
	variant = davinci_get_variant();

	/* First check only the major version in a safe way */
	for (i = 0; i < ARRAY_SIZE(davinci_ids); i++) {
		if (part_no == (davinci_ids[i].part_no)) {
			system_rev = davinci_ids[i].type;
			break;
		}
	}

	/* Check if we can find the dev revision */
	for (i = 0; i < ARRAY_SIZE(davinci_ids); i++) {
		if (part_no == davinci_ids[i].part_no &&
		    variant == davinci_ids[i].variant) {
			system_rev = davinci_ids[i].type;
			break;
		}
	}

	printk("DaVinci DM%04x variant 0x%x\n", system_rev >> 16, variant);
}
