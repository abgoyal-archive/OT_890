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
 * Copyright 2005-2007 Freescale Semiconductor, Inc. All Rights Reserved.
 * Copyright 2008 Juergen Beisert, kernel@pengutronix.de
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA  02110-1301, USA.
 */

#ifndef __ASM_ARCH_MXC_CLOCK_H__
#define __ASM_ARCH_MXC_CLOCK_H__

#ifndef __ASSEMBLY__
#include <linux/list.h>

struct module;

struct clk {
	struct list_head node;
	struct module *owner;
	const char *name;
	int id;
	/* Source clock this clk depends on */
	struct clk *parent;
	/* Secondary clock to enable/disable with this clock */
	struct clk *secondary;
	/* Reference count of clock enable/disable */
	__s8 usecount;
	/* Register bit position for clock's enable/disable control. */
	u8 enable_shift;
	/* Register address for clock's enable/disable control. */
	void __iomem *enable_reg;
	u32 flags;
	/* get the current clock rate (always a fresh value) */
	unsigned long (*get_rate) (struct clk *);
	/* Function ptr to set the clock to a new rate. The rate must match a
	   supported rate returned from round_rate. Leave blank if clock is not
	   programmable */
	int (*set_rate) (struct clk *, unsigned long);
	/* Function ptr to round the requested clock rate to the nearest
	   supported rate that is less than or equal to the requested rate. */
	unsigned long (*round_rate) (struct clk *, unsigned long);
	/* Function ptr to enable the clock. Leave blank if clock can not
	   be gated. */
	int (*enable) (struct clk *);
	/* Function ptr to disable the clock. Leave blank if clock can not
	   be gated. */
	void (*disable) (struct clk *);
	/* Function ptr to set the parent clock of the clock. */
	int (*set_parent) (struct clk *, struct clk *);
};

int clk_register(struct clk *clk);
void clk_unregister(struct clk *clk);

#endif /* __ASSEMBLY__ */
#endif /* __ASM_ARCH_MXC_CLOCK_H__ */
