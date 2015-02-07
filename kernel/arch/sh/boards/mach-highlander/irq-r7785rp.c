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
 * Renesas Solutions Highlander R7785RP Support.
 *
 * Copyright (C) 2002  Atom Create Engineering Co., Ltd.
 * Copyright (C) 2006 - 2008  Paul Mundt
 * Copyright (C) 2007  Magnus Damm
 *
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 */
#include <linux/init.h>
#include <linux/irq.h>
#include <linux/io.h>
#include <mach/highlander.h>

enum {
	UNUSED = 0,

	/* FPGA specific interrupt sources */
	CF,		/* Compact Flash */
	SMBUS,		/* SMBUS */
	TP,		/* Touch panel */
	RTC,		/* RTC Alarm */
	TH_ALERT,	/* Temperature sensor */
	AX88796,	/* Ethernet controller */

	/* external bus connector */
	EXT0, EXT1, EXT2, EXT3, EXT4, EXT5, EXT6, EXT7,
};

static struct intc_vect vectors[] __initdata = {
	INTC_IRQ(CF, IRQ_CF),
	INTC_IRQ(SMBUS, IRQ_SMBUS),
	INTC_IRQ(TP, IRQ_TP),
	INTC_IRQ(RTC, IRQ_RTC),
	INTC_IRQ(TH_ALERT, IRQ_TH_ALERT),

	INTC_IRQ(EXT0, IRQ_EXT0), INTC_IRQ(EXT1, IRQ_EXT1),
	INTC_IRQ(EXT2, IRQ_EXT2), INTC_IRQ(EXT3, IRQ_EXT3),

	INTC_IRQ(EXT4, IRQ_EXT4), INTC_IRQ(EXT5, IRQ_EXT5),
	INTC_IRQ(EXT6, IRQ_EXT6), INTC_IRQ(EXT7, IRQ_EXT7),

	INTC_IRQ(AX88796, IRQ_AX88796),
};

static struct intc_mask_reg mask_registers[] __initdata = {
	{ 0xa4000010, 0, 16, /* IRLMCR1 */
	  { 0, 0, 0, 0, CF, AX88796, SMBUS, TP,
	    RTC, 0, TH_ALERT, 0, 0, 0, 0, 0 } },
	{ 0xa4000012, 0, 16, /* IRLMCR2 */
	  { 0, 0, 0, 0, 0, 0, 0, 0,
	    EXT7, EXT6, EXT5, EXT4, EXT3, EXT2, EXT1, EXT0 } },
};

static unsigned char irl2irq[HL_NR_IRL] __initdata = {
	0, IRQ_CF, IRQ_EXT4, IRQ_EXT5,
	IRQ_EXT6, IRQ_EXT7, IRQ_SMBUS, IRQ_TP,
	IRQ_RTC, IRQ_TH_ALERT, IRQ_AX88796, IRQ_EXT0,
	IRQ_EXT1, IRQ_EXT2, IRQ_EXT3,
};

static DECLARE_INTC_DESC(intc_desc, "r7785rp", vectors,
			 NULL, mask_registers, NULL, NULL);

unsigned char * __init highlander_plat_irq_setup(void)
{
	if ((ctrl_inw(0xa4000158) & 0xf000) != 0x1000)
		return NULL;

	printk(KERN_INFO "Using r7785rp interrupt controller.\n");

	ctrl_outw(0x0000, PA_IRLSSR1);	/* FPGA IRLSSR1(CF_CD clear) */

	/* Setup the FPGA IRL */
	ctrl_outw(0x0000, PA_IRLPRA);	/* FPGA IRLA */
	ctrl_outw(0xe598, PA_IRLPRB);	/* FPGA IRLB */
	ctrl_outw(0x7060, PA_IRLPRC);	/* FPGA IRLC */
	ctrl_outw(0x0000, PA_IRLPRD);	/* FPGA IRLD */
	ctrl_outw(0x4321, PA_IRLPRE);	/* FPGA IRLE */
	ctrl_outw(0xdcba, PA_IRLPRF);	/* FPGA IRLF */

	register_intc_controller(&intc_desc);
	return irl2irq;
}
