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
 *  arch/arm/mach-rpc/include/mach/acornfb.h
 *
 *  Copyright (C) 1999 Russell King
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 *  AcornFB architecture specific code
 */

#define acornfb_bandwidth(var) ((var)->pixclock * 8 / (var)->bits_per_pixel)

static inline int
acornfb_valid_pixrate(struct fb_var_screeninfo *var)
{
	u_long limit;

	if (!var->pixclock)
		return 0;

	/*
	 * Limits below are taken from RISC OS bandwidthlimit file
	 */
	if (current_par.using_vram) {
		if (current_par.vram_half_sam == 2048)
			limit = 6578;
		else
			limit = 13157;
	} else {
		limit = 26315;
	}

	return acornfb_bandwidth(var) >= limit;
}

/*
 * Try to find the best PLL parameters for the pixel clock.
 * This algorithm seems to give best predictable results,
 * and produces the same values as detailed in the VIDC20
 * data sheet.
 */
static inline u_int
acornfb_vidc20_find_pll(u_int pixclk)
{
	u_int r, best_r = 2, best_v = 2;
	int best_d = 0x7fffffff;

	for (r = 2; r <= 32; r++) {
		u_int rr, v, p;
		int d;

		rr = 41667 * r;

		v = (rr + pixclk / 2) / pixclk;

		if (v > 32 || v < 2)
			continue;

		p = (rr + v / 2) / v;

		d = pixclk - p;

		if (d < 0)
			d = -d;

		if (d < best_d) {
			best_d = d;
			best_v = v - 1;
			best_r = r - 1;
		}

		if (d == 0)
			break;
	}

	return best_v << 8 | best_r;
}

static inline void
acornfb_vidc20_find_rates(struct vidc_timing *vidc,
			  struct fb_var_screeninfo *var)
{
	u_int div;

	/* Select pixel-clock divisor to keep PLL in range */
	div = var->pixclock / 9090; /*9921*/

	/* Limit divisor */
	if (div == 0)
		div = 1;
	if (div > 8)
		div = 8;

	/* Encode divisor to VIDC20 setting */
	switch (div) {
	case 1:	vidc->control |= VIDC20_CTRL_PIX_CK;  break;
	case 2:	vidc->control |= VIDC20_CTRL_PIX_CK2; break;
	case 3:	vidc->control |= VIDC20_CTRL_PIX_CK3; break;
	case 4:	vidc->control |= VIDC20_CTRL_PIX_CK4; break;
	case 5:	vidc->control |= VIDC20_CTRL_PIX_CK5; break;
	case 6:	vidc->control |= VIDC20_CTRL_PIX_CK6; break;
	case 7:	vidc->control |= VIDC20_CTRL_PIX_CK7; break;
	case 8: vidc->control |= VIDC20_CTRL_PIX_CK8; break;
	}

	/*
	 * With VRAM, the FIFO can be set to the highest possible setting
	 * because there are no latency considerations for other memory
	 * accesses. However, in 64 bit bus mode the FIFO preload value
	 * must not be set to VIDC20_CTRL_FIFO_28 because this will let
	 * the FIFO overflow. See VIDC20 manual page 33 (6.0 Setting the
	 * FIFO preload value).
	 */
	if (current_par.using_vram) {
		if (current_par.vram_half_sam == 2048)
			vidc->control |= VIDC20_CTRL_FIFO_24;
		else
			vidc->control |= VIDC20_CTRL_FIFO_28;
	} else {
		unsigned long bandwidth = acornfb_bandwidth(var);

		/* Encode bandwidth as VIDC20 setting */
		if (bandwidth > 33334)		/* < 30.0MB/s */
			vidc->control |= VIDC20_CTRL_FIFO_16;
		else if (bandwidth > 26666)	/* < 37.5MB/s */
			vidc->control |= VIDC20_CTRL_FIFO_20;
		else if (bandwidth > 22222)	/* < 45.0MB/s */
			vidc->control |= VIDC20_CTRL_FIFO_24;
		else				/* > 45.0MB/s */
			vidc->control |= VIDC20_CTRL_FIFO_28;
	}

	/* Find the PLL values */
	vidc->pll_ctl = acornfb_vidc20_find_pll(var->pixclock / div);
}

#define acornfb_default_control()	(VIDC20_CTRL_PIX_VCLK)
#define acornfb_default_econtrol()	(VIDC20_ECTL_DAC | VIDC20_ECTL_REG(3))
