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
 *  linux/arch/arm/mach-versatile/versatile_pb.c
 *
 *  Copyright (C) 2004 ARM Limited
 *  Copyright (C) 2000 Deep Blue Solutions Ltd
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include <linux/init.h>
#include <linux/device.h>
#include <linux/sysdev.h>
#include <linux/amba/bus.h>
#include <linux/io.h>

#include <mach/hardware.h>
#include <asm/irq.h>
#include <asm/mach-types.h>

#include <asm/mach/arch.h>
#include <asm/mach/mmc.h>

#include "core.h"

#if 1
#define IRQ_MMCI1A	IRQ_VICSOURCE23
#else
#define IRQ_MMCI1A	IRQ_SIC_MMCI1A
#endif

static struct mmc_platform_data mmc1_plat_data = {
	.ocr_mask	= MMC_VDD_32_33|MMC_VDD_33_34,
	.status		= mmc_status,
};

#define UART3_IRQ	{ IRQ_SIC_UART3, NO_IRQ }
#define UART3_DMA	{ 0x86, 0x87 }
#define SCI1_IRQ	{ IRQ_SIC_SCI3, NO_IRQ }
#define SCI1_DMA	{ 0x88, 0x89 }
#define MMCI1_IRQ	{ IRQ_MMCI1A, IRQ_SIC_MMCI1B }
#define MMCI1_DMA	{ 0x85, 0 }

/*
 * These devices are connected via the core APB bridge
 */
#define GPIO2_IRQ	{ IRQ_GPIOINT2, NO_IRQ }
#define GPIO2_DMA	{ 0, 0 }
#define GPIO3_IRQ	{ IRQ_GPIOINT3, NO_IRQ }
#define GPIO3_DMA	{ 0, 0 }

/*
 * These devices are connected via the DMA APB bridge
 */

/* FPGA Primecells */
AMBA_DEVICE(uart3, "fpga:09", UART3,    NULL);
AMBA_DEVICE(sci1,  "fpga:0a", SCI1,     NULL);
AMBA_DEVICE(mmc1,  "fpga:0b", MMCI1,    &mmc1_plat_data);

/* DevChip Primecells */
AMBA_DEVICE(gpio2, "dev:e6",  GPIO2,    NULL);
AMBA_DEVICE(gpio3, "dev:e7",  GPIO3,    NULL);

static struct amba_device *amba_devs[] __initdata = {
	&uart3_device,
	&gpio2_device,
	&gpio3_device,
	&sci1_device,
	&mmc1_device,
};

static void __init versatile_pb_init(void)
{
	int i;

	versatile_init();

	for (i = 0; i < ARRAY_SIZE(amba_devs); i++) {
		struct amba_device *d = amba_devs[i];
		amba_device_register(d, &iomem_resource);
	}
}

MACHINE_START(VERSATILE_PB, "ARM-Versatile PB")
	/* Maintainer: ARM Ltd/Deep Blue Solutions Ltd */
	.phys_io	= 0x101f1000,
	.io_pg_offst	= ((0xf11f1000) >> 18) & 0xfffc,
	.boot_params	= 0x00000100,
	.map_io		= versatile_map_io,
	.init_irq	= versatile_init_irq,
	.timer		= &versatile_timer,
	.init_machine	= versatile_pb_init,
MACHINE_END
