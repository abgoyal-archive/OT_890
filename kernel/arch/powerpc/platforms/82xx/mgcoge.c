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
 * Keymile mgcoge support
 * Copyright 2008 DENX Software Engineering GmbH
 * Author: Heiko Schocher <hs@denx.de>
 *
 * based on code from:
 * Copyright 2007 Freescale Semiconductor, Inc.
 * Author: Scott Wood <scottwood@freescale.com>
 *
 * This program is free software; you can redistribute  it and/or modify it
 * under  the terms of  the GNU General  Public License as published by the
 * Free Software Foundation;  either version 2 of the  License, or (at your
 * option) any later version.
 */

#include <linux/init.h>
#include <linux/interrupt.h>
#include <linux/fsl_devices.h>
#include <linux/of_platform.h>

#include <asm/io.h>
#include <asm/cpm2.h>
#include <asm/udbg.h>
#include <asm/machdep.h>
#include <asm/time.h>
#include <asm/mpc8260.h>
#include <asm/prom.h>

#include <sysdev/fsl_soc.h>
#include <sysdev/cpm2_pic.h>

#include "pq2.h"

static void __init mgcoge_pic_init(void)
{
	struct device_node *np = of_find_compatible_node(NULL, NULL, "fsl,pq2-pic");
	if (!np) {
		printk(KERN_ERR "PIC init: can not find cpm-pic node\n");
		return;
	}

	cpm2_pic_init(np);
	of_node_put(np);
}

struct cpm_pin {
	int port, pin, flags;
};

static __initdata struct cpm_pin mgcoge_pins[] = {

	/* SMC2 */
	{1, 8, CPM_PIN_INPUT | CPM_PIN_PRIMARY},
	{1, 9, CPM_PIN_OUTPUT | CPM_PIN_PRIMARY},

	/* SCC4 */
	{3, 25, CPM_PIN_INPUT | CPM_PIN_PRIMARY},
	{3, 24, CPM_PIN_INPUT | CPM_PIN_PRIMARY},
	{3,  9, CPM_PIN_INPUT | CPM_PIN_PRIMARY},
	{3,  8, CPM_PIN_INPUT | CPM_PIN_PRIMARY},
	{4, 22, CPM_PIN_INPUT | CPM_PIN_PRIMARY},
	{4, 21, CPM_PIN_OUTPUT | CPM_PIN_PRIMARY},
};

static void __init init_ioports(void)
{
	int i;

	for (i = 0; i < ARRAY_SIZE(mgcoge_pins); i++) {
		const struct cpm_pin *pin = &mgcoge_pins[i];
		cpm2_set_pin(pin->port - 1, pin->pin, pin->flags);
	}

	cpm2_smc_clk_setup(CPM_CLK_SMC2, CPM_BRG8);
	cpm2_clk_setup(CPM_CLK_SCC4, CPM_CLK7, CPM_CLK_RX);
	cpm2_clk_setup(CPM_CLK_SCC4, CPM_CLK8, CPM_CLK_TX);
}

static void __init mgcoge_setup_arch(void)
{
	if (ppc_md.progress)
		ppc_md.progress("mgcoge_setup_arch()", 0);

	cpm2_reset();

	/* When this is set, snooping CPM DMA from RAM causes
	 * machine checks.  See erratum SIU18.
	 */
	clrbits32(&cpm2_immr->im_siu_conf.siu_82xx.sc_bcr, MPC82XX_BCR_PLDP);

	init_ioports();

	if (ppc_md.progress)
		ppc_md.progress("mgcoge_setup_arch(), finish", 0);
}

static  __initdata struct of_device_id of_bus_ids[] = {
	{ .compatible = "simple-bus", },
	{},
};

static int __init declare_of_platform_devices(void)
{
	of_platform_bus_probe(NULL, of_bus_ids, NULL);

	return 0;
}
machine_device_initcall(mgcoge, declare_of_platform_devices);

/*
 * Called very early, device-tree isn't unflattened
 */
static int __init mgcoge_probe(void)
{
	unsigned long root = of_get_flat_dt_root();
	return of_flat_dt_is_compatible(root, "keymile,mgcoge");
}

define_machine(mgcoge)
{
	.name = "Keymile MGCOGE",
	.probe = mgcoge_probe,
	.setup_arch = mgcoge_setup_arch,
	.init_IRQ = mgcoge_pic_init,
	.get_irq = cpm2_get_irq,
	.calibrate_decr = generic_calibrate_decr,
	.restart = pq2_restart,
	.progress = udbg_progress,
};
