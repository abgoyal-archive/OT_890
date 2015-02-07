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
 *  Copyright (C) 2000 Deep Blue Solutions Ltd
 *  Copyright (C) 2002 Shane Nay (shane@minirl.com)
 *  Copyright 2005-2007 Freescale Semiconductor, Inc. All Rights Reserved.
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

#include <linux/types.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/memory.h>

#include <mach/hardware.h>
#include <asm/mach-types.h>
#include <asm/mach/arch.h>
#include <asm/mach/time.h>
#include <asm/mach/map.h>
#include <mach/common.h>
#include <asm/page.h>
#include <asm/setup.h>
#include <mach/board-mx31lite.h>

/*
 * This file contains the board-specific initialization routines.
 */

/*
 * This structure defines the MX31 memory map.
 */
static struct map_desc mx31lite_io_desc[] __initdata = {
	{
		.virtual = AIPS1_BASE_ADDR_VIRT,
		.pfn = __phys_to_pfn(AIPS1_BASE_ADDR),
		.length = AIPS1_SIZE,
		.type = MT_DEVICE_NONSHARED
	}, {
		.virtual = SPBA0_BASE_ADDR_VIRT,
		.pfn = __phys_to_pfn(SPBA0_BASE_ADDR),
		.length = SPBA0_SIZE,
		.type = MT_DEVICE_NONSHARED
	}, {
		.virtual = AIPS2_BASE_ADDR_VIRT,
		.pfn = __phys_to_pfn(AIPS2_BASE_ADDR),
		.length = AIPS2_SIZE,
		.type = MT_DEVICE_NONSHARED
	}, {
		.virtual = CS4_BASE_ADDR_VIRT,
		.pfn = __phys_to_pfn(CS4_BASE_ADDR),
		.length = CS4_SIZE,
		.type = MT_DEVICE
	}
};

/*
 * Set up static virtual mappings.
 */
void __init mx31lite_map_io(void)
{
	mxc_map_io();
	iotable_init(mx31lite_io_desc, ARRAY_SIZE(mx31lite_io_desc));
}

/*
 * Board specific initialization.
 */
static void __init mxc_board_init(void)
{
}

static void __init mx31lite_timer_init(void)
{
	mxc_clocks_init(26000000);
	mxc_timer_init("ipg_clk.0");
}

struct sys_timer mx31lite_timer = {
	.init	= mx31lite_timer_init,
};

/*
 * The following uses standard kernel macros defined in arch.h in order to
 * initialize __mach_desc_MX31LITE data structure.
 */

MACHINE_START(MX31LITE, "LogicPD MX31 LITEKIT")
	/* Maintainer: Freescale Semiconductor, Inc. */
	.phys_io        = AIPS1_BASE_ADDR,
	.io_pg_offst    = ((AIPS1_BASE_ADDR_VIRT) >> 18) & 0xfffc,
	.boot_params    = PHYS_OFFSET + 0x100,
	.map_io         = mx31lite_map_io,
	.init_irq       = mxc_init_irq,
	.init_machine   = mxc_board_init,
	.timer          = &mx31lite_timer,
MACHINE_END
