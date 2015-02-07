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

/* arch/arm/plat-s3c64xx/include/plat/pll.h
 *
 * Copyright 2008 Openmoko, Inc.
 * Copyright 2008 Simtec Electronics
 *	Ben Dooks <ben@simtec.co.uk>
 *	http://armlinux.simtec.co.uk/
 *
 * S3C64XX PLL code
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
*/

#define S3C6400_PLL_MDIV_MASK	((1 << (25-16+1)) - 1)
#define S3C6400_PLL_PDIV_MASK	((1 << (13-8+1)) - 1)
#define S3C6400_PLL_SDIV_MASK	((1 << (2-0+1)) - 1)
#define S3C6400_PLL_MDIV_SHIFT	(16)
#define S3C6400_PLL_PDIV_SHIFT	(8)
#define S3C6400_PLL_SDIV_SHIFT	(0)

#include <asm/div64.h>

static inline unsigned long s3c6400_get_pll(unsigned long baseclk,
					    u32 pllcon)
{
	u32 mdiv, pdiv, sdiv;
	u64 fvco = baseclk;

	mdiv = (pllcon >> S3C6400_PLL_MDIV_SHIFT) & S3C6400_PLL_MDIV_MASK;
	pdiv = (pllcon >> S3C6400_PLL_PDIV_SHIFT) & S3C6400_PLL_PDIV_MASK;
	sdiv = (pllcon >> S3C6400_PLL_SDIV_SHIFT) & S3C6400_PLL_SDIV_MASK;

	fvco *= mdiv;
	do_div(fvco, (pdiv << sdiv));

	return (unsigned long)fvco;
}

#define S3C6400_EPLL_MDIV_MASK	((1 << (23-16)) - 1)
#define S3C6400_EPLL_PDIV_MASK	((1 << (13-8)) - 1)
#define S3C6400_EPLL_SDIV_MASK	((1 << (2-0)) - 1)
#define S3C6400_EPLL_MDIV_SHIFT	(16)
#define S3C6400_EPLL_PDIV_SHIFT	(8)
#define S3C6400_EPLL_SDIV_SHIFT	(0)
#define S3C6400_EPLL_KDIV_MASK  (0xffff)

static inline unsigned long s3c6400_get_epll(unsigned long baseclk)
{
	unsigned long result;
	u32 epll0 = __raw_readl(S3C_EPLL_CON0);
	u32 epll1 = __raw_readl(S3C_EPLL_CON1);
	u32 mdiv, pdiv, sdiv, kdiv;
	u64 tmp;

	mdiv = (epll0 >> S3C6400_EPLL_MDIV_SHIFT) & S3C6400_EPLL_MDIV_MASK;
	pdiv = (epll0 >> S3C6400_EPLL_PDIV_SHIFT) & S3C6400_EPLL_PDIV_MASK;
	sdiv = (epll0 >> S3C6400_EPLL_SDIV_SHIFT) & S3C6400_EPLL_SDIV_MASK;
	kdiv = epll1 & S3C6400_EPLL_KDIV_MASK;

	/* We need to multiple baseclk by mdiv (the integer part) and kdiv
	 * which is in 2^16ths, so shift mdiv up (does not overflow) and
	 * add kdiv before multiplying. The use of tmp is to avoid any
	 * overflows before shifting bac down into result when multipling
	 * by the mdiv and kdiv pair.
	 */

	tmp = baseclk;
	tmp *= (mdiv << 16) + kdiv;
	do_div(tmp, (pdiv << sdiv));
	result = tmp >> 16;

	return result;
}
