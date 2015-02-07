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
 * linux/arch/sh/boards/lbox/setup.c
 *
 * Copyright (C) 2007 Nobuhiro Iwamatsu
 *
 * NTT COMWARE L-BOX RE2 Support
 *
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 *
 */

#include <linux/init.h>
#include <linux/platform_device.h>
#include <linux/ata_platform.h>
#include <asm/machvec.h>
#include <asm/addrspace.h>
#include <mach/lboxre2.h>
#include <asm/io.h>

static struct resource cf_ide_resources[] = {
	[0] = {
		.start  = 0x1f0,
		.end    = 0x1f0 + 8 ,
		.flags  = IORESOURCE_IO,
	},
	[1] = {
		.start  = 0x1f0 + 0x206,
		.end    = 0x1f0 +8 + 0x206 + 8,
		.flags  = IORESOURCE_IO,
	},
	[2] = {
		.start  = IRQ_CF0,
		.flags  = IORESOURCE_IRQ,
	},
};

static struct platform_device cf_ide_device  = {
	.name           = "pata_platform",
	.id             = -1,
	.num_resources  = ARRAY_SIZE(cf_ide_resources),
	.resource       = cf_ide_resources,
};

static struct platform_device *lboxre2_devices[] __initdata = {
       &cf_ide_device,
};

static int __init lboxre2_devices_setup(void)
{
	u32 cf0_io_base;	/* Boot CF base address */
	pgprot_t prot;
	unsigned long paddrbase, psize;

	/* open I/O area window */
	paddrbase = virt_to_phys((void*)PA_AREA5_IO);
	psize = PAGE_SIZE;
	prot = PAGE_KERNEL_PCC( 1 , _PAGE_PCC_IO16);
	cf0_io_base = (u32)p3_ioremap(paddrbase, psize, prot.pgprot);
	if (!cf0_io_base) {
		printk(KERN_ERR "%s : can't open CF I/O window!\n" , __func__ );
		return -ENOMEM;
	}

	cf_ide_resources[0].start += cf0_io_base ;
	cf_ide_resources[0].end   += cf0_io_base ;
	cf_ide_resources[1].start += cf0_io_base ;
	cf_ide_resources[1].end   += cf0_io_base ;

	return platform_add_devices(lboxre2_devices,
			ARRAY_SIZE(lboxre2_devices));

}
device_initcall(lboxre2_devices_setup);

/*
 * The Machine Vector
 */
static struct sh_machine_vector mv_lboxre2 __initmv = {
	.mv_name		= "L-BOX RE2",
	.mv_nr_irqs		= 72,
	.mv_init_irq		= init_lboxre2_IRQ,
};
