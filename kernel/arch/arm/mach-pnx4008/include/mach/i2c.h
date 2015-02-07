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
 * PNX4008-specific tweaks for I2C IP3204 block
 *
 * Author: Vitaly Wool <vwool@ru.mvista.com>
 *
 * 2005 (c) MontaVista Software, Inc. This file is licensed under
 * the terms of the GNU General Public License version 2. This program
 * is licensed "as is" without any warranty of any kind, whether express
 * or implied.
 */

#ifndef __ASM_ARCH_I2C_H__
#define __ASM_ARCH_I2C_H__

enum {
	mstatus_tdi = 0x00000001,
	mstatus_afi = 0x00000002,
	mstatus_nai = 0x00000004,
	mstatus_drmi = 0x00000008,
	mstatus_active = 0x00000020,
	mstatus_scl = 0x00000040,
	mstatus_sda = 0x00000080,
	mstatus_rff = 0x00000100,
	mstatus_rfe = 0x00000200,
	mstatus_tff = 0x00000400,
	mstatus_tfe = 0x00000800,
};

enum {
	mcntrl_tdie = 0x00000001,
	mcntrl_afie = 0x00000002,
	mcntrl_naie = 0x00000004,
	mcntrl_drmie = 0x00000008,
	mcntrl_daie = 0x00000020,
	mcntrl_rffie = 0x00000040,
	mcntrl_tffie = 0x00000080,
	mcntrl_reset = 0x00000100,
	mcntrl_cdbmode = 0x00000400,
};

enum {
	rw_bit = 1 << 0,
	start_bit = 1 << 8,
	stop_bit = 1 << 9,
};

#define I2C_REG_RX(a)	((a)->ioaddr)		/* Rx FIFO reg (RO) */
#define I2C_REG_TX(a)	((a)->ioaddr)		/* Tx FIFO reg (WO) */
#define I2C_REG_STS(a)	((a)->ioaddr + 0x04)	/* Status reg (RO) */
#define I2C_REG_CTL(a)	((a)->ioaddr + 0x08)	/* Ctl reg */
#define I2C_REG_CKL(a)	((a)->ioaddr + 0x0c)	/* Clock divider low */
#define I2C_REG_CKH(a)	((a)->ioaddr + 0x10)	/* Clock divider high */
#define I2C_REG_ADR(a)	((a)->ioaddr + 0x14)	/* I2C address */
#define I2C_REG_RFL(a)	((a)->ioaddr + 0x18)	/* Rx FIFO level (RO) */
#define I2C_REG_TFL(a)	((a)->ioaddr + 0x1c)	/* Tx FIFO level (RO) */
#define I2C_REG_RXB(a)	((a)->ioaddr + 0x20)	/* Num of bytes Rx-ed (RO) */
#define I2C_REG_TXB(a)	((a)->ioaddr + 0x24)	/* Num of bytes Tx-ed (RO) */
#define I2C_REG_TXS(a)	((a)->ioaddr + 0x28)	/* Tx slave FIFO (RO) */
#define I2C_REG_STFL(a)	((a)->ioaddr + 0x2c)	/* Tx slave FIFO level (RO) */

#define HCLK_MHZ		13
#define I2C_CHIP_NAME		"PNX4008-I2C"

#endif				/* __ASM_ARCH_I2C_H___ */
