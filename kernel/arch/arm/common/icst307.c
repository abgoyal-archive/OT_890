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
 *  linux/arch/arm/common/icst307.c
 *
 *  Copyright (C) 2003 Deep Blue Solutions, Ltd, All Rights Reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 *  Support functions for calculating clocks/divisors for the ICST307
 *  clock generators.  See http://www.icst.com/ for more information
 *  on these devices.
 *
 *  This is an almost identical implementation to the ICST525 clock generator.
 *  The s2div and idx2s files are different
 */
#include <linux/module.h>
#include <linux/kernel.h>

#include <asm/hardware/icst307.h>

/*
 * Divisors for each OD setting.
 */
static unsigned char s2div[8] = { 10, 2, 8, 4, 5, 7, 3, 6 };

unsigned long icst307_khz(const struct icst307_params *p, struct icst307_vco vco)
{
	return p->ref * 2 * (vco.v + 8) / ((vco.r + 2) * s2div[vco.s]);
}

EXPORT_SYMBOL(icst307_khz);

/*
 * Ascending divisor S values.
 */
static unsigned char idx2s[8] = { 1, 6, 3, 4, 7, 5, 2, 0 };

struct icst307_vco
icst307_khz_to_vco(const struct icst307_params *p, unsigned long freq)
{
	struct icst307_vco vco = { .s = 1, .v = p->vd_max, .r = p->rd_max };
	unsigned long f;
	unsigned int i = 0, rd, best = (unsigned int)-1;

	/*
	 * First, find the PLL output divisor such
	 * that the PLL output is within spec.
	 */
	do {
		f = freq * s2div[idx2s[i]];

		/*
		 * f must be between 6MHz and 200MHz (3.3 or 5V)
		 */
		if (f > 6000 && f <= p->vco_max)
			break;
	} while (i < ARRAY_SIZE(idx2s));

	if (i >= ARRAY_SIZE(idx2s))
		return vco;

	vco.s = idx2s[i];

	/*
	 * Now find the closest divisor combination
	 * which gives a PLL output of 'f'.
	 */
	for (rd = p->rd_min; rd <= p->rd_max; rd++) {
		unsigned long fref_div, f_pll;
		unsigned int vd;
		int f_diff;

		fref_div = (2 * p->ref) / rd;

		vd = (f + fref_div / 2) / fref_div;
		if (vd < p->vd_min || vd > p->vd_max)
			continue;

		f_pll = fref_div * vd;
		f_diff = f_pll - f;
		if (f_diff < 0)
			f_diff = -f_diff;

		if ((unsigned)f_diff < best) {
			vco.v = vd - 8;
			vco.r = rd - 2;
			if (f_diff == 0)
				break;
			best = f_diff;
		}
	}

	return vco;
}

EXPORT_SYMBOL(icst307_khz_to_vco);

struct icst307_vco
icst307_ps_to_vco(const struct icst307_params *p, unsigned long period)
{
	struct icst307_vco vco = { .s = 1, .v = p->vd_max, .r = p->rd_max };
	unsigned long f, ps;
	unsigned int i = 0, rd, best = (unsigned int)-1;

	ps = 1000000000UL / p->vco_max;

	/*
	 * First, find the PLL output divisor such
	 * that the PLL output is within spec.
	 */
	do {
		f = period / s2div[idx2s[i]];

		/*
		 * f must be between 6MHz and 200MHz (3.3 or 5V)
		 */
		if (f >= ps && f < 1000000000UL / 6000 + 1)
			break;
	} while (i < ARRAY_SIZE(idx2s));

	if (i >= ARRAY_SIZE(idx2s))
		return vco;

	vco.s = idx2s[i];

	ps = 500000000UL / p->ref;

	/*
	 * Now find the closest divisor combination
	 * which gives a PLL output of 'f'.
	 */
	for (rd = p->rd_min; rd <= p->rd_max; rd++) {
		unsigned long f_in_div, f_pll;
		unsigned int vd;
		int f_diff;

		f_in_div = ps * rd;

		vd = (f_in_div + f / 2) / f;
		if (vd < p->vd_min || vd > p->vd_max)
			continue;

		f_pll = (f_in_div + vd / 2) / vd;
		f_diff = f_pll - f;
		if (f_diff < 0)
			f_diff = -f_diff;

		if ((unsigned)f_diff < best) {
			vco.v = vd - 8;
			vco.r = rd - 2;
			if (f_diff == 0)
				break;
			best = f_diff;
		}
	}

	return vco;
}

EXPORT_SYMBOL(icst307_ps_to_vco);
