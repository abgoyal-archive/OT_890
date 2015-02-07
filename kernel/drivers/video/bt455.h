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
 *	linux/drivers/video/bt455.h
 *
 *	Copyright 2003  Thiemo Seufer <seufer@csv.ica.uni-stuttgart.de>
 *
 *	This file is subject to the terms and conditions of the GNU General
 *	Public License. See the file COPYING in the main directory of this
 *	archive for more details.
 */
#include <linux/types.h>
#include <asm/system.h>

/*
 * Bt455 byte-wide registers, 32-bit aligned.
 */
struct bt455_regs {
	volatile u8 addr_cmap;
	u8 pad0[3];
	volatile u8 addr_cmap_data;
	u8 pad1[3];
	volatile u8 addr_clr;
	u8 pad2[3];
	volatile u8 addr_ovly;
	u8 pad3[3];
};

static inline void bt455_select_reg(struct bt455_regs *regs, int ir)
{
	mb();
	regs->addr_cmap = ir & 0x0f;
}

/*
 * Read/write to a Bt455 color map register.
 */
static inline void bt455_read_cmap_entry(struct bt455_regs *regs, int cr,
					 u8* red, u8* green, u8* blue)
{
	bt455_select_reg(regs, cr);
	mb();
	*red = regs->addr_cmap_data & 0x0f;
	rmb();
	*green = regs->addr_cmap_data & 0x0f;
	rmb();
	*blue = regs->addr_cmap_data & 0x0f;
}

static inline void bt455_write_cmap_entry(struct bt455_regs *regs, int cr,
					  u8 red, u8 green, u8 blue)
{
	bt455_select_reg(regs, cr);
	wmb();
	regs->addr_cmap_data = red & 0x0f;
	wmb();
	regs->addr_cmap_data = green & 0x0f;
	wmb();
	regs->addr_cmap_data = blue & 0x0f;
}

static inline void bt455_write_ovly_entry(struct bt455_regs *regs, int cr,
					  u8 red, u8 green, u8 blue)
{
	bt455_select_reg(regs, cr);
	wmb();
	regs->addr_ovly = red & 0x0f;
	wmb();
	regs->addr_ovly = green & 0x0f;
	wmb();
	regs->addr_ovly = blue & 0x0f;
}

static inline void bt455_set_cursor(struct bt455_regs *regs)
{
	mb();
	regs->addr_ovly = 0x0f;
	wmb();
	regs->addr_ovly = 0x0f;
	wmb();
	regs->addr_ovly = 0x0f;
}

static inline void bt455_erase_cursor(struct bt455_regs *regs)
{
	/* bt455_write_cmap_entry(regs, 8, 0x00, 0x00, 0x00); */
	/* bt455_write_cmap_entry(regs, 9, 0x00, 0x00, 0x00); */
	bt455_write_ovly_entry(regs, 8, 0x03, 0x03, 0x03);
	bt455_write_ovly_entry(regs, 9, 0x07, 0x07, 0x07);

	wmb();
	regs->addr_ovly = 0x09;
	wmb();
	regs->addr_ovly = 0x09;
	wmb();
	regs->addr_ovly = 0x09;
}
