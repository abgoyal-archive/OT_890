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

/* power.c: Power management driver.
 *
 * Copyright (C) 1999, 2007, 2008 David S. Miller (davem@davemloft.net)
 */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/interrupt.h>
#include <linux/reboot.h>
#include <linux/of_device.h>

#include <asm/prom.h>
#include <asm/io.h>

static void __iomem *power_reg;

static irqreturn_t power_handler(int irq, void *dev_id)
{
	orderly_poweroff(true);

	/* FIXME: Check registers for status... */
	return IRQ_HANDLED;
}

static int __init has_button_interrupt(unsigned int irq, struct device_node *dp)
{
	if (irq == 0xffffffff)
		return 0;
	if (!of_find_property(dp, "button", NULL))
		return 0;

	return 1;
}

static int __devinit power_probe(struct of_device *op, const struct of_device_id *match)
{
	struct resource *res = &op->resource[0];
	unsigned int irq= op->irqs[0];

	power_reg = of_ioremap(res, 0, 0x4, "power");

	printk(KERN_INFO "%s: Control reg at %llx\n",
	       op->node->name, res->start);

	if (has_button_interrupt(irq, op->node)) {
		if (request_irq(irq,
				power_handler, 0, "power", NULL) < 0)
			printk(KERN_ERR "power: Cannot setup IRQ handler.\n");
	}

	return 0;
}

static struct of_device_id __initdata power_match[] = {
	{
		.name = "power",
	},
	{},
};

static struct of_platform_driver power_driver = {
	.match_table	= power_match,
	.probe		= power_probe,
	.driver		= {
		.name	= "power",
	},
};

static int __init power_init(void)
{
	return of_register_driver(&power_driver, &of_platform_bus_type);
}

device_initcall(power_init);
