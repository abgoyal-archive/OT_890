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
 * High-Speed Bus Matrix configuration registers
 *
 * Copyright (C) 2008 Atmel Corporation
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */
#ifndef __HMATRIX_H
#define __HMATRIX_H

extern struct clk at32_hmatrix_clk;

void hmatrix_write_reg(unsigned long offset, u32 value);
u32 hmatrix_read_reg(unsigned long offset);

void hmatrix_sfr_set_bits(unsigned int slave_id, u32 mask);
void hmatrix_sfr_clear_bits(unsigned int slave_id, u32 mask);

/* Master Configuration register */
#define HMATRIX_MCFG(m)			(0x0000 + 4 * (m))
/* Undefined length burst limit */
# define HMATRIX_MCFG_ULBT_INFINITE	0	/* Infinite length */
# define HMATRIX_MCFG_ULBT_SINGLE	1	/* Single Access */
# define HMATRIX_MCFG_ULBT_FOUR_BEAT	2	/* Four beat */
# define HMATRIX_MCFG_ULBT_EIGHT_BEAT	3	/* Eight beat */
# define HMATRIX_MCFG_ULBT_SIXTEEN_BEAT	4	/* Sixteen beat */

/* Slave Configuration register */
#define HMATRIX_SCFG(s)			(0x0040 + 4 * (s))
# define HMATRIX_SCFG_SLOT_CYCLE(x)	((x) <<  0)	/* Max burst cycles */
# define HMATRIX_SCFG_DEFMSTR_NONE	(  0 << 16)	/* No default master */
# define HMATRIX_SCFG_DEFMSTR_LAST	(  1 << 16)	/* Last def master */
# define HMATRIX_SCFG_DEFMSTR_FIXED	(  2 << 16)	/* Fixed def master */
# define HMATRIX_SCFG_FIXED_DEFMSTR(m)	((m) << 18)	/* Fixed master ID */
# define HMATRIX_SCFG_ARBT_ROUND_ROBIN	(  0 << 24)	/* RR arbitration */
# define HMATRIX_SCFG_ARBT_FIXED_PRIO	(  1 << 24)	/* Fixed priority */

/* Slave Priority register A (master 0..7) */
#define HMATRIX_PRAS(s)			(0x0080 + 8 * (s))
# define HMATRIX_PRAS_PRIO(m, p)	((p) << ((m) * 4))

/* Slave Priority register A (master 8..15) */
#define HMATRIX_PRBS(s)			(0x0084 + 8 * (s))
# define HMATRIX_PRBS_PRIO(m, p)	((p) << (((m) - 8) * 4))

/* Master Remap Control Register */
#define HMATRIX_MRCR				0x0100
# define HMATRIX_MRCR_REMAP(m)		(  1 << (m))	/* Remap master m */

/* Special Function Register. Bit definitions are chip-specific */
#define HMATRIX_SFR(s)			(0x0110 + 4 * (s))

#endif /* __HMATRIX_H */
