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
 * arch/sh/boards/renesas/sdk7780/setup.c
 *
 * Renesas Solutions SH7780 SDK Support
 * Copyright (C) 2008 Nicholas Beck <nbeck@mpc-data.co.uk>
 *
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 */
#include <linux/init.h>
#include <linux/types.h>
#include <linux/platform_device.h>
#include <linux/ata_platform.h>
#include <asm/machvec.h>
#include <mach/sdk7780.h>
#include <asm/heartbeat.h>
#include <asm/io.h>
#include <asm/addrspace.h>

#define GPIO_PECR        0xFFEA0008

//* Heartbeat */
static struct heartbeat_data heartbeat_data = {
	.regsize = 16,
};

static struct resource heartbeat_resources[] = {
	[0] = {
		.start  = PA_LED,
		.end    = PA_LED,
		.flags  = IORESOURCE_MEM,
	},
};

static struct platform_device heartbeat_device = {
	.name           = "heartbeat",
	.id             = -1,
	.dev = {
		.platform_data = &heartbeat_data,
	},
	.num_resources  = ARRAY_SIZE(heartbeat_resources),
	.resource       = heartbeat_resources,
};

/* SMC91x */
static struct resource smc91x_eth_resources[] = {
	[0] = {
		.name   = "smc91x-regs" ,
		.start  = PA_LAN + 0x300,
		.end    = PA_LAN + 0x300 + 0x10 ,
		.flags  = IORESOURCE_MEM,
	},
	[1] = {
		.start  = IRQ_ETHERNET,
		.end    = IRQ_ETHERNET,
		.flags  = IORESOURCE_IRQ,
	},
};

static struct platform_device smc91x_eth_device = {
	.name           = "smc91x",
	.id             = 0,
	.dev = {
		.dma_mask               = NULL,         /* don't use dma */
		.coherent_dma_mask      = 0xffffffff,
	},
	.num_resources  = ARRAY_SIZE(smc91x_eth_resources),
	.resource       = smc91x_eth_resources,
};

static struct platform_device *sdk7780_devices[] __initdata = {
	&heartbeat_device,
	&smc91x_eth_device,
};

static int __init sdk7780_devices_setup(void)
{
	return platform_add_devices(sdk7780_devices,
		ARRAY_SIZE(sdk7780_devices));
}
device_initcall(sdk7780_devices_setup);

static void __init sdk7780_setup(char **cmdline_p)
{
	u16 ver = ctrl_inw(FPGA_FPVERR);
	u16 dateStamp = ctrl_inw(FPGA_FPDATER);

	printk(KERN_INFO "Renesas Technology Europe SDK7780 support.\n");
	printk(KERN_INFO "Board version: %d (revision %d), "
			 "FPGA version: %d (revision %d), datestamp : %d\n",
			 (ver >> 12) & 0xf, (ver >> 8) & 0xf,
			 (ver >>  4) & 0xf, ver & 0xf,
			 dateStamp);

	/* Setup pin mux'ing for PCIC */
	ctrl_outw(0x0000, GPIO_PECR);
}

/*
 * The Machine Vector
 */
static struct sh_machine_vector mv_se7780 __initmv = {
	.mv_name        = "Renesas SDK7780-R3" ,
	.mv_setup		= sdk7780_setup,
	.mv_nr_irqs		= 111,
	.mv_init_irq	= init_sdk7780_IRQ,
};

