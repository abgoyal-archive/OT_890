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
 * linux/arch/arm/plat-omap/ocpi.c
 *
 * Minimal OCP bus support for omap16xx
 *
 * Copyright (C) 2003 - 2005 Nokia Corporation
 * Written by Tony Lindgren <tony@atomide.com>
 *
 * Modified for clock framework by Paul Mundt <paul.mundt@nokia.com>.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 */

#include <linux/module.h>
#include <linux/types.h>
#include <linux/errno.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/spinlock.h>
#include <linux/err.h>
#include <linux/clk.h>
#include <linux/io.h>

#include <mach/hardware.h>

#define OCPI_BASE		0xfffec320
#define OCPI_FAULT		(OCPI_BASE + 0x00)
#define OCPI_CMD_FAULT		(OCPI_BASE + 0x04)
#define OCPI_SINT0		(OCPI_BASE + 0x08)
#define OCPI_TABORT		(OCPI_BASE + 0x0c)
#define OCPI_SINT1		(OCPI_BASE + 0x10)
#define OCPI_PROT		(OCPI_BASE + 0x14)
#define OCPI_SEC		(OCPI_BASE + 0x18)

/* USB OHCI OCPI access error registers */
#define HOSTUEADDR	0xfffba0e0
#define HOSTUESTATUS	0xfffba0e4

static struct clk *ocpi_ck;

/*
 * Enables device access to OMAP buses via the OCPI bridge
 * FIXME: Add locking
 */
int ocpi_enable(void)
{
	unsigned int val;

	if (!cpu_is_omap16xx())
		return -ENODEV;

	/* Enable access for OHCI in OCPI */
	val = omap_readl(OCPI_PROT);
	val &= ~0xff;
	//val &= (1 << 0);	/* Allow access only to EMIFS */
	omap_writel(val, OCPI_PROT);

	val = omap_readl(OCPI_SEC);
	val &= ~0xff;
	omap_writel(val, OCPI_SEC);

	return 0;
}
EXPORT_SYMBOL(ocpi_enable);

static int __init omap_ocpi_init(void)
{
	if (!cpu_is_omap16xx())
		return -ENODEV;

	ocpi_ck = clk_get(NULL, "l3_ocpi_ck");
	if (IS_ERR(ocpi_ck))
		return PTR_ERR(ocpi_ck);

	clk_enable(ocpi_ck);
	ocpi_enable();
	printk("OMAP OCPI interconnect driver loaded\n");

	return 0;
}

static void __exit omap_ocpi_exit(void)
{
	/* REVISIT: Disable OCPI */

	if (!cpu_is_omap16xx())
		return;

	clk_disable(ocpi_ck);
	clk_put(ocpi_ck);
}

MODULE_AUTHOR("Tony Lindgren <tony@atomide.com>");
MODULE_DESCRIPTION("OMAP OCPI bus controller module");
MODULE_LICENSE("GPL");
module_init(omap_ocpi_init);
module_exit(omap_ocpi_exit);
