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

/* pmc - Driver implementation for power management functions
 * of Power Management Controller (PMC) on SPARCstation-Voyager.
 *
 * Copyright (c) 2002 Eric Brower (ebrower@usa.net)
 */

#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/errno.h>
#include <linux/init.h>
#include <linux/pm.h>
#include <linux/of.h>
#include <linux/of_device.h>

#include <asm/io.h>
#include <asm/oplib.h>
#include <asm/uaccess.h>
#include <asm/auxio.h>

/* Debug
 *
 * #define PMC_DEBUG_LED
 * #define PMC_NO_IDLE
 */

#define PMC_OBPNAME	"SUNW,pmc"
#define PMC_DEVNAME	"pmc"

#define PMC_IDLE_REG	0x00
#define PMC_IDLE_ON	0x01

static u8 __iomem *regs;

#define pmc_readb(offs)		(sbus_readb(regs+offs))
#define pmc_writeb(val, offs)	(sbus_writeb(val, regs+offs))

/*
 * CPU idle callback function
 * See .../arch/sparc/kernel/process.c
 */
static void pmc_swift_idle(void)
{
#ifdef PMC_DEBUG_LED
	set_auxio(0x00, AUXIO_LED);
#endif

	pmc_writeb(pmc_readb(PMC_IDLE_REG) | PMC_IDLE_ON, PMC_IDLE_REG);

#ifdef PMC_DEBUG_LED
	set_auxio(AUXIO_LED, 0x00);
#endif
}

static int __devinit pmc_probe(struct of_device *op,
			       const struct of_device_id *match)
{
	regs = of_ioremap(&op->resource[0], 0,
			  resource_size(&op->resource[0]), PMC_OBPNAME);
	if (!regs) {
		printk(KERN_ERR "%s: unable to map registers\n", PMC_DEVNAME);
		return -ENODEV;
	}

#ifndef PMC_NO_IDLE
	/* Assign power management IDLE handler */
	pm_idle = pmc_swift_idle;
#endif

	printk(KERN_INFO "%s: power management initialized\n", PMC_DEVNAME);
	return 0;
}

static struct of_device_id __initdata pmc_match[] = {
	{
		.name = PMC_OBPNAME,
	},
	{},
};
MODULE_DEVICE_TABLE(of, pmc_match);

static struct of_platform_driver pmc_driver = {
	.name		= "pmc",
	.match_table	= pmc_match,
	.probe		= pmc_probe,
};

static int __init pmc_init(void)
{
	return of_register_driver(&pmc_driver, &of_bus_type);
}

/* This driver is not critical to the boot process
 * and is easiest to ioremap when SBus is already
 * initialized, so we install ourselves thusly:
 */
__initcall(pmc_init);
