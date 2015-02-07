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
 * linux/arch/arm/mach-omap1/id.c
 *
 * OMAP1 CPU identification code
 *
 * Copyright (C) 2004 Nokia Corporation
 * Written by Tony Lindgren <tony@atomide.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/io.h>
#include <mach/cpu.h>

#define OMAP_DIE_ID_0		0xfffe1800
#define OMAP_DIE_ID_1		0xfffe1804
#define OMAP_PRODUCTION_ID_0	0xfffe2000
#define OMAP_PRODUCTION_ID_1	0xfffe2004
#define OMAP32_ID_0		0xfffed400
#define OMAP32_ID_1		0xfffed404

struct omap_id {
	u16	jtag_id;	/* Used to determine OMAP type */
	u8	die_rev;	/* Processor revision */
	u32	omap_id;	/* OMAP revision */
	u32	type;		/* Cpu id bits [31:08], cpu class bits [07:00] */
};

static unsigned int omap_revision;

/* Register values to detect the OMAP version */
static struct omap_id omap_ids[] __initdata = {
	{ .jtag_id = 0xb574, .die_rev = 0x2, .omap_id = 0x03310315, .type = 0x03100000},
	{ .jtag_id = 0x355f, .die_rev = 0x0, .omap_id = 0x03320000, .type = 0x07300100},
	{ .jtag_id = 0xb55f, .die_rev = 0x0, .omap_id = 0x03320000, .type = 0x07300300},
	{ .jtag_id = 0xb470, .die_rev = 0x0, .omap_id = 0x03310100, .type = 0x15100000},
	{ .jtag_id = 0xb576, .die_rev = 0x0, .omap_id = 0x03320000, .type = 0x16100000},
	{ .jtag_id = 0xb576, .die_rev = 0x2, .omap_id = 0x03320100, .type = 0x16110000},
	{ .jtag_id = 0xb576, .die_rev = 0x3, .omap_id = 0x03320100, .type = 0x16100c00},
	{ .jtag_id = 0xb576, .die_rev = 0x0, .omap_id = 0x03320200, .type = 0x16100d00},
	{ .jtag_id = 0xb613, .die_rev = 0x0, .omap_id = 0x03320300, .type = 0x1610ef00},
	{ .jtag_id = 0xb613, .die_rev = 0x0, .omap_id = 0x03320300, .type = 0x1610ef00},
	{ .jtag_id = 0xb576, .die_rev = 0x1, .omap_id = 0x03320100, .type = 0x16110000},
	{ .jtag_id = 0xb58c, .die_rev = 0x2, .omap_id = 0x03320200, .type = 0x16110b00},
	{ .jtag_id = 0xb58c, .die_rev = 0x3, .omap_id = 0x03320200, .type = 0x16110c00},
	{ .jtag_id = 0xb65f, .die_rev = 0x0, .omap_id = 0x03320400, .type = 0x16212300},
	{ .jtag_id = 0xb65f, .die_rev = 0x1, .omap_id = 0x03320400, .type = 0x16212300},
	{ .jtag_id = 0xb65f, .die_rev = 0x1, .omap_id = 0x03320500, .type = 0x16212300},
	{ .jtag_id = 0xb5f7, .die_rev = 0x0, .omap_id = 0x03330000, .type = 0x17100000},
	{ .jtag_id = 0xb5f7, .die_rev = 0x1, .omap_id = 0x03330100, .type = 0x17100000},
	{ .jtag_id = 0xb5f7, .die_rev = 0x2, .omap_id = 0x03330100, .type = 0x17100000},
};

unsigned int omap_rev(void)
{
	return omap_revision;
}
EXPORT_SYMBOL(omap_rev);

/*
 * Get OMAP type from PROD_ID.
 * 1710 has the PROD_ID in bits 15:00, not in 16:01 as documented in TRM.
 * 1510 PROD_ID is empty, and 1610 PROD_ID does not make sense.
 * Undocumented register in TEST BLOCK is used as fallback; This seems to
 * work on 1510, 1610 & 1710. The official way hopefully will work in future
 * processors.
 */
static u16 __init omap_get_jtag_id(void)
{
	u32 prod_id, omap_id;

	prod_id = omap_readl(OMAP_PRODUCTION_ID_1);
	omap_id = omap_readl(OMAP32_ID_1);

	/* Check for unusable OMAP_PRODUCTION_ID_1 on 1611B/5912 and 730 */
	if (((prod_id >> 20) == 0) || (prod_id == omap_id))
		prod_id = 0;
	else
		prod_id &= 0xffff;

	if (prod_id)
		return prod_id;

	/* Use OMAP32_ID_1 as fallback */
	prod_id = ((omap_id >> 12) & 0xffff);

	return prod_id;
}

/*
 * Get OMAP revision from DIE_REV.
 * Early 1710 processors may have broken OMAP_DIE_ID, it contains PROD_ID.
 * Undocumented register in the TEST BLOCK is used as fallback.
 * REVISIT: This does not seem to work on 1510
 */
static u8 __init omap_get_die_rev(void)
{
	u32 die_rev;

	die_rev = omap_readl(OMAP_DIE_ID_1);

	/* Check for broken OMAP_DIE_ID on early 1710 */
	if (((die_rev >> 12) & 0xffff) == omap_get_jtag_id())
		die_rev = 0;

	die_rev = (die_rev >> 17) & 0xf;
	if (die_rev)
		return die_rev;

	die_rev = (omap_readl(OMAP32_ID_1) >> 28) & 0xf;

	return die_rev;
}

void __init omap_check_revision(void)
{
	int i;
	u16 jtag_id;
	u8 die_rev;
	u32 omap_id;
	u8 cpu_type;

	jtag_id = omap_get_jtag_id();
	die_rev = omap_get_die_rev();
	omap_id = omap_readl(OMAP32_ID_0);

#ifdef DEBUG
	printk(KERN_DEBUG "OMAP_DIE_ID_0: 0x%08x\n", omap_readl(OMAP_DIE_ID_0));
	printk(KERN_DEBUG "OMAP_DIE_ID_1: 0x%08x DIE_REV: %i\n",
		omap_readl(OMAP_DIE_ID_1),
	       (omap_readl(OMAP_DIE_ID_1) >> 17) & 0xf);
	printk(KERN_DEBUG "OMAP_PRODUCTION_ID_0: 0x%08x\n",
		omap_readl(OMAP_PRODUCTION_ID_0));
	printk(KERN_DEBUG "OMAP_PRODUCTION_ID_1: 0x%08x JTAG_ID: 0x%04x\n",
		omap_readl(OMAP_PRODUCTION_ID_1),
		omap_readl(OMAP_PRODUCTION_ID_1) & 0xffff);
	printk(KERN_DEBUG "OMAP32_ID_0: 0x%08x\n", omap_readl(OMAP32_ID_0));
	printk(KERN_DEBUG "OMAP32_ID_1: 0x%08x\n", omap_readl(OMAP32_ID_1));
	printk(KERN_DEBUG "JTAG_ID: 0x%04x DIE_REV: %i\n", jtag_id, die_rev);
#endif

	system_serial_high = omap_readl(OMAP_DIE_ID_0);
	system_serial_low = omap_readl(OMAP_DIE_ID_1);

	/* First check only the major version in a safe way */
	for (i = 0; i < ARRAY_SIZE(omap_ids); i++) {
		if (jtag_id == (omap_ids[i].jtag_id)) {
			omap_revision = omap_ids[i].type;
			break;
		}
	}

	/* Check if we can find the die revision */
	for (i = 0; i < ARRAY_SIZE(omap_ids); i++) {
		if (jtag_id == omap_ids[i].jtag_id && die_rev == omap_ids[i].die_rev) {
			omap_revision = omap_ids[i].type;
			break;
		}
	}

	/* Finally check also the omap_id */
	for (i = 0; i < ARRAY_SIZE(omap_ids); i++) {
		if (jtag_id == omap_ids[i].jtag_id
		    && die_rev == omap_ids[i].die_rev
		    && omap_id == omap_ids[i].omap_id) {
			omap_revision = omap_ids[i].type;
			break;
		}
	}

	/* Add the cpu class info (7xx, 15xx, 16xx, 24xx) */
	cpu_type = omap_revision >> 24;

	switch (cpu_type) {
	case 0x07:
		omap_revision |= 0x07;
		break;
	case 0x03:
	case 0x15:
		omap_revision |= 0x15;
		break;
	case 0x16:
	case 0x17:
		omap_revision |= 0x16;
		break;
	default:
		printk(KERN_INFO "Unknown OMAP cpu type: 0x%02x\n", cpu_type);
	}

	printk(KERN_INFO "OMAP%04x", omap_revision >> 16);
	if ((omap_revision >> 8) & 0xff)
		printk(KERN_INFO "%x", (omap_revision >> 8) & 0xff);
	printk(KERN_INFO " revision %i handled as %02xxx id: %08x%08x\n",
	       die_rev, omap_revision & 0xff, system_serial_low,
	       system_serial_high);
}

