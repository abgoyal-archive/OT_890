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
 * Toshiba TC6393XB SoC support
 *
 * Copyright(c) 2005-2006 Chris Humbert
 * Copyright(c) 2005 Dirk Opfer
 * Copyright(c) 2005 Ian Molton <spyro@f2s.com>
 * Copyright(c) 2007 Dmitry Baryshkov
 *
 * Based on code written by Sharp/Lineo for 2.4 kernels
 * Based on locomo.c
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#ifndef MFD_TC6393XB_H
#define MFD_TC6393XB_H

#include <linux/fb.h>

/* Also one should provide the CK3P6MI clock */
struct tc6393xb_platform_data {
	u16	scr_pll2cr;	/* PLL2 Control */
	u16	scr_gper;	/* GP Enable */

	int	(*enable)(struct platform_device *dev);
	int	(*disable)(struct platform_device *dev);
	int	(*suspend)(struct platform_device *dev);
	int	(*resume)(struct platform_device *dev);

	int	irq_base;	/* base for subdevice irqs */
	int	gpio_base;
	int	(*setup)(struct platform_device *dev);
	void	(*teardown)(struct platform_device *dev);

	struct tmio_nand_data	*nand_data;
	struct tmio_fb_data	*fb_data;

	unsigned resume_restore : 1; /* make special actions
					to preserve the state
					on suspend/resume */
};

extern int tc6393xb_lcd_mode(struct platform_device *fb,
			     const struct fb_videomode *mode);
extern int tc6393xb_lcd_set_power(struct platform_device *fb, bool on);

/*
 * Relative to irq_base
 */
#define	IRQ_TC6393_NAND		0
#define	IRQ_TC6393_MMC		1
#define	IRQ_TC6393_OHCI		2
#define	IRQ_TC6393_FB		4

#define	TC6393XB_NR_IRQS	8

#endif
