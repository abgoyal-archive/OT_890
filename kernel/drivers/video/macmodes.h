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
 *  linux/drivers/video/macmodes.h -- Standard MacOS video modes
 *
 *	Copyright (C) 1998 Geert Uytterhoeven
 *
 *  This file is subject to the terms and conditions of the GNU General Public
 *  License. See the file COPYING in the main directory of this archive for
 *  more details.
 */

#ifndef _VIDEO_MACMODES_H
#define _VIDEO_MACMODES_H

    /*
     *  Video mode values.
     *  These are supposed to be the same as the values that Apple uses in
     *  MacOS.
     */

#define VMODE_NVRAM		0
#define VMODE_512_384_60I	1	/* 512x384, 60Hz interlaced (NTSC) */
#define VMODE_512_384_60	2	/* 512x384, 60Hz */
#define VMODE_640_480_50I	3	/* 640x480, 50Hz interlaced (PAL) */
#define VMODE_640_480_60I	4	/* 640x480, 60Hz interlaced (NTSC) */
#define VMODE_640_480_60	5	/* 640x480, 60Hz (VGA) */
#define VMODE_640_480_67	6	/* 640x480, 67Hz */
#define VMODE_640_870_75P	7	/* 640x870, 75Hz (portrait) */
#define VMODE_768_576_50I	8	/* 768x576, 50Hz (PAL full frame) */
#define VMODE_800_600_56	9	/* 800x600, 56Hz */
#define VMODE_800_600_60	10	/* 800x600, 60Hz */
#define VMODE_800_600_72	11	/* 800x600, 72Hz */
#define VMODE_800_600_75	12	/* 800x600, 75Hz */
#define VMODE_832_624_75	13	/* 832x624, 75Hz */
#define VMODE_1024_768_60	14	/* 1024x768, 60Hz */
#define VMODE_1024_768_70	15	/* 1024x768, 70Hz (or 72Hz?) */
#define VMODE_1024_768_75V	16	/* 1024x768, 75Hz (VESA) */
#define VMODE_1024_768_75	17	/* 1024x768, 75Hz */
#define VMODE_1152_870_75	18	/* 1152x870, 75Hz */
#define VMODE_1280_960_75	19	/* 1280x960, 75Hz */
#define VMODE_1280_1024_75	20	/* 1280x1024, 75Hz */
#define VMODE_1152_768_60	21	/* 1152x768, 60Hz     Titanium PowerBook */
#define VMODE_1600_1024_60	22	/* 1600x1024, 60Hz 22" Cinema Display */
#define VMODE_MAX		22
#define VMODE_CHOOSE		99

#define CMODE_NVRAM		-1
#define CMODE_CHOOSE		-2
#define CMODE_8			0	/* 8 bits/pixel */
#define CMODE_16		1	/* 16 (actually 15) bits/pixel */
#define CMODE_32		2	/* 32 (actually 24) bits/pixel */


extern int mac_vmode_to_var(int vmode, int cmode,
			    struct fb_var_screeninfo *var);
extern int mac_var_to_vmode(const struct fb_var_screeninfo *var, int *vmode,
			    int *cmode);
extern int mac_map_monitor_sense(int sense);
extern int mac_find_mode(struct fb_var_screeninfo *var,
			 struct fb_info *info,
			 const char *mode_option,
			 unsigned int default_bpp);


    /*
     *  Addresses in NVRAM where video mode and pixel size are stored.
     */

#define NV_VMODE		0x140f
#define NV_CMODE		0x1410

#endif /* _VIDEO_MACMODES_H */
