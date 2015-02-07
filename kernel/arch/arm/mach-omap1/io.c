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
 * linux/arch/arm/mach-omap1/io.c
 *
 * OMAP1 I/O mapping code
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/io.h>

#include <asm/tlb.h>
#include <asm/mach/map.h>
#include <mach/mux.h>
#include <mach/tc.h>

extern int omap1_clk_init(void);
extern void omap_check_revision(void);
extern void omap_sram_init(void);
extern void omapfb_reserve_sdram(void);

/*
 * The machine specific code may provide the extra mapping besides the
 * default mapping provided here.
 */
static struct map_desc omap_io_desc[] __initdata = {
	{
		.virtual	= IO_VIRT,
		.pfn		= __phys_to_pfn(IO_PHYS),
		.length		= IO_SIZE,
		.type		= MT_DEVICE
	}
};

#ifdef CONFIG_ARCH_OMAP730
static struct map_desc omap730_io_desc[] __initdata = {
	{
		.virtual	= OMAP730_DSP_BASE,
		.pfn		= __phys_to_pfn(OMAP730_DSP_START),
		.length		= OMAP730_DSP_SIZE,
		.type		= MT_DEVICE
	}, {
		.virtual	= OMAP730_DSPREG_BASE,
		.pfn		= __phys_to_pfn(OMAP730_DSPREG_START),
		.length		= OMAP730_DSPREG_SIZE,
		.type		= MT_DEVICE
	}
};
#endif

#ifdef CONFIG_ARCH_OMAP15XX
static struct map_desc omap1510_io_desc[] __initdata = {
	{
		.virtual	= OMAP1510_DSP_BASE,
		.pfn		= __phys_to_pfn(OMAP1510_DSP_START),
		.length		= OMAP1510_DSP_SIZE,
		.type		= MT_DEVICE
	}, {
		.virtual	= OMAP1510_DSPREG_BASE,
		.pfn		= __phys_to_pfn(OMAP1510_DSPREG_START),
		.length		= OMAP1510_DSPREG_SIZE,
		.type		= MT_DEVICE
	}
};
#endif

#if defined(CONFIG_ARCH_OMAP16XX)
static struct map_desc omap16xx_io_desc[] __initdata = {
	{
		.virtual	= OMAP16XX_DSP_BASE,
		.pfn		= __phys_to_pfn(OMAP16XX_DSP_START),
		.length		= OMAP16XX_DSP_SIZE,
		.type		= MT_DEVICE
	}, {
		.virtual	= OMAP16XX_DSPREG_BASE,
		.pfn		= __phys_to_pfn(OMAP16XX_DSPREG_START),
		.length		= OMAP16XX_DSPREG_SIZE,
		.type		= MT_DEVICE
	}
};
#endif

/*
 * Maps common IO regions for omap1. This should only get called from
 * board specific init.
 */
void __init omap1_map_common_io(void)
{
	iotable_init(omap_io_desc, ARRAY_SIZE(omap_io_desc));

	/* Normally devicemaps_init() would flush caches and tlb after
	 * mdesc->map_io(), but we must also do it here because of the CPU
	 * revision check below.
	 */
	local_flush_tlb_all();
	flush_cache_all();

	/* We want to check CPU revision early for cpu_is_omapxxxx() macros.
	 * IO space mapping must be initialized before we can do that.
	 */
	omap_check_revision();

#ifdef CONFIG_ARCH_OMAP730
	if (cpu_is_omap730()) {
		iotable_init(omap730_io_desc, ARRAY_SIZE(omap730_io_desc));
	}
#endif
#ifdef CONFIG_ARCH_OMAP15XX
	if (cpu_is_omap15xx()) {
		iotable_init(omap1510_io_desc, ARRAY_SIZE(omap1510_io_desc));
	}
#endif
#if defined(CONFIG_ARCH_OMAP16XX)
	if (cpu_is_omap16xx()) {
		iotable_init(omap16xx_io_desc, ARRAY_SIZE(omap16xx_io_desc));
	}
#endif

	omap_sram_init();
	omapfb_reserve_sdram();
}

/*
 * Common low-level hardware init for omap1. This should only get called from
 * board specific init.
 */
void __init omap1_init_common_hw(void)
{
	/* REVISIT: Refer to OMAP5910 Errata, Advisory SYS_1: "Timeout Abort
	 * on a Posted Write in the TIPB Bridge".
	 */
	omap_writew(0x0, MPU_PUBLIC_TIPB_CNTL);
	omap_writew(0x0, MPU_PRIVATE_TIPB_CNTL);

	/* Must init clocks early to assure that timer interrupt works
	 */
	omap1_clk_init();

	omap1_mux_init();
}

