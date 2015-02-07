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
 *
 * Platform setup for the Keymile mgsuvd board
 *
 * Heiko Schocher <hs@denx.de>
 *
 * Copyright 2008 DENX Software Engineering GmbH
 *
 * This file is licensed under the terms of the GNU General Public License
 * version 2. This program is licensed "as is" without any warranty of any
 * kind, whether express or implied.
 */

#include <linux/ioport.h>
#include <linux/of_platform.h>

#include <asm/io.h>
#include <asm/machdep.h>
#include <asm/processor.h>
#include <asm/cpm1.h>
#include <asm/prom.h>
#include <asm/fs_pd.h>

#include "mpc8xx.h"

struct cpm_pin {
	int port, pin, flags;
};

static __initdata struct cpm_pin mgsuvd_pins[] = {
	/* SMC1 */
	{CPM_PORTB, 24, CPM_PIN_INPUT}, /* RX */
	{CPM_PORTB, 25, CPM_PIN_INPUT | CPM_PIN_SECONDARY}, /* TX */

	/* SCC3 */
	{CPM_PORTA, 10, CPM_PIN_INPUT},
	{CPM_PORTA, 11, CPM_PIN_INPUT},
	{CPM_PORTA, 3, CPM_PIN_INPUT},
	{CPM_PORTA, 2, CPM_PIN_INPUT},
	{CPM_PORTC, 13, CPM_PIN_INPUT},
};

static void __init init_ioports(void)
{
	int i;

	for (i = 0; i < ARRAY_SIZE(mgsuvd_pins); i++) {
		struct cpm_pin *pin = &mgsuvd_pins[i];
		cpm1_set_pin(pin->port, pin->pin, pin->flags);
	}

	setbits16(&mpc8xx_immr->im_ioport.iop_pcso, 0x300);
	cpm1_clk_setup(CPM_CLK_SCC3, CPM_CLK5, CPM_CLK_RX);
	cpm1_clk_setup(CPM_CLK_SCC3, CPM_CLK6, CPM_CLK_TX);
	cpm1_clk_setup(CPM_CLK_SMC1, CPM_BRG1, CPM_CLK_RTX);
}

static void __init mgsuvd_setup_arch(void)
{
	cpm_reset();
	init_ioports();
}

static __initdata struct of_device_id of_bus_ids[] = {
	{ .compatible = "simple-bus" },
	{},
};

static int __init declare_of_platform_devices(void)
{
	of_platform_bus_probe(NULL, of_bus_ids, NULL);
	return 0;
}
machine_device_initcall(mgsuvd, declare_of_platform_devices);

static int __init mgsuvd_probe(void)
{
	unsigned long root = of_get_flat_dt_root();
	return of_flat_dt_is_compatible(root, "keymile,mgsuvd");
}

define_machine(mgsuvd) {
	.name           = "MGSUVD",
	.probe          = mgsuvd_probe,
	.setup_arch     = mgsuvd_setup_arch,
	.init_IRQ       = mpc8xx_pics_init,
	.get_irq        = mpc8xx_get_irq,
	.restart        = mpc8xx_restart,
	.calibrate_decr = mpc8xx_calibrate_decr,
	.set_rtc_time   = mpc8xx_set_rtc_time,
	.get_rtc_time   = mpc8xx_get_rtc_time,
};
