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
 * OMAP24XX powerdomain definitions
 *
 * Copyright (C) 2007-2008 Texas Instruments, Inc.
 * Copyright (C) 2007-2008 Nokia Corporation
 *
 * Written by Paul Walmsley
 * Debugging and integration fixes by Jouni Högander
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#ifndef ARCH_ARM_MACH_OMAP2_POWERDOMAINS24XX
#define ARCH_ARM_MACH_OMAP2_POWERDOMAINS24XX

/*
 * N.B. If powerdomains are added or removed from this file, update
 * the array in mach-omap2/powerdomains.h.
 */

#include <mach/powerdomain.h>

#include "prcm-common.h"
#include "prm.h"
#include "prm-regbits-24xx.h"
#include "cm.h"
#include "cm-regbits-24xx.h"

/* 24XX powerdomains and dependencies */

#ifdef CONFIG_ARCH_OMAP24XX


/* Wakeup dependency source arrays */

/*
 * 2420/2430 PM_WKDEP_DSP: CORE, MPU, WKUP
 * 2430 PM_WKDEP_MDM: same as above
 */
static struct pwrdm_dep dsp_mdm_24xx_wkdeps[] = {
	{
		.pwrdm_name = "core_pwrdm",
		.omap_chip = OMAP_CHIP_INIT(CHIP_IS_OMAP24XX)
	},
	{
		.pwrdm_name = "mpu_pwrdm",
		.omap_chip = OMAP_CHIP_INIT(CHIP_IS_OMAP24XX)
	},
	{
		.pwrdm_name = "wkup_pwrdm",
		.omap_chip = OMAP_CHIP_INIT(CHIP_IS_OMAP24XX)
	},
	{ NULL },
};

/*
 * 2420 PM_WKDEP_MPU: CORE, DSP, WKUP
 * 2430 adds MDM
 */
static struct pwrdm_dep mpu_24xx_wkdeps[] = {
	{
		.pwrdm_name = "core_pwrdm",
		.omap_chip = OMAP_CHIP_INIT(CHIP_IS_OMAP24XX)
	},
	{
		.pwrdm_name = "dsp_pwrdm",
		.omap_chip = OMAP_CHIP_INIT(CHIP_IS_OMAP24XX)
	},
	{
		.pwrdm_name = "wkup_pwrdm",
		.omap_chip = OMAP_CHIP_INIT(CHIP_IS_OMAP24XX)
	},
	{
		.pwrdm_name = "mdm_pwrdm",
		.omap_chip = OMAP_CHIP_INIT(CHIP_IS_OMAP2430)
	},
	{ NULL },
};

/*
 * 2420 PM_WKDEP_CORE: DSP, GFX, MPU, WKUP
 * 2430 adds MDM
 */
static struct pwrdm_dep core_24xx_wkdeps[] = {
	{
		.pwrdm_name = "dsp_pwrdm",
		.omap_chip = OMAP_CHIP_INIT(CHIP_IS_OMAP24XX)
	},
	{
		.pwrdm_name = "gfx_pwrdm",
		.omap_chip = OMAP_CHIP_INIT(CHIP_IS_OMAP24XX)
	},
	{
		.pwrdm_name = "mpu_pwrdm",
		.omap_chip = OMAP_CHIP_INIT(CHIP_IS_OMAP24XX)
	},
	{
		.pwrdm_name = "wkup_pwrdm",
		.omap_chip = OMAP_CHIP_INIT(CHIP_IS_OMAP24XX)
	},
	{
		.pwrdm_name = "mdm_pwrdm",
		.omap_chip = OMAP_CHIP_INIT(CHIP_IS_OMAP2430)
	},
	{ NULL },
};


/* Powerdomains */

static struct powerdomain dsp_pwrdm = {
	.name		  = "dsp_pwrdm",
	.prcm_offs	  = OMAP24XX_DSP_MOD,
	.omap_chip	  = OMAP_CHIP_INIT(CHIP_IS_OMAP24XX),
	.dep_bit	  = OMAP24XX_PM_WKDEP_MPU_EN_DSP_SHIFT,
	.wkdep_srcs	  = dsp_mdm_24xx_wkdeps,
	.pwrsts		  = PWRSTS_OFF_RET_ON,
	.pwrsts_logic_ret = PWRDM_POWER_RET,
	.banks		  = 1,
	.pwrsts_mem_ret	  = {
		[0] = PWRDM_POWER_RET,
	},
	.pwrsts_mem_on	  = {
		[0] = PWRDM_POWER_ON,
	},
};

static struct powerdomain mpu_24xx_pwrdm = {
	.name		  = "mpu_pwrdm",
	.prcm_offs	  = MPU_MOD,
	.omap_chip	  = OMAP_CHIP_INIT(CHIP_IS_OMAP24XX),
	.dep_bit	  = OMAP24XX_EN_MPU_SHIFT,
	.wkdep_srcs	  = mpu_24xx_wkdeps,
	.pwrsts		  = PWRSTS_OFF_RET_ON,
	.pwrsts_logic_ret = PWRSTS_OFF_RET,
	.banks		  = 1,
	.pwrsts_mem_ret	  = {
		[0] = PWRDM_POWER_RET,
	},
	.pwrsts_mem_on	  = {
		[0] = PWRDM_POWER_ON,
	},
};

static struct powerdomain core_24xx_pwrdm = {
	.name		  = "core_pwrdm",
	.prcm_offs	  = CORE_MOD,
	.omap_chip	  = OMAP_CHIP_INIT(CHIP_IS_OMAP24XX),
	.wkdep_srcs	  = core_24xx_wkdeps,
	.pwrsts		  = PWRSTS_OFF_RET_ON,
	.dep_bit	  = OMAP24XX_EN_CORE_SHIFT,
	.banks		  = 3,
	.pwrsts_mem_ret	  = {
		[0] = PWRSTS_OFF_RET,	 /* MEM1RETSTATE */
		[1] = PWRSTS_OFF_RET,	 /* MEM2RETSTATE */
		[2] = PWRSTS_OFF_RET,	 /* MEM3RETSTATE */
	},
	.pwrsts_mem_on	  = {
		[0] = PWRSTS_OFF_RET_ON, /* MEM1ONSTATE */
		[1] = PWRSTS_OFF_RET_ON, /* MEM2ONSTATE */
		[2] = PWRSTS_OFF_RET_ON, /* MEM3ONSTATE */
	},
};

#endif	   /* CONFIG_ARCH_OMAP24XX */



/*
 * 2430-specific powerdomains
 */

#ifdef CONFIG_ARCH_OMAP2430

/* XXX 2430 KILLDOMAINWKUP bit?  No current users apparently */

/* Another case of bit name collisions between several registers: EN_MDM */
static struct powerdomain mdm_pwrdm = {
	.name		  = "mdm_pwrdm",
	.prcm_offs	  = OMAP2430_MDM_MOD,
	.omap_chip	  = OMAP_CHIP_INIT(CHIP_IS_OMAP2430),
	.dep_bit	  = OMAP2430_PM_WKDEP_MPU_EN_MDM_SHIFT,
	.wkdep_srcs	  = dsp_mdm_24xx_wkdeps,
	.pwrsts		  = PWRSTS_OFF_RET_ON,
	.pwrsts_logic_ret = PWRDM_POWER_RET,
	.banks		  = 1,
	.pwrsts_mem_ret	  = {
		[0] = PWRDM_POWER_RET, /* MEMRETSTATE */
	},
	.pwrsts_mem_on	  = {
		[0] = PWRDM_POWER_ON,  /* MEMONSTATE */
	},
};

#endif     /* CONFIG_ARCH_OMAP2430 */


#endif
