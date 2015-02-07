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

/* Analogue & Micro Adder MPC875 board support
 *
 * Author: Scott Wood <scottwood@freescale.com>
 *
 * Copyright (c) 2007 Freescale Semiconductor, Inc.
 *
 * This program is free software; you can redistribute  it and/or modify
 * it under the terms of the GNU General Public License, version 2, as
 * published by the Free Software Foundation.
 */

#include <linux/init.h>
#include <linux/fs_enet_pd.h>
#include <linux/of_platform.h>

#include <asm/time.h>
#include <asm/machdep.h>
#include <asm/cpm1.h>
#include <asm/fs_pd.h>
#include <asm/udbg.h>
#include <asm/prom.h>

#include "mpc8xx.h"

struct cpm_pin {
	int port, pin, flags;
};

static __initdata struct cpm_pin adder875_pins[] = {
	/* SMC1 */
	{CPM_PORTB, 24, CPM_PIN_INPUT}, /* RX */
	{CPM_PORTB, 25, CPM_PIN_INPUT | CPM_PIN_SECONDARY}, /* TX */

	/* MII1 */
	{CPM_PORTA, 0, CPM_PIN_INPUT},
	{CPM_PORTA, 1, CPM_PIN_INPUT},
	{CPM_PORTA, 2, CPM_PIN_INPUT},
	{CPM_PORTA, 3, CPM_PIN_INPUT},
	{CPM_PORTA, 4, CPM_PIN_OUTPUT},
	{CPM_PORTA, 10, CPM_PIN_OUTPUT},
	{CPM_PORTA, 11, CPM_PIN_OUTPUT},
	{CPM_PORTB, 19, CPM_PIN_INPUT},
	{CPM_PORTB, 31, CPM_PIN_INPUT},
	{CPM_PORTC, 12, CPM_PIN_INPUT},
	{CPM_PORTC, 13, CPM_PIN_INPUT},
	{CPM_PORTE, 30, CPM_PIN_OUTPUT},
	{CPM_PORTE, 31, CPM_PIN_OUTPUT},

	/* MII2 */
	{CPM_PORTE, 14, CPM_PIN_OUTPUT | CPM_PIN_SECONDARY},
	{CPM_PORTE, 15, CPM_PIN_OUTPUT | CPM_PIN_SECONDARY},
	{CPM_PORTE, 16, CPM_PIN_OUTPUT},
	{CPM_PORTE, 17, CPM_PIN_OUTPUT | CPM_PIN_SECONDARY},
	{CPM_PORTE, 18, CPM_PIN_OUTPUT | CPM_PIN_SECONDARY},
	{CPM_PORTE, 19, CPM_PIN_OUTPUT | CPM_PIN_SECONDARY},
	{CPM_PORTE, 20, CPM_PIN_OUTPUT | CPM_PIN_SECONDARY},
	{CPM_PORTE, 21, CPM_PIN_OUTPUT},
	{CPM_PORTE, 22, CPM_PIN_OUTPUT},
	{CPM_PORTE, 23, CPM_PIN_OUTPUT},
	{CPM_PORTE, 24, CPM_PIN_OUTPUT},
	{CPM_PORTE, 25, CPM_PIN_OUTPUT},
	{CPM_PORTE, 26, CPM_PIN_OUTPUT},
	{CPM_PORTE, 27, CPM_PIN_OUTPUT},
	{CPM_PORTE, 28, CPM_PIN_OUTPUT},
	{CPM_PORTE, 29, CPM_PIN_OUTPUT},
};

static void __init init_ioports(void)
{
	int i;

	for (i = 0; i < ARRAY_SIZE(adder875_pins); i++) {
		const struct cpm_pin *pin = &adder875_pins[i];
		cpm1_set_pin(pin->port, pin->pin, pin->flags);
	}

	cpm1_clk_setup(CPM_CLK_SMC1, CPM_BRG1, CPM_CLK_RTX);

	/* Set FEC1 and FEC2 to MII mode */
	clrbits32(&mpc8xx_immr->im_cpm.cp_cptr, 0x00000180);
}

static void __init adder875_setup(void)
{
	cpm_reset();
	init_ioports();
}

static int __init adder875_probe(void)
{
	unsigned long root = of_get_flat_dt_root();
	return of_flat_dt_is_compatible(root, "analogue-and-micro,adder875");
}

static __initdata struct of_device_id of_bus_ids[] = {
	{ .compatible = "simple-bus", },
	{},
};

static int __init declare_of_platform_devices(void)
{
	of_platform_bus_probe(NULL, of_bus_ids, NULL);
	return 0;
}
machine_device_initcall(adder875, declare_of_platform_devices);

define_machine(adder875) {
	.name = "Adder MPC875",
	.probe = adder875_probe,
	.setup_arch = adder875_setup,
	.init_IRQ = mpc8xx_pics_init,
	.get_irq = mpc8xx_get_irq,
	.restart = mpc8xx_restart,
	.calibrate_decr = generic_calibrate_decr,
	.set_rtc_time = mpc8xx_set_rtc_time,
	.get_rtc_time = mpc8xx_get_rtc_time,
	.progress = udbg_progress,
};
