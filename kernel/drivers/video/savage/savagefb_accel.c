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

/*-*- linux-c -*-
 *  linux/drivers/video/savage/savage_accel.c -- Hardware Acceleration
 *
 *      Copyright (C) 2004 Antonino Daplas<adaplas@pol.net>
 *      All Rights Reserved
 *
 *  This file is subject to the terms and conditions of the GNU General Public
 *  License. See the file COPYING in the main directory of this archive for
 *  more details.
 */
#include <linux/kernel.h>
#include <linux/string.h>
#include <linux/fb.h>

#include "savagefb.h"

static u32 savagefb_rop[] = {
	0xCC, /* ROP_COPY */
	0x5A  /* ROP_XOR  */
};

int savagefb_sync(struct fb_info *info)
{
	struct savagefb_par *par = info->par;

	par->SavageWaitIdle(par);
	return 0;
}

void savagefb_copyarea(struct fb_info *info, const struct fb_copyarea *region)
{
	struct savagefb_par *par = info->par;
	int sx = region->sx, dx = region->dx;
	int sy = region->sy, dy = region->dy;
	int cmd;

	if (!region->width || !region->height)
		return;
	par->bci_ptr = 0;
	cmd = BCI_CMD_RECT | BCI_CMD_DEST_GBD | BCI_CMD_SRC_GBD;
	BCI_CMD_SET_ROP(cmd, savagefb_rop[0]);

	if (dx <= sx) {
		cmd |= BCI_CMD_RECT_XP;
	} else {
		sx += region->width - 1;
		dx += region->width - 1;
	}

	if (dy <= sy) {
		cmd |= BCI_CMD_RECT_YP;
	} else {
		sy += region->height - 1;
		dy += region->height - 1;
	}

	par->SavageWaitFifo(par,4);
	BCI_SEND(cmd);
	BCI_SEND(BCI_X_Y(sx, sy));
	BCI_SEND(BCI_X_Y(dx, dy));
	BCI_SEND(BCI_W_H(region->width, region->height));
}

void savagefb_fillrect(struct fb_info *info, const struct fb_fillrect *rect)
{
	struct savagefb_par *par = info->par;
	int cmd, color;

	if (!rect->width || !rect->height)
		return;

	if (info->fix.visual == FB_VISUAL_PSEUDOCOLOR)
		color = rect->color;
	else
		color = ((u32 *)info->pseudo_palette)[rect->color];

	cmd = BCI_CMD_RECT | BCI_CMD_RECT_XP | BCI_CMD_RECT_YP |
	      BCI_CMD_DEST_GBD | BCI_CMD_SRC_SOLID |
	      BCI_CMD_SEND_COLOR;

	par->bci_ptr = 0;
	BCI_CMD_SET_ROP(cmd, savagefb_rop[rect->rop]);

	par->SavageWaitFifo(par,4);
	BCI_SEND(cmd);
	BCI_SEND(color);
	BCI_SEND( BCI_X_Y(rect->dx, rect->dy) );
	BCI_SEND( BCI_W_H(rect->width, rect->height) );
}

void savagefb_imageblit(struct fb_info *info, const struct fb_image *image)
{
	struct savagefb_par *par = info->par;
	int fg, bg, size, i, width;
	int cmd;
	u32 *src = (u32 *) image->data;

	if (!image->width || !image->height)
		return;

	if (image->depth != 1) {
		cfb_imageblit(info, image);
		return;
	}

	if (info->fix.visual == FB_VISUAL_PSEUDOCOLOR) {
		fg = image->fg_color;
		bg = image->bg_color;
	} else {
		fg = ((u32 *)info->pseudo_palette)[image->fg_color];
		bg = ((u32 *)info->pseudo_palette)[image->bg_color];
	}

	cmd = BCI_CMD_RECT | BCI_CMD_RECT_XP | BCI_CMD_RECT_YP |
	      BCI_CMD_CLIP_LR | BCI_CMD_DEST_GBD | BCI_CMD_SRC_MONO |
	      BCI_CMD_SEND_COLOR;

	par->bci_ptr = 0;
	BCI_CMD_SET_ROP(cmd, savagefb_rop[0]);

	width = (image->width + 31) & ~31;
	size = (width * image->height)/8;
	size >>= 2;

	par->SavageWaitFifo(par, size + 5);
	BCI_SEND(cmd);
	BCI_SEND(BCI_CLIP_LR(image->dx, image->dx + image->width - 1));
	BCI_SEND(fg);
	BCI_SEND(bg);
	BCI_SEND(BCI_X_Y(image->dx, image->dy));
	BCI_SEND(BCI_W_H(width, image->height));
	for (i = 0; i < size; i++)
		BCI_SEND(src[i]);
}

MODULE_LICENSE("GPL");
