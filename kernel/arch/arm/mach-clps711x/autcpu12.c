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
 *  linux/arch/arm/mach-clps711x/autcpu12.c
 *
 * (c) 2001 Thomas Gleixner, autronix automation <gleixner@autronix.de>
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
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/types.h>
#include <linux/string.h>
#include <linux/mm.h>
#include <linux/io.h>

#include <mach/hardware.h>
#include <asm/sizes.h>
#include <asm/setup.h>
#include <asm/mach-types.h>
#include <asm/mach/arch.h>
#include <asm/pgtable.h>
#include <asm/page.h>

#include <asm/mach/map.h>
#include <mach/autcpu12.h>

#include "common.h"

/*
 * The on-chip registers are given a size of 1MB so that a section can
 * be used to map them; this saves a page table.  This is the place to
 * add mappings for ROM, expansion memory, PCMCIA, etc.  (if static
 * mappings are chosen for those areas).
 *
*/

static struct map_desc autcpu12_io_desc[] __initdata = {
	/* memory-mapped extra io and CS8900A Ethernet chip */
 	/* ethernet chip */
 	{
		.virtual	= AUTCPU12_VIRT_CS8900A,
		.pfn		= __phys_to_pfn(AUTCPU12_PHYS_CS8900A),
		.length		= SZ_1M,
		.type		= MT_DEVICE
	}
};

void __init autcpu12_map_io(void)
{
        clps711x_map_io();
        iotable_init(autcpu12_io_desc, ARRAY_SIZE(autcpu12_io_desc));
}

MACHINE_START(AUTCPU12, "autronix autcpu12")
	/* Maintainer: Thomas Gleixner */
	.phys_io	= 0x80000000,
	.io_pg_offst	= ((0xff000000) >> 18) & 0xfffc,
	.boot_params	= 0xc0020000,
	.map_io		= autcpu12_map_io,
	.init_irq	= clps711x_init_irq,
	.timer		= &clps711x_timer,
MACHINE_END

