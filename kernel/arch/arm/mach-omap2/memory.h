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
 * linux/arch/arm/mach-omap2/memory.h
 *
 * Interface for memory timing related functions for OMAP24XX
 *
 * Copyright (C) 2005 Texas Instruments Inc.
 * Richard Woodruff <r-woodruff2@ti.com>
 *
 * Copyright (C) 2005 Nokia Corporation
 * Tony Lindgren <tony@atomide.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#ifndef ARCH_ARM_MACH_OMAP2_MEMORY_H
#define ARCH_ARM_MACH_OMAP2_MEMORY_H

/* Memory timings */
#define M_DDR		1
#define M_LOCK_CTRL	(1 << 2)
#define M_UNLOCK	0
#define M_LOCK		1

struct memory_timings {
	u32 m_type;		/* ddr = 1, sdr = 0 */
	u32 dll_mode;		/* use lock mode = 1, unlock mode = 0 */
	u32 slow_dll_ctrl;	/* unlock mode, dll value for slow speed */
	u32 fast_dll_ctrl;	/* unlock mode, dll value for fast speed */
	u32 base_cs;		/* base chip select to use for calculations */
};

extern void omap2_init_memory_params(u32 force_lock_to_unlock_mode);
extern u32 omap2_memory_get_slow_dll_ctrl(void);
extern u32 omap2_memory_get_fast_dll_ctrl(void);
extern u32 omap2_memory_get_type(void);
u32 omap2_dll_force_needed(void);
u32 omap2_reprogram_sdrc(u32 level, u32 force);
void __init omap2_init_memory(void);
void __init gpmc_init(void);

#endif
