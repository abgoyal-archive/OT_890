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
 *
 * The following software/firmware and/or related documentation ("MediaTek Software")
 * have been modified by MediaTek Inc. All revisions are subject to any receiver's
 * applicable license agreements with MediaTek Inc.
 */

/* linux/arch/arm/mach-mt3351/board-43evz.c
 *
 * Machine support for MT3351 43EVZ board. 
 *
 * Copyright (C) 2008,2009 MediaTek <www.mediatek.com>
 * Authors: Infinity Chen <infinity.chen@mediatek.com>  
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
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

/* system header files */
#include <linux/kernel.h>
#include <linux/mm.h>
#include <linux/interrupt.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/device.h>
#include <linux/platform_device.h>

#include <asm/irq.h>
#include <asm/io.h>
#include <asm/mach-types.h>

#include <asm/mach/arch.h>
#include <asm/mach/irq.h>
#include <asm/mach/map.h>
#include <asm/mach/time.h>
#include <mach/board.h>
#include <mach/hardware.h>
#include <mach/mt3351_typedefs.h>
#include <mach/mt3351_reg_base.h>
#include <mach/mt3351_gpio.h>
#include <mach/mt3351_devs.h>

/*=======================================================================*/
/* Board Specific Devices                                                */
/*=======================================================================*/

/* MT3328 GPS */
struct platform_device mt3328_device_gps = {
	.name	       = "mt3328-gps",
	.id            = -1,
};

/*=======================================================================*/
/* Board Specific Devices Power Management                               */
/*=======================================================================*/
static void mt3351_sd1_ext_power_on(void)
{
    mt_set_gpio_out(34, GPIO_OUT_ONE); /* enable card power source */
}

static void mt3351_sd1_ext_power_off(void)
{
    mt_set_gpio_out(34, GPIO_OUT_ZERO); /* disable card power source */
}

/*=======================================================================*/
/* Board Specific Devices Init                                           */
/*=======================================================================*/
static __init int board_init(void)
{
    /* MT3351 board device init */
    mt3351_board_init();

    /* MT3328 GPS */
    platform_device_register(&mt3328_device_gps);

    return 0;
}

late_initcall(board_init);

/*=======================================================================*/
/* Board Devices Capability                                              */
/*=======================================================================*/
#if defined(CFG_DEV_MSDC1)
struct mt3351_sd_host_hw mt3351_sd1_hw = {
    .cmd_odc        = MSDC_ODC_16MA,
    .data_odc       = MSDC_ODC_16MA,
    .cmd_slew_rate  = MSDC_ODC_SLEW_FAST,
    .data_slew_rate = MSDC_ODC_SLEW_FAST,
    .data_pins      = CFG_MSDC1_DATA_PINS,
    .data_offset    = CFG_MSDC1_DATA_OFFSET,
    .flags          = MSDC_CD_PIN_EN,
    .ext_power_on   = mt3351_sd1_ext_power_on,
    .ext_power_off  = mt3351_sd1_ext_power_off,
};
#endif
#if defined(CFG_DEV_MSDC2)
struct mt3351_sd_host_hw mt3351_sd2_hw = {
    .cmd_odc         = MSDC_ODC_16MA,
    .data_odc        = MSDC_ODC_16MA,
    .cmd_slew_rate   = MSDC_ODC_SLEW_FAST,
    .data_slew_rate  = MSDC_ODC_SLEW_FAST,
    .data_pins       = CFG_MSDC2_DATA_PINS,
    .data_offset     = CFG_MSDC2_DATA_OFFSET,
    .flags           = MSDC_CD_PIN_EN | MSDC_REMOVABLE,
};
#endif
#if defined(CFG_DEV_MSDC3)
struct mt3351_sd_host_hw mt3351_sd3_hw = {
    .cmd_odc         = MSDC_ODC_4MA,
    .data_odc        = MSDC_ODC_4MA,
    .cmd_slew_rate   = MSDC_ODC_SLEW_FAST,
    .data_slew_rate  = MSDC_ODC_SLEW_FAST,
    .data_pins       = CFG_MSDC3_DATA_PINS,
    .data_offset     = CFG_MSDC3_DATA_OFFSET,
    .flags           = 0,
};
#endif

/*=======================================================================*/
/* Board Memory Mapping                                                  */
/*=======================================================================*/
static struct map_desc mt3351_io_desc[] __initdata =
{
    /* FIXME: shall replace with io agents */
    {IO_VIRT, IO_PHYS, IO_SIZE, MT_DEVICE},
    {IO_VIRT+0x100000, IO_PHYS+0x100000, IO_SIZE, MT_DEVICE},
    /*{PBIA_VIRT, PBIA_PHYS, PBIA_SIZE, MT_DEVICE},*/ 
    {IO_VIRT+0x600000, 0xA0000000, IO_SIZE, MT_DEVICE},
    {IO_VIRT+0x700000, 0xA2000000, IO_SIZE, MT_DEVICE},
    {IO_VIRT+0x800000, 0xA2100000, IO_SIZE, MT_DEVICE},
    {IO_VIRT+0x900000, 0xA2200000, IO_SIZE, MT_DEVICE},
};

static void __init mt3351_map_io(void)
{
    iotable_init(mt3351_io_desc, ARRAY_SIZE(mt3351_io_desc));
}

/*=======================================================================*/
/* Board Low-Level Init                                                  */
/*=======================================================================*/
static void mt3351_power_off(void)
{
    /* Power down v-core1, but it is not work as charger-in */
	DRV_WriteReg32((RTC_base+0x07C), 0x00);
}

static void __init mt3351_init_pinmux(void)
{
    mt_set_gpio_PinMux(PIN_MUX_JTAG_CTRL_JTAG, JTAG_CTL);
    mt_set_gpio_PinMux(PIN_MUX_SPI_CTRL_SPI1, SPI_CTL);

    mt_set_gpio_PinMux(PIN_MUX_DAI_CTRL_GPIO, DAI_CTL);
    mt_set_gpio_PinMux(PIN_MUX_PWM_CTRL_GPIO, PWM_CTL);

    mt_set_gpio_PinMux(PIN_MUX_MC0_CTRL_MC8_ON_BOARD, MC0_CTL);
    mt_set_gpio_PinMux(PIN_MUX_MC1_CTRL_GPIO, MC1_CTL);
    
    mt_set_gpio_PinMux(PIN_MUX_CAM0_CTRL_GPIO, CAM0_CTL);
    mt_set_gpio_PinMux(PIN_MUX_CAM1_CTRL_GPIO, CAM1_CTL);

    mt_set_gpio_PinMux(PIN_MUX_UT0_CTRL_UART, UT0_CTL);
    mt_set_gpio_PinMux(PIN_MUX_UT1_CTRL_GPIO, UT1_CTL);
    mt_set_gpio_PinMux(PIN_MUX_UT2_CTRL_UART, UT2_CTL);
    mt_set_gpio_PinMux(PIN_MUX_UT3_CTRL_GPIO, UT3_CTL);
    mt_set_gpio_PinMux(PIN_MUX_UT4_CTRL_UART, UT4_CTL);

    mt_set_gpio_PinMux(PIN_MUX_NLD_CTRLL_NAND, NLD_CTLL);
    mt_set_gpio_PinMux(PIN_MUX_NLD_CTRLH_GPIO, NLD_CTLH);   /* for LCM GPIO */

    mt_set_gpio_PinMux(PIN_MUX_DPI_CTRL_DPI666, DPI_CTL);
}

static void __init mt3351_init_gpio(void)
{
    /* For MT3328 GPS clock */
    mt_set_gpio_OCFG(GPIO22_OCTL_ECLK, GPIO22_OCTL); /* External Clock */ 

    /* PWKEY SRC */
    mt_set_gpio_ICFG(EINT7_GPIO19_INPUT, EINT7_SRC);

    /* PWM PWM5 GPIO */
    mt_set_gpio_dir(13, GPIO_DIR_OUT);
    mt_set_gpio_OCFG(GPIO13_OCTL_PWM5, GPIO13_OCTL); 

    /* MSDC0 power switch */
    mt_set_gpio_dir(34, GPIO_DIR_OUT);

    /* LCM GPIO */
    mt_set_gpio_dir(90, GPIO_DIR_OUT);          /* POWER_ON */
    mt_set_gpio_pullup(90, GPIO_PU_DISABLE);
    mt_set_gpio_pulldown(90, GPIO_PD_DISABLE);
}

static void __init mt3351_init_machine(void)
{
    /* Setting the Machine power off function */
    pm_power_off = mt3351_power_off;

    /* Init pinmux */
    mt3351_init_pinmux();

    /* Init gpio */
    mt3351_init_gpio();
}

extern void __init mt3351_init_irq(void);
extern struct sys_timer mt3351_timer;

MACHINE_START(MT3351, "MT3351 EVZ")
    //.phys_ram       = MT3351_SDRAM_PA,
    .phys_io        = MT3351_PA_UART,
    .io_pg_offst    = (MT3351_VA_UART >> 18) & 0xfffc,
    .boot_params    = MT3351_SDRAM_PA + 0x100,
    .map_io         = mt3351_map_io,
    .init_irq       = mt3351_init_irq,
    .init_machine   = mt3351_init_machine,
    .timer          = &mt3351_timer,
MACHINE_END

