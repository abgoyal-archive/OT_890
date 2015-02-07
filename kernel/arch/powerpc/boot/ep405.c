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
 * Embedded Planet EP405 with PlanetCore firmware
 *
 * (c) Benjamin Herrenschmidt <benh@kernel.crashing.org>, IBM Corp,\
 *
 * Based on ep88xc.c by
 *
 * Scott Wood <scottwood@freescale.com>
 *
 * Copyright (c) 2007 Freescale Semiconductor, Inc.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2 as published
 * by the Free Software Foundation.
 */

#include "ops.h"
#include "stdio.h"
#include "planetcore.h"
#include "dcr.h"
#include "4xx.h"
#include "io.h"

static char *table;
static u64 mem_size;

static void platform_fixups(void)
{
	u64 val;
	void *nvrtc;

	dt_fixup_memory(0, mem_size);
	planetcore_set_mac_addrs(table);

	if (!planetcore_get_decimal(table, PLANETCORE_KEY_CRYSTAL_HZ, &val)) {
		printf("No PlanetCore crystal frequency key.\r\n");
		return;
	}
	ibm405gp_fixup_clocks(val, 0xa8c000);
	ibm4xx_quiesce_eth((u32 *)0xef600800, NULL);
	ibm4xx_fixup_ebc_ranges("/plb/ebc");

	if (!planetcore_get_decimal(table, PLANETCORE_KEY_KB_NVRAM, &val)) {
		printf("No PlanetCore NVRAM size key.\r\n");
		return;
	}
	nvrtc = finddevice("/plb/ebc/nvrtc@4,200000");
	if (nvrtc != NULL) {
		u32 reg[3] = { 4, 0x200000, 0};
		getprop(nvrtc, "reg", reg, 3);
		reg[2] = (val << 10) & 0xffffffff;
		setprop(nvrtc, "reg", reg, 3);
	}
}

void platform_init(unsigned long r3, unsigned long r4, unsigned long r5,
		   unsigned long r6, unsigned long r7)
{
	table = (char *)r3;
	planetcore_prepare_table(table);

	if (!planetcore_get_decimal(table, PLANETCORE_KEY_MB_RAM, &mem_size))
		return;

	mem_size *= 1024 * 1024;
	simple_alloc_init(_end, mem_size - (unsigned long)_end, 32, 64);

	fdt_init(_dtb_start);

	planetcore_set_stdout_path(table);

	serial_console_init();
	platform_ops.fixups = platform_fixups;
}
