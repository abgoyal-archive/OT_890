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
 *  linux/drivers/video/console/fbcon_rotate.h -- Software Display Rotation
 *
 *	Copyright (C) 2005 Antonino Daplas <adaplas@pol.net>
 *
 *  This file is subject to the terms and conditions of the GNU General Public
 *  License.  See the file COPYING in the main directory of this archive
 *  for more details.
 */

#ifndef _FBCON_ROTATE_H
#define _FBCON_ROTATE_H

#define GETVYRES(s,i) ({                           \
        (s == SCROLL_REDRAW || s == SCROLL_MOVE) ? \
        (i)->var.yres : (i)->var.yres_virtual; })

#define GETVXRES(s,i) ({                           \
        (s == SCROLL_REDRAW || s == SCROLL_MOVE || !(i)->fix.xpanstep) ? \
        (i)->var.xres : (i)->var.xres_virtual; })


static inline int pattern_test_bit(u32 x, u32 y, u32 pitch, const char *pat)
{
	u32 tmp = (y * pitch) + x, index = tmp / 8,  bit = tmp % 8;

	pat +=index;
	return (*pat) & (0x80 >> bit);
}

static inline void pattern_set_bit(u32 x, u32 y, u32 pitch, char *pat)
{
	u32 tmp = (y * pitch) + x, index = tmp / 8, bit = tmp % 8;

	pat += index;

	(*pat) |= 0x80 >> bit;
}

static inline void rotate_ud(const char *in, char *out, u32 width, u32 height)
{
	int i, j;
	int shift = (8 - (width % 8)) & 7;

	width = (width + 7) & ~7;

	for (i = 0; i < height; i++) {
		for (j = 0; j < width; j++) {
			if (pattern_test_bit(j, i, width, in))
				pattern_set_bit(width - (1 + j + shift),
						height - (1 + i),
						width, out);
		}

	}
}

static inline void rotate_cw(const char *in, char *out, u32 width, u32 height)
{
	int i, j, h = height, w = width;
	int shift = (8 - (height % 8)) & 7;

	width = (width + 7) & ~7;
	height = (height + 7) & ~7;

	for (i = 0; i < h; i++) {
		for (j = 0; j < w; j++) {
			if (pattern_test_bit(j, i, width, in))
				pattern_set_bit(height - 1 - i - shift, j,
						height, out);

		}
	}
}

static inline void rotate_ccw(const char *in, char *out, u32 width, u32 height)
{
	int i, j, h = height, w = width;
	int shift = (8 - (width % 8)) & 7;

	width = (width + 7) & ~7;
	height = (height + 7) & ~7;

	for (i = 0; i < h; i++) {
		for (j = 0; j < w; j++) {
			if (pattern_test_bit(j, i, width, in))
				pattern_set_bit(i, width - 1 - j - shift,
						height, out);
		}
	}
}

extern void fbcon_rotate_cw(struct fbcon_ops *ops);
extern void fbcon_rotate_ud(struct fbcon_ops *ops);
extern void fbcon_rotate_ccw(struct fbcon_ops *ops);
#endif
