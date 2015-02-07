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
 * arch/sh/boards/renesas/r7780rp/psw.c
 *
 * push switch support for RDBRP-1/RDBREVRP-1 debug boards.
 *
 * Copyright (C) 2006  Paul Mundt
 *
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 */
#include <linux/io.h>
#include <linux/init.h>
#include <linux/interrupt.h>
#include <linux/platform_device.h>
#include <mach/highlander.h>
#include <asm/push-switch.h>

static irqreturn_t psw_irq_handler(int irq, void *arg)
{
	struct platform_device *pdev = arg;
	struct push_switch *psw = platform_get_drvdata(pdev);
	struct push_switch_platform_info *psw_info = pdev->dev.platform_data;
	unsigned int l, mask;
	int ret = 0;

	l = ctrl_inw(PA_DBSW);

	/* Nothing to do if there's no state change */
	if (psw->state) {
		ret = 1;
		goto out;
	}

	mask = l & 0x70;
	/* Figure out who raised it */
	if (mask & (1 << psw_info->bit)) {
		psw->state = !!(mask & (1 << psw_info->bit));
		if (psw->state)	/* debounce */
			mod_timer(&psw->debounce, jiffies + 50);

		ret = 1;
	}

out:
	/* Clear the switch IRQs */
	l |= (0x7 << 12);
	ctrl_outw(l, PA_DBSW);

	return IRQ_RETVAL(ret);
}

static struct resource psw_resources[] = {
	[0] = {
		.start	= IRQ_PSW,
		.flags	= IORESOURCE_IRQ,
	},
};

static struct push_switch_platform_info s2_platform_data = {
	.name		= "s2",
	.bit		= 6,
	.irq_flags	= IRQF_TRIGGER_RISING | IRQF_TRIGGER_FALLING |
			  IRQF_SHARED,
	.irq_handler	= psw_irq_handler,
};

static struct platform_device s2_switch_device = {
	.name		= "push-switch",
	.id		= 0,
	.num_resources	= ARRAY_SIZE(psw_resources),
	.resource	= psw_resources,
	.dev		= {
		.platform_data = &s2_platform_data,
	},
};

static struct push_switch_platform_info s3_platform_data = {
	.name		= "s3",
	.bit		= 5,
	.irq_flags	= IRQF_TRIGGER_RISING | IRQF_TRIGGER_FALLING |
			  IRQF_SHARED,
	.irq_handler	= psw_irq_handler,
};

static struct platform_device s3_switch_device = {
	.name		= "push-switch",
	.id		= 1,
	.num_resources	= ARRAY_SIZE(psw_resources),
	.resource	= psw_resources,
	.dev		= {
		.platform_data = &s3_platform_data,
	},
};

static struct push_switch_platform_info s4_platform_data = {
	.name		= "s4",
	.bit		= 4,
	.irq_flags	= IRQF_TRIGGER_RISING | IRQF_TRIGGER_FALLING |
			  IRQF_SHARED,
	.irq_handler	= psw_irq_handler,
};

static struct platform_device s4_switch_device = {
	.name		= "push-switch",
	.id		= 2,
	.num_resources	= ARRAY_SIZE(psw_resources),
	.resource	= psw_resources,
	.dev		= {
		.platform_data = &s4_platform_data,
	},
};

static struct platform_device *psw_devices[] = {
	&s2_switch_device, &s3_switch_device, &s4_switch_device,
};

static int __init psw_init(void)
{
	return platform_add_devices(psw_devices, ARRAY_SIZE(psw_devices));
}
module_init(psw_init);
