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
 * linux/drivers/video/fonts.c -- `Soft' font definitions
 *
 *    Created 1995 by Geert Uytterhoeven
 *    Rewritten 1998 by Martin Mares <mj@ucw.cz>
 *
 *	2001 - Documented with DocBook
 *	- Brad Douglas <brad@neruo.com>
 *
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file COPYING in the main directory of this archive
 * for more details.
 */

#include <linux/module.h>
#include <linux/types.h>
#include <linux/string.h>
#if defined(__mc68000__)
#include <asm/setup.h>
#endif
#include <linux/font.h>

#define NO_FONTS

static const struct font_desc *fonts[] = {
#ifdef CONFIG_FONT_8x8
#undef NO_FONTS
    &font_vga_8x8,
#endif
#ifdef CONFIG_FONT_8x16
#undef NO_FONTS
    &font_vga_8x16,
#endif
#ifdef CONFIG_FONT_6x11
#undef NO_FONTS
    &font_vga_6x11,
#endif
#ifdef CONFIG_FONT_7x14
#undef NO_FONTS
    &font_7x14,
#endif
#ifdef CONFIG_FONT_SUN8x16
#undef NO_FONTS
    &font_sun_8x16,
#endif
#ifdef CONFIG_FONT_SUN12x22
#undef NO_FONTS
    &font_sun_12x22,
#endif
#ifdef CONFIG_FONT_10x18
#undef NO_FONTS
    &font_10x18,
#endif
#ifdef CONFIG_FONT_ACORN_8x8
#undef NO_FONTS
    &font_acorn_8x8,
#endif
#ifdef CONFIG_FONT_PEARL_8x8
#undef NO_FONTS
    &font_pearl_8x8,
#endif
#ifdef CONFIG_FONT_MINI_4x6
#undef NO_FONTS
    &font_mini_4x6,
#endif
};

#define num_fonts ARRAY_SIZE(fonts)

#ifdef NO_FONTS
#error No fonts configured.
#endif


/**
 *	find_font - find a font
 *	@name: string name of a font
 *
 *	Find a specified font with string name @name.
 *
 *	Returns %NULL if no font found, or a pointer to the
 *	specified font.
 *
 */

const struct font_desc *find_font(const char *name)
{
   unsigned int i;

   for (i = 0; i < num_fonts; i++)
      if (!strcmp(fonts[i]->name, name))
	  return fonts[i];
   return NULL;
}


/**
 *	get_default_font - get default font
 *	@xres: screen size of X
 *	@yres: screen size of Y
 *      @font_w: bit array of supported widths (1 - 32)
 *      @font_h: bit array of supported heights (1 - 32)
 *
 *	Get the default font for a specified screen size.
 *	Dimensions are in pixels.
 *
 *	Returns %NULL if no font is found, or a pointer to the
 *	chosen font.
 *
 */

const struct font_desc *get_default_font(int xres, int yres, u32 font_w,
					 u32 font_h)
{
    int i, c, cc;
    const struct font_desc *f, *g;

    g = NULL;
    cc = -10000;
    for(i=0; i<num_fonts; i++) {
	f = fonts[i];
	c = f->pref;
#if defined(__mc68000__)
#ifdef CONFIG_FONT_PEARL_8x8
	if (MACH_IS_AMIGA && f->idx == PEARL8x8_IDX)
	    c = 100;
#endif
#ifdef CONFIG_FONT_6x11
	if (MACH_IS_MAC && xres < 640 && f->idx == VGA6x11_IDX)
	    c = 100;
#endif
#endif
	if ((yres < 400) == (f->height <= 8))
	    c += 1000;

	if ((font_w & (1 << (f->width - 1))) &&
	    (font_h & (1 << (f->height - 1))))
	    c += 1000;

	if (c > cc) {
	    cc = c;
	    g = f;
	}
    }
    return g;
}

EXPORT_SYMBOL(find_font);
EXPORT_SYMBOL(get_default_font);

MODULE_AUTHOR("James Simmons <jsimmons@users.sf.net>");
MODULE_DESCRIPTION("Console Fonts");
MODULE_LICENSE("GPL");
