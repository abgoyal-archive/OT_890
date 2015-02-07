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
 *  linux/drivers/video/mfb.c -- Low level frame buffer operations for
 *				 monochrome
 *
 *	Created 5 Apr 1997 by Geert Uytterhoeven
 *
 *  This file is subject to the terms and conditions of the GNU General Public
 *  License.  See the file COPYING in the main directory of this archive for
 *  more details.
 */

#include <linux/module.h>
#include <linux/string.h>
#include <linux/fb.h>

#include "atafb.h"
#include "atafb_utils.h"


    /*
     *  Monochrome
     */

void atafb_mfb_copyarea(struct fb_info *info, u_long next_line,
			int sy, int sx, int dy, int dx,
			int height, int width)
{
	u8 *src, *dest;
	u_int rows;

	if (sx == 0 && dx == 0 && width == next_line) {
		src = (u8 *)info->screen_base + sy * (width >> 3);
		dest = (u8 *)info->screen_base + dy * (width >> 3);
		fb_memmove(dest, src, height * (width >> 3));
	} else if (dy <= sy) {
		src = (u8 *)info->screen_base + sy * next_line + (sx >> 3);
		dest = (u8 *)info->screen_base + dy * next_line + (dx >> 3);
		for (rows = height; rows--;) {
			fb_memmove(dest, src, width >> 3);
			src += next_line;
			dest += next_line;
		}
	} else {
		src = (u8 *)info->screen_base + (sy + height - 1) * next_line + (sx >> 3);
		dest = (u8 *)info->screen_base + (dy + height - 1) * next_line + (dx >> 3);
		for (rows = height; rows--;) {
			fb_memmove(dest, src, width >> 3);
			src -= next_line;
			dest -= next_line;
		}
	}
}

void atafb_mfb_fillrect(struct fb_info *info, u_long next_line, u32 color,
			int sy, int sx, int height, int width)
{
	u8 *dest;
	u_int rows;

	dest = (u8 *)info->screen_base + sy * next_line + (sx >> 3);

	if (sx == 0 && width == next_line) {
		if (color)
			fb_memset255(dest, height * (width >> 3));
		else
			fb_memclear(dest, height * (width >> 3));
	} else {
		for (rows = height; rows--; dest += next_line) {
			if (color)
				fb_memset255(dest, width >> 3);
			else
				fb_memclear_small(dest, width >> 3);
		}
	}
}

void atafb_mfb_linefill(struct fb_info *info, u_long next_line,
			int dy, int dx, u32 width,
			const u8 *data, u32 bgcolor, u32 fgcolor)
{
	u8 *dest;
	u_int rows;

	dest = (u8 *)info->screen_base + dy * next_line + (dx >> 3);

	for (rows = width / 8; rows--; /* check margins */ ) {
		// use fast_memmove or fb_memmove
		*dest++ = *data++;
	}
}

#ifdef MODULE
MODULE_LICENSE("GPL");

int init_module(void)
{
	return 0;
}

void cleanup_module(void)
{
}
#endif /* MODULE */


    /*
     *  Visible symbols for modules
     */

EXPORT_SYMBOL(atafb_mfb_copyarea);
EXPORT_SYMBOL(atafb_mfb_fillrect);
EXPORT_SYMBOL(atafb_mfb_linefill);
