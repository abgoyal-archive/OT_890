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
 * High-Speed Bus Matrix helper functions
 *
 * Copyright (C) 2008 Atmel Corporation
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */
#include <linux/clk.h>
#include <linux/io.h>

#include <mach/chip.h>
#include <mach/hmatrix.h>

static inline void __hmatrix_write_reg(unsigned long offset, u32 value)
{
	__raw_writel(value, (void __iomem __force *)(HMATRIX_BASE + offset));
}

static inline u32 __hmatrix_read_reg(unsigned long offset)
{
	return __raw_readl((void __iomem __force *)(HMATRIX_BASE + offset));
}

/**
 * hmatrix_write_reg - write HMATRIX configuration register
 * @offset: register offset
 * @value: value to be written to the register at @offset
 */
void hmatrix_write_reg(unsigned long offset, u32 value)
{
	clk_enable(&at32_hmatrix_clk);
	__hmatrix_write_reg(offset, value);
	__hmatrix_read_reg(offset);
	clk_disable(&at32_hmatrix_clk);
}

/**
 * hmatrix_read_reg - read HMATRIX configuration register
 * @offset: register offset
 *
 * Returns the value of the register at @offset.
 */
u32 hmatrix_read_reg(unsigned long offset)
{
	u32 value;

	clk_enable(&at32_hmatrix_clk);
	value = __hmatrix_read_reg(offset);
	clk_disable(&at32_hmatrix_clk);

	return value;
}

/**
 * hmatrix_sfr_set_bits - set bits in a slave's Special Function Register
 * @slave_id: operate on the SFR belonging to this slave
 * @mask: mask of bits to be set in the SFR
 */
void hmatrix_sfr_set_bits(unsigned int slave_id, u32 mask)
{
	u32 value;

	clk_enable(&at32_hmatrix_clk);
	value = __hmatrix_read_reg(HMATRIX_SFR(slave_id));
	value |= mask;
	__hmatrix_write_reg(HMATRIX_SFR(slave_id), value);
	__hmatrix_read_reg(HMATRIX_SFR(slave_id));
	clk_disable(&at32_hmatrix_clk);
}

/**
 * hmatrix_sfr_set_bits - clear bits in a slave's Special Function Register
 * @slave_id: operate on the SFR belonging to this slave
 * @mask: mask of bits to be cleared in the SFR
 */
void hmatrix_sfr_clear_bits(unsigned int slave_id, u32 mask)
{
	u32 value;

	clk_enable(&at32_hmatrix_clk);
	value = __hmatrix_read_reg(HMATRIX_SFR(slave_id));
	value &= ~mask;
	__hmatrix_write_reg(HMATRIX_SFR(slave_id), value);
	__hmatrix_read_reg(HMATRIX_SFR(slave_id));
	clk_disable(&at32_hmatrix_clk);
}
