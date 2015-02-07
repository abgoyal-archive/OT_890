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
 * Pb1200/DBAu1200 board platform device registration
 *
 * Copyright (C) 2008 MontaVista Software Inc. <source@mvista.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */

#include <linux/dma-mapping.h>
#include <linux/init.h>
#include <linux/leds.h>
#include <linux/platform_device.h>

#include <asm/mach-au1x00/au1xxx.h>
#include <asm/mach-au1x00/au1100_mmc.h>

static int mmc_activity;

static void pb1200mmc0_set_power(void *mmc_host, int state)
{
	if (state)
		bcsr->board |= BCSR_BOARD_SD0PWR;
	else
		bcsr->board &= ~BCSR_BOARD_SD0PWR;

	au_sync_delay(1);
}

static int pb1200mmc0_card_readonly(void *mmc_host)
{
	return (bcsr->status & BCSR_STATUS_SD0WP) ? 1 : 0;
}

static int pb1200mmc0_card_inserted(void *mmc_host)
{
	return (bcsr->sig_status & BCSR_INT_SD0INSERT) ? 1 : 0;
}

static void pb1200_mmcled_set(struct led_classdev *led,
			enum led_brightness brightness)
{
	if (brightness != LED_OFF) {
		if (++mmc_activity == 1)
			bcsr->disk_leds &= ~(1 << 8);
	} else {
		if (--mmc_activity == 0)
			bcsr->disk_leds |= (1 << 8);
	}
}

static struct led_classdev pb1200mmc_led = {
	.brightness_set	= pb1200_mmcled_set,
};

#ifndef CONFIG_MIPS_DB1200
static void pb1200mmc1_set_power(void *mmc_host, int state)
{
	if (state)
		bcsr->board |= BCSR_BOARD_SD1PWR;
	else
		bcsr->board &= ~BCSR_BOARD_SD1PWR;

	au_sync_delay(1);
}

static int pb1200mmc1_card_readonly(void *mmc_host)
{
	return (bcsr->status & BCSR_STATUS_SD1WP) ? 1 : 0;
}

static int pb1200mmc1_card_inserted(void *mmc_host)
{
	return (bcsr->sig_status & BCSR_INT_SD1INSERT) ? 1 : 0;
}
#endif

const struct au1xmmc_platform_data au1xmmc_platdata[2] = {
	[0] = {
		.set_power	= pb1200mmc0_set_power,
		.card_inserted	= pb1200mmc0_card_inserted,
		.card_readonly	= pb1200mmc0_card_readonly,
		.cd_setup	= NULL,		/* use poll-timer in driver */
		.led		= &pb1200mmc_led,
	},
#ifndef CONFIG_MIPS_DB1200
	[1] = {
		.set_power	= pb1200mmc1_set_power,
		.card_inserted	= pb1200mmc1_card_inserted,
		.card_readonly	= pb1200mmc1_card_readonly,
		.cd_setup	= NULL,		/* use poll-timer in driver */
		.led		= &pb1200mmc_led,
	},
#endif
};

static struct resource ide_resources[] = {
	[0] = {
		.start	= IDE_PHYS_ADDR,
		.end 	= IDE_PHYS_ADDR + IDE_PHYS_LEN - 1,
		.flags	= IORESOURCE_MEM
	},
	[1] = {
		.start	= IDE_INT,
		.end	= IDE_INT,
		.flags	= IORESOURCE_IRQ
	}
};

static u64 ide_dmamask = DMA_32BIT_MASK;

static struct platform_device ide_device = {
	.name		= "au1200-ide",
	.id		= 0,
	.dev = {
		.dma_mask 		= &ide_dmamask,
		.coherent_dma_mask	= DMA_32BIT_MASK,
	},
	.num_resources	= ARRAY_SIZE(ide_resources),
	.resource	= ide_resources
};

static struct resource smc91c111_resources[] = {
	[0] = {
		.name	= "smc91x-regs",
		.start	= SMC91C111_PHYS_ADDR,
		.end	= SMC91C111_PHYS_ADDR + 0xf,
		.flags	= IORESOURCE_MEM
	},
	[1] = {
		.start	= SMC91C111_INT,
		.end	= SMC91C111_INT,
		.flags	= IORESOURCE_IRQ
	},
};

static struct platform_device smc91c111_device = {
	.name		= "smc91x",
	.id		= -1,
	.num_resources	= ARRAY_SIZE(smc91c111_resources),
	.resource	= smc91c111_resources
};

static struct platform_device *board_platform_devices[] __initdata = {
	&ide_device,
	&smc91c111_device
};

static int __init board_register_devices(void)
{
	return platform_add_devices(board_platform_devices,
				    ARRAY_SIZE(board_platform_devices));
}

arch_initcall(board_register_devices);
