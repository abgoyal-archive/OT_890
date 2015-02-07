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
 * linux/arch/sh/boards/sh03/setup.c
 *
 * Copyright (C) 2004  Interface Co.,Ltd. Saito.K
 *
 */

#include <linux/init.h>
#include <linux/irq.h>
#include <linux/pci.h>
#include <linux/platform_device.h>
#include <linux/ata_platform.h>
#include <asm/io.h>
#include <asm/rtc.h>
#include <mach-sh03/mach/io.h>
#include <mach-sh03/mach/sh03.h>
#include <asm/addrspace.h>

static void __init init_sh03_IRQ(void)
{
	plat_irq_setup_pins(IRQ_MODE_IRQ);
}

/* arch/sh/boards/sh03/rtc.c */
void sh03_time_init(void);

static void __init sh03_setup(char **cmdline_p)
{
	board_time_init = sh03_time_init;
}

static struct resource cf_ide_resources[] = {
	[0] = {
		.start  = 0x1f0,
		.end    = 0x1f0 + 8,
		.flags  = IORESOURCE_IO,
	},
	[1] = {
		.start  = 0x1f0 + 0x206,
		.end    = 0x1f0 +8 + 0x206 + 8,
		.flags  = IORESOURCE_IO,
	},
	[2] = {
		.start  = IRL2_IRQ,
		.flags  = IORESOURCE_IRQ,
	},
};

static struct platform_device cf_ide_device = {
	.name		= "pata_platform",
	.id		= -1,
	.num_resources	= ARRAY_SIZE(cf_ide_resources),
	.resource	= cf_ide_resources,
};

static struct resource heartbeat_resources[] = {
	[0] = {
		.start	= 0xa0800000,
		.end	= 0xa0800000,
		.flags	= IORESOURCE_MEM,
	},
};

static struct platform_device heartbeat_device = {
	.name		= "heartbeat",
	.id		= -1,
	.num_resources	= ARRAY_SIZE(heartbeat_resources),
	.resource	= heartbeat_resources,
};

static struct platform_device *sh03_devices[] __initdata = {
	&heartbeat_device,
	&cf_ide_device,
};

static int __init sh03_devices_setup(void)
{
	pgprot_t prot;
	unsigned long paddrbase;
	void *cf_ide_base;

	/* open I/O area window */
	paddrbase = virt_to_phys((void *)PA_AREA5_IO);
	prot = PAGE_KERNEL_PCC(1, _PAGE_PCC_IO16);
	cf_ide_base = p3_ioremap(paddrbase, PAGE_SIZE, prot.pgprot);
	if (!cf_ide_base) {
		printk("allocate_cf_area : can't open CF I/O window!\n");
		return -ENOMEM;
	}

	/* IDE cmd address : 0x1f0-0x1f7 and 0x3f6 */
	cf_ide_resources[0].start += (unsigned long)cf_ide_base;
	cf_ide_resources[0].end   += (unsigned long)cf_ide_base;
	cf_ide_resources[1].start += (unsigned long)cf_ide_base;
	cf_ide_resources[1].end   += (unsigned long)cf_ide_base;

	return platform_add_devices(sh03_devices, ARRAY_SIZE(sh03_devices));
}
__initcall(sh03_devices_setup);

static struct sh_machine_vector mv_sh03 __initmv = {
	.mv_name		= "Interface (CTP/PCI-SH03)",
	.mv_setup		= sh03_setup,
	.mv_nr_irqs		= 48,
	.mv_init_irq		= init_sh03_IRQ,
};
