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
 * arch/arm/plat-omap/include/mach/sram.h
 *
 * Interface for functions that need to be run in internal SRAM
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#ifndef __ARCH_ARM_OMAP_SRAM_H
#define __ARCH_ARM_OMAP_SRAM_H

extern int __init omap_sram_init(void);
extern void * omap_sram_push(void * start, unsigned long size);
extern void omap_sram_reprogram_clock(u32 dpllctl, u32 ckctl);

extern void omap2_sram_ddr_init(u32 *slow_dll_ctrl, u32 fast_dll_ctrl,
				u32 base_cs, u32 force_unlock);
extern void omap2_sram_reprogram_sdrc(u32 perf_level, u32 dll_val,
				      u32 mem_type);
extern u32 omap2_set_prcm(u32 dpll_ctrl_val, u32 sdrc_rfr_val, int bypass);

extern u32 omap3_configure_core_dpll(u32 sdrc_rfr_ctrl,
				     u32 sdrc_actim_ctrla,
				     u32 sdrc_actim_ctrlb, u32 m2);

/* Do not use these */
extern void omap1_sram_reprogram_clock(u32 ckctl, u32 dpllctl);
extern unsigned long omap1_sram_reprogram_clock_sz;

extern void omap24xx_sram_reprogram_clock(u32 ckctl, u32 dpllctl);
extern unsigned long omap24xx_sram_reprogram_clock_sz;

extern void omap242x_sram_ddr_init(u32 *slow_dll_ctrl, u32 fast_dll_ctrl,
						u32 base_cs, u32 force_unlock);
extern unsigned long omap242x_sram_ddr_init_sz;

extern u32 omap242x_sram_set_prcm(u32 dpll_ctrl_val, u32 sdrc_rfr_val,
						int bypass);
extern unsigned long omap242x_sram_set_prcm_sz;

extern void omap242x_sram_reprogram_sdrc(u32 perf_level, u32 dll_val,
						u32 mem_type);
extern unsigned long omap242x_sram_reprogram_sdrc_sz;


extern void omap243x_sram_ddr_init(u32 *slow_dll_ctrl, u32 fast_dll_ctrl,
						u32 base_cs, u32 force_unlock);
extern unsigned long omap243x_sram_ddr_init_sz;

extern u32 omap243x_sram_set_prcm(u32 dpll_ctrl_val, u32 sdrc_rfr_val,
						int bypass);
extern unsigned long omap243x_sram_set_prcm_sz;

extern void omap243x_sram_reprogram_sdrc(u32 perf_level, u32 dll_val,
						u32 mem_type);
extern unsigned long omap243x_sram_reprogram_sdrc_sz;


extern u32 omap3_sram_configure_core_dpll(u32 sdrc_rfr_ctrl,
					  u32 sdrc_actim_ctrla,
					  u32 sdrc_actim_ctrlb, u32 m2);
extern unsigned long omap3_sram_configure_core_dpll_sz;

#endif
