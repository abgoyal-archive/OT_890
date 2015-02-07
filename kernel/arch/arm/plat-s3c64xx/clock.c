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

/* linux/arch/arm/plat-s3c64xx/clock.c
 *
 * Copyright 2008 Openmoko, Inc.
 * Copyright 2008 Simtec Electronics
 *	Ben Dooks <ben@simtec.co.uk>
 *	http://armlinux.simtec.co.uk/
 *
 * S3C64XX Base clock support
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
*/

#include <linux/init.h>
#include <linux/module.h>
#include <linux/interrupt.h>
#include <linux/ioport.h>
#include <linux/io.h>

#include <mach/hardware.h>
#include <mach/map.h>

#include <plat/regs-sys.h>
#include <plat/regs-clock.h>
#include <plat/cpu.h>
#include <plat/devs.h>
#include <plat/clock.h>

struct clk clk_27m = {
	.name		= "clk_27m",
	.id		= -1,
	.rate		= 27000000,
};

static int clk_48m_ctrl(struct clk *clk, int enable)
{
	unsigned long flags;
	u32 val;

	/* can't rely on clock lock, this register has other usages */
	local_irq_save(flags);

	val = __raw_readl(S3C64XX_OTHERS);
	if (enable)
		val |= S3C64XX_OTHERS_USBMASK;
	else
		val &= ~S3C64XX_OTHERS_USBMASK;

	__raw_writel(val, S3C64XX_OTHERS);
	local_irq_restore(flags);

	return 0;
}

struct clk clk_48m = {
	.name		= "clk_48m",
	.id		= -1,
	.rate		= 48000000,
	.enable		= clk_48m_ctrl,
};

static int inline s3c64xx_gate(void __iomem *reg,
				struct clk *clk,
				int enable)
{
	unsigned int ctrlbit = clk->ctrlbit;
	u32 con;

	con = __raw_readl(reg);

	if (enable)
		con |= ctrlbit;
	else
		con &= ~ctrlbit;

	__raw_writel(con, reg);
	return 0;
}

static int s3c64xx_pclk_ctrl(struct clk *clk, int enable)
{
	return s3c64xx_gate(S3C_PCLK_GATE, clk, enable);
}

static int s3c64xx_hclk_ctrl(struct clk *clk, int enable)
{
	return s3c64xx_gate(S3C_HCLK_GATE, clk, enable);
}

int s3c64xx_sclk_ctrl(struct clk *clk, int enable)
{
	return s3c64xx_gate(S3C_SCLK_GATE, clk, enable);
}

static struct clk init_clocks_disable[] = {
	{
		.name		= "nand",
		.id		= -1,
		.parent		= &clk_h,
	}, {
		.name		= "adc",
		.id		= -1,
		.parent		= &clk_p,
		.enable		= s3c64xx_pclk_ctrl,
		.ctrlbit	= S3C_CLKCON_PCLK_TSADC,
	}, {
		.name		= "i2c",
		.id		= -1,
		.parent		= &clk_p,
		.enable		= s3c64xx_pclk_ctrl,
		.ctrlbit	= S3C_CLKCON_PCLK_IIC,
	}, {
		.name		= "iis",
		.id		= 0,
		.parent		= &clk_p,
		.enable		= s3c64xx_pclk_ctrl,
		.ctrlbit	= S3C_CLKCON_PCLK_IIS0,
	}, {
		.name		= "iis",
		.id		= 1,
		.parent		= &clk_p,
		.enable		= s3c64xx_pclk_ctrl,
		.ctrlbit	= S3C_CLKCON_PCLK_IIS1,
	}, {
		.name		= "spi",
		.id		= 0,
		.parent		= &clk_p,
		.enable		= s3c64xx_pclk_ctrl,
		.ctrlbit	= S3C_CLKCON_PCLK_SPI0,
	}, {
		.name		= "spi",
		.id		= 1,
		.parent		= &clk_p,
		.enable		= s3c64xx_pclk_ctrl,
		.ctrlbit	= S3C_CLKCON_PCLK_SPI1,
	}, {
		.name		= "48m",
		.id		= 0,
		.parent		= &clk_48m,
		.enable		= s3c64xx_sclk_ctrl,
		.ctrlbit	= S3C_CLKCON_SCLK_MMC0_48,
	}, {
		.name		= "48m",
		.id		= 1,
		.parent		= &clk_48m,
		.enable		= s3c64xx_sclk_ctrl,
		.ctrlbit	= S3C_CLKCON_SCLK_MMC1_48,
	}, {
		.name		= "48m",
		.id		= 2,
		.parent		= &clk_48m,
		.enable		= s3c64xx_sclk_ctrl,
		.ctrlbit	= S3C_CLKCON_SCLK_MMC2_48,
	},
};

static struct clk init_clocks[] = {
	{
		.name		= "lcd",
		.id		= -1,
		.parent		= &clk_h,
		.enable		= s3c64xx_hclk_ctrl,
		.ctrlbit	= S3C_CLKCON_HCLK_LCD,
	}, {
		.name		= "gpio",
		.id		= -1,
		.parent		= &clk_p,
		.enable		= s3c64xx_pclk_ctrl,
		.ctrlbit	= S3C_CLKCON_PCLK_GPIO,
	}, {
		.name		= "usb-host",
		.id		= -1,
		.parent		= &clk_h,
		.enable		= s3c64xx_hclk_ctrl,
		.ctrlbit	= S3C_CLKCON_SCLK_UHOST,
	}, {
		.name		= "hsmmc",
		.id		= 0,
		.parent		= &clk_h,
		.enable		= s3c64xx_hclk_ctrl,
		.ctrlbit	= S3C_CLKCON_HCLK_HSMMC0,
	}, {
		.name		= "hsmmc",
		.id		= 1,
		.parent		= &clk_h,
		.enable		= s3c64xx_hclk_ctrl,
		.ctrlbit	= S3C_CLKCON_HCLK_HSMMC1,
	}, {
		.name		= "hsmmc",
		.id		= 2,
		.parent		= &clk_h,
		.enable		= s3c64xx_hclk_ctrl,
		.ctrlbit	= S3C_CLKCON_HCLK_HSMMC2,
	}, {
		.name		= "timers",
		.id		= -1,
		.parent		= &clk_p,
		.enable		= s3c64xx_pclk_ctrl,
		.ctrlbit	= S3C_CLKCON_PCLK_PWM,
	}, {
		.name		= "uart",
		.id		= 0,
		.parent		= &clk_p,
		.enable		= s3c64xx_pclk_ctrl,
		.ctrlbit	= S3C_CLKCON_PCLK_UART0,
	}, {
		.name		= "uart",
		.id		= 1,
		.parent		= &clk_p,
		.enable		= s3c64xx_pclk_ctrl,
		.ctrlbit	= S3C_CLKCON_PCLK_UART1,
	}, {
		.name		= "uart",
		.id		= 2,
		.parent		= &clk_p,
		.enable		= s3c64xx_pclk_ctrl,
		.ctrlbit	= S3C_CLKCON_PCLK_UART2,
	}, {
		.name		= "uart",
		.id		= 3,
		.parent		= &clk_p,
		.enable		= s3c64xx_pclk_ctrl,
		.ctrlbit	= S3C_CLKCON_PCLK_UART3,
	}, {
		.name		= "rtc",
		.id		= -1,
		.parent		= &clk_p,
		.enable		= s3c64xx_pclk_ctrl,
		.ctrlbit	= S3C_CLKCON_PCLK_RTC,
	}, {
		.name		= "watchdog",
		.id		= -1,
		.parent		= &clk_p,
		.ctrlbit	= S3C_CLKCON_PCLK_WDT,
	}, {
		.name		= "ac97",
		.id		= -1,
		.parent		= &clk_p,
		.ctrlbit	= S3C_CLKCON_PCLK_AC97,
	}
};

static struct clk *clks[] __initdata = {
	&clk_ext,
	&clk_epll,
	&clk_27m,
	&clk_48m,
};

void __init s3c64xx_register_clocks(void)
{
	struct clk *clkp;
	int ret;
	int ptr;

	s3c24xx_register_clocks(clks, ARRAY_SIZE(clks));

	clkp = init_clocks;
	for (ptr = 0; ptr < ARRAY_SIZE(init_clocks); ptr++, clkp++) {
		ret = s3c24xx_register_clock(clkp);
		if (ret < 0) {
			printk(KERN_ERR "Failed to register clock %s (%d)\n",
			       clkp->name, ret);
		}
	}

	clkp = init_clocks_disable;
	for (ptr = 0; ptr < ARRAY_SIZE(init_clocks_disable); ptr++, clkp++) {

		ret = s3c24xx_register_clock(clkp);
		if (ret < 0) {
			printk(KERN_ERR "Failed to register clock %s (%d)\n",
			       clkp->name, ret);
		}

		(clkp->enable)(clkp, 0);
	}

	s3c_pwmclk_init();
}
