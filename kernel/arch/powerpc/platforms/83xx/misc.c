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
 * misc setup functions for MPC83xx
 *
 * Maintainer: Kumar Gala <galak@kernel.crashing.org>
 *
 * This program is free software; you can redistribute  it and/or modify it
 * under  the terms of  the GNU General  Public License as published by the
 * Free Software Foundation;  either version 2 of the  License, or (at your
 * option) any later version.
 */

#include <linux/stddef.h>
#include <linux/kernel.h>

#include <asm/io.h>
#include <asm/hw_irq.h>
#include <sysdev/fsl_soc.h>

#include "mpc83xx.h"

static __be32 __iomem *restart_reg_base;

static int __init mpc83xx_restart_init(void)
{
	/* map reset restart_reg_baseister space */
	restart_reg_base = ioremap(get_immrbase() + 0x900, 0xff);

	return 0;
}

arch_initcall(mpc83xx_restart_init);

void mpc83xx_restart(char *cmd)
{
#define RST_OFFSET	0x00000900
#define RST_PROT_REG	0x00000018
#define RST_CTRL_REG	0x0000001c

	local_irq_disable();

	if (restart_reg_base) {
		/* enable software reset "RSTE" */
		out_be32(restart_reg_base + (RST_PROT_REG >> 2), 0x52535445);

		/* set software hard reset */
		out_be32(restart_reg_base + (RST_CTRL_REG >> 2), 0x2);
	} else {
		printk (KERN_EMERG "Error: Restart registers not mapped, spinning!\n");
	}

	for (;;) ;
}

long __init mpc83xx_time_init(void)
{
#define SPCR_OFFSET	0x00000110
#define SPCR_TBEN	0x00400000
	__be32 __iomem *spcr = ioremap(get_immrbase() + SPCR_OFFSET, 4);
	__be32 tmp;

	tmp = in_be32(spcr);
	out_be32(spcr, tmp | SPCR_TBEN);

	iounmap(spcr);

	return 0;
}
