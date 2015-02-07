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
 * linux/arch/arm/mach-sa1100/lart.c
 */

#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/tty.h>

#include <mach/hardware.h>
#include <asm/setup.h>
#include <asm/mach-types.h>

#include <asm/mach/arch.h>
#include <asm/mach/map.h>
#include <asm/mach/serial_sa1100.h>
#include <mach/mcp.h>

#include "generic.h"


#warning "include/asm/arch-sa1100/ide.h needs fixing for lart"

static struct mcp_plat_data lart_mcp_data = {
	.mccr0		= MCCR0_ADM,
	.sclk_rate	= 11981000,
};

static void __init lart_init(void)
{
	sa11x0_set_mcp_data(&lart_mcp_data);
}

static struct map_desc lart_io_desc[] __initdata = {
	{	/* main flash memory */
		.virtual	=  0xe8000000,
		.pfn		= __phys_to_pfn(0x00000000),
		.length		= 0x00400000,
		.type		= MT_DEVICE
	}, {	/* main flash, alternative location */
		.virtual	=  0xec000000,
		.pfn		= __phys_to_pfn(0x08000000),
		.length		= 0x00400000,
		.type		= MT_DEVICE
	}
};

static void __init lart_map_io(void)
{
	sa1100_map_io();
	iotable_init(lart_io_desc, ARRAY_SIZE(lart_io_desc));

	sa1100_register_uart(0, 3);
	sa1100_register_uart(1, 1);
	sa1100_register_uart(2, 2);

	GAFR |= (GPIO_UART_TXD | GPIO_UART_RXD);
	GPDR |= GPIO_UART_TXD;
	GPDR &= ~GPIO_UART_RXD;
	PPAR |= PPAR_UPR;
}

MACHINE_START(LART, "LART")
	.phys_io	= 0x80000000,
	.io_pg_offst	= ((0xf8000000) >> 18) & 0xfffc,
	.boot_params	= 0xc0000100,
	.map_io		= lart_map_io,
	.init_irq	= sa1100_init_irq,
	.init_machine	= lart_init,
	.timer		= &sa1100_timer,
MACHINE_END
