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
 * Board-specific setup code for the MIMC200
 *
 * Copyright (C) 2008 Mercury IMC Ltd
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

extern struct atmel_lcdfb_info mimc200_lcdc_data;

#include <linux/clk.h>
#include <linux/etherdevice.h>
#include <linux/i2c-gpio.h>
#include <linux/init.h>
#include <linux/linkage.h>
#include <linux/platform_device.h>
#include <linux/types.h>
#include <linux/leds.h>
#include <linux/spi/spi.h>
#include <linux/spi/eeprom.h>

#include <video/atmel_lcdc.h>
#include <linux/fb.h>

#include <linux/atmel-mci.h>
#include <linux/io.h>
#include <asm/setup.h>

#include <mach/at32ap700x.h>
#include <mach/board.h>
#include <mach/init.h>
#include <mach/portmux.h>

/* Oscillator frequencies. These are board-specific */
unsigned long at32_board_osc_rates[3] = {
	[0] = 32768,	/* 32.768 kHz on RTC osc */
	[1] = 10000000,	/* 10 MHz on osc0 */
	[2] = 12000000,	/* 12 MHz on osc1 */
};

/* Initialized by bootloader-specific startup code. */
struct tag *bootloader_tags __initdata;

static struct fb_videomode __initdata tx14d14_modes[] = {
	{
		.name		= "640x480 @ 60",
		.refresh	= 60,
		.xres		= 640,		.yres		= 480,
		.pixclock	= KHZ2PICOS(11666),

		.left_margin	= 80,		.right_margin	= 1,
		.upper_margin	= 13,		.lower_margin	= 2,
		.hsync_len	= 64,		.vsync_len	= 1,

		.sync		= 0,
		.vmode		= FB_VMODE_NONINTERLACED,
	},
};

static struct fb_monspecs __initdata mimc200_default_monspecs = {
	.manufacturer		= "HIT",
	.monitor		= "TX14D14VM1BAB",
	.modedb			= tx14d14_modes,
	.modedb_len		= ARRAY_SIZE(tx14d14_modes),
	.hfmin			= 14820,
	.hfmax			= 22230,
	.vfmin			= 60,
	.vfmax			= 73.3,
	.dclkmax		= 25200000,
};

struct atmel_lcdfb_info __initdata mimc200_lcdc_data = {
	.default_bpp		= 16,
	.default_dmacon		= ATMEL_LCDC_DMAEN | ATMEL_LCDC_DMA2DEN,
	.default_lcdcon2	= (ATMEL_LCDC_DISTYPE_TFT
				   | ATMEL_LCDC_INVCLK
				   | ATMEL_LCDC_CLKMOD_ALWAYSACTIVE
				   | ATMEL_LCDC_MEMOR_BIG),
	.default_monspecs	= &mimc200_default_monspecs,
	.guard_time		= 2,
};

struct eth_addr {
	u8 addr[6];
};
static struct eth_addr __initdata hw_addr[2];
static struct eth_platform_data __initdata eth_data[2];

static struct spi_eeprom eeprom_25lc010 = {
		.name = "25lc010",
		.byte_len = 128,
		.page_size = 16,
		.flags = EE_ADDR1,
};

static struct spi_board_info spi0_board_info[] __initdata = {
	{
		.modalias	= "rtc-ds1390",
		.max_speed_hz	= 4000000,
		.chip_select	= 2,
	},
	{
		.modalias	= "at25",
		.max_speed_hz	= 1000000,
		.chip_select	= 1,
		.mode		= SPI_MODE_3,
		.platform_data	= &eeprom_25lc010,
	},
};

static struct mci_platform_data __initdata mci0_data = {
	.slot[0] = {
		.bus_width	= 4,
		.detect_pin	= GPIO_PIN_PA(26),
		.wp_pin		= GPIO_PIN_PA(27),
	},
};

/*
 * The next two functions should go away as the boot loader is
 * supposed to initialize the macb address registers with a valid
 * ethernet address. But we need to keep it around for a while until
 * we can be reasonably sure the boot loader does this.
 *
 * The phy_id is ignored as the driver will probe for it.
 */
static int __init parse_tag_ethernet(struct tag *tag)
{
	int i;

	i = tag->u.ethernet.mac_index;
	if (i < ARRAY_SIZE(hw_addr))
		memcpy(hw_addr[i].addr, tag->u.ethernet.hw_address,
		       sizeof(hw_addr[i].addr));

	return 0;
}
__tagtable(ATAG_ETHERNET, parse_tag_ethernet);

static void __init set_hw_addr(struct platform_device *pdev)
{
	struct resource *res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	const u8 *addr;
	void __iomem *regs;
	struct clk *pclk;

	if (!res)
		return;
	if (pdev->id >= ARRAY_SIZE(hw_addr))
		return;

	addr = hw_addr[pdev->id].addr;
	if (!is_valid_ether_addr(addr))
		return;

	/*
	 * Since this is board-specific code, we'll cheat and use the
	 * physical address directly as we happen to know that it's
	 * the same as the virtual address.
	 */
	regs = (void __iomem __force *)res->start;
	pclk = clk_get(&pdev->dev, "pclk");
	if (!pclk)
		return;

	clk_enable(pclk);
	__raw_writel((addr[3] << 24) | (addr[2] << 16)
		     | (addr[1] << 8) | addr[0], regs + 0x98);
	__raw_writel((addr[5] << 8) | addr[4], regs + 0x9c);
	clk_disable(pclk);
	clk_put(pclk);
}

void __init setup_board(void)
{
	at32_map_usart(0, 0);	/* USART 0: /dev/ttyS0 (TTL --> Altera) */
	at32_map_usart(1, 1);	/* USART 1: /dev/ttyS1 (RS232) */
	at32_map_usart(2, 2);	/* USART 2: /dev/ttyS2 (RS485) */
	at32_map_usart(3, 3);	/* USART 3: /dev/ttyS3 (RS422 Multidrop) */
}

static struct i2c_gpio_platform_data i2c_gpio_data = {
	.sda_pin		= GPIO_PIN_PA(6),
	.scl_pin		= GPIO_PIN_PA(7),
	.sda_is_open_drain	= 1,
	.scl_is_open_drain	= 1,
	.udelay			= 2,	/* close to 100 kHz */
};

static struct platform_device i2c_gpio_device = {
	.name		= "i2c-gpio",
	.id		= 0,
	.dev		= {
	.platform_data	= &i2c_gpio_data,
	},
};

static struct i2c_board_info __initdata i2c_info[] = {
};

static int __init mimc200_init(void)
{
	/*
	 * MIMC200 uses 16-bit SDRAM interface, so we don't need to
	 * reserve any pins for it.
	 */

	at32_add_device_usart(0);
	at32_add_device_usart(1);
	at32_add_device_usart(2);
	at32_add_device_usart(3);

	set_hw_addr(at32_add_device_eth(0, &eth_data[0]));
	set_hw_addr(at32_add_device_eth(1, &eth_data[1]));

	at32_add_device_spi(0, spi0_board_info, ARRAY_SIZE(spi0_board_info));
	at32_add_device_mci(0, &mci0_data);
	at32_add_device_usba(0, NULL);

	at32_select_periph(GPIO_PIOB_BASE, 1 << 28, 0, AT32_GPIOF_PULLUP);
	at32_select_gpio(i2c_gpio_data.sda_pin,
		AT32_GPIOF_MULTIDRV | AT32_GPIOF_OUTPUT | AT32_GPIOF_HIGH);
	at32_select_gpio(i2c_gpio_data.scl_pin,
		AT32_GPIOF_MULTIDRV | AT32_GPIOF_OUTPUT | AT32_GPIOF_HIGH);
	platform_device_register(&i2c_gpio_device);
	i2c_register_board_info(0, i2c_info, ARRAY_SIZE(i2c_info));

	at32_add_device_lcdc(0, &mimc200_lcdc_data,
			     fbmem_start, fbmem_size, 1);

	return 0;
}
postcore_initcall(mimc200_init);