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
 *  linux/arch/arm/mach-pxa/generic.c
 *
 *  Author:	Nicolas Pitre
 *  Created:	Jun 15, 2001
 *  Copyright:	MontaVista Software Inc.
 *
 * Code common to all PXA machines.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * Since this file should be linked before any other machine specific file,
 * the __initcall() here will be executed first.  This serves as default
 * initialization stuff for PXA machines which can be overridden later if
 * need be.
 */
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>

#include <mach/hardware.h>
#include <asm/system.h>
#include <asm/pgtable.h>
#include <asm/mach/map.h>
#include <asm/mach-types.h>

#include <mach/pxa-regs.h>
#include <mach/reset.h>

#include "generic.h"

void clear_reset_status(unsigned int mask)
{
	if (cpu_is_pxa2xx())
		pxa2xx_clear_reset_status(mask);

	if (cpu_is_pxa3xx())
		pxa3xx_clear_reset_status(mask);
}

unsigned long get_clock_tick_rate(void)
{
	unsigned long clock_tick_rate;

	if (cpu_is_pxa25x())
		clock_tick_rate = 3686400;
	else if (machine_is_mainstone())
		clock_tick_rate = 3249600;
	else
		clock_tick_rate = 3250000;

	return clock_tick_rate;
}
EXPORT_SYMBOL(get_clock_tick_rate);

/*
 * Get the clock frequency as reflected by CCCR and the turbo flag.
 * We assume these values have been applied via a fcs.
 * If info is not 0 we also display the current settings.
 */
unsigned int get_clk_frequency_khz(int info)
{
	if (cpu_is_pxa25x())
		return pxa25x_get_clk_frequency_khz(info);
	else if (cpu_is_pxa27x())
		return pxa27x_get_clk_frequency_khz(info);
	else
		return pxa3xx_get_clk_frequency_khz(info);
}
EXPORT_SYMBOL(get_clk_frequency_khz);

/*
 * Return the current memory clock frequency in units of 10kHz
 */
unsigned int get_memclk_frequency_10khz(void)
{
	if (cpu_is_pxa25x())
		return pxa25x_get_memclk_frequency_10khz();
	else if (cpu_is_pxa27x())
		return pxa27x_get_memclk_frequency_10khz();
	else
		return pxa3xx_get_memclk_frequency_10khz();
}
EXPORT_SYMBOL(get_memclk_frequency_10khz);

/*
 * Intel PXA2xx internal register mapping.
 *
 * Note 1: not all PXA2xx variants implement all those addresses.
 *
 * Note 2: virtual 0xfffe0000-0xffffffff is reserved for the vector table
 *         and cache flush area.
 */
static struct map_desc standard_io_desc[] __initdata = {
  	{	/* Devs */
		.virtual	=  0xf2000000,
		.pfn		= __phys_to_pfn(0x40000000),
		.length		= 0x02000000,
		.type		= MT_DEVICE
	}, {	/* Mem Ctl */
		.virtual	=  0xf6000000,
		.pfn		= __phys_to_pfn(0x48000000),
		.length		= 0x00200000,
		.type		= MT_DEVICE
	}, {	/* Camera */
		.virtual	=  0xfa000000,
		.pfn		= __phys_to_pfn(0x50000000),
		.length		= 0x00100000,
		.type		= MT_DEVICE
	}, {	/* IMem ctl */
		.virtual	=  0xfe000000,
		.pfn		= __phys_to_pfn(0x58000000),
		.length		= 0x00100000,
		.type		= MT_DEVICE
	}, {	/* UNCACHED_PHYS_0 */
		.virtual	= 0xff000000,
		.pfn		= __phys_to_pfn(0x00000000),
		.length		= 0x00100000,
		.type		= MT_DEVICE
	}
};

void __init pxa_map_io(void)
{
	iotable_init(standard_io_desc, ARRAY_SIZE(standard_io_desc));
	get_clk_frequency_khz(1);
}
