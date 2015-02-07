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

/* linux/arch/arm/plat-s3c/include/plat/fb.h
 *
 * Copyright 2008 Openmoko, Inc.
 * Copyright 2008 Simtec Electronics
 *	http://armlinux.simtec.co.uk/
 *	Ben Dooks <ben@simtec.co.uk>
 *
 * S3C - FB platform data definitions
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
*/

#ifndef __PLAT_S3C_FB_H
#define __PLAT_S3C_FB_H __FILE__

/**
 * struct s3c_fb_pd_win - per window setup data
 * @win_mode: The display parameters to initialise (not for window 0)
 * @virtual_x: The virtual X size.
 * @virtual_y: The virtual Y size.
 */
struct s3c_fb_pd_win {
	struct fb_videomode	win_mode;

	unsigned short		default_bpp;
	unsigned short		max_bpp;
	unsigned short		virtual_x;
	unsigned short		virtual_y;
};

/**
 * struct s3c_fb_platdata -  S3C driver platform specific information
 * @setup_gpio: Setup the external GPIO pins to the right state to transfer
 *		the data from the display system to the connected display
 *		device.
 * @vidcon0: The base vidcon0 values to control the panel data format.
 * @vidcon1: The base vidcon1 values to control the panel data output.
 * @win: The setup data for each hardware window, or NULL for unused.
 * @display_mode: The LCD output display mode.
 *
 * The platform data supplies the video driver with all the information
 * it requires to work with the display(s) attached to the machine. It
 * controls the initial mode, the number of display windows (0 is always
 * the base framebuffer) that are initialised etc.
 *
 */
struct s3c_fb_platdata {
	void	(*setup_gpio)(void);

	struct s3c_fb_pd_win	*win[S3C_FB_MAX_WIN];

	u32			 vidcon0;
	u32			 vidcon1;
};

/**
 * s3c_fb_set_platdata() - Setup the FB device with platform data.
 * @pd: The platform data to set. The data is copied from the passed structure
 *      so the machine data can mark the data __initdata so that any unused
 *      machines will end up dumping their data at runtime.
 */
extern void s3c_fb_set_platdata(struct s3c_fb_platdata *pd);

/**
 * s3c64xx_fb_gpio_setup_24bpp() - S3C64XX setup function for 24bpp LCD
 *
 * Initialise the GPIO for an 24bpp LCD display on the RGB interface.
 */
extern void s3c64xx_fb_gpio_setup_24bpp(void);

#endif /* __PLAT_S3C_FB_H */
