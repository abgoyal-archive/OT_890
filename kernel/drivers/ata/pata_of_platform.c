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
 * OF-platform PATA driver
 *
 * Copyright (c) 2007  MontaVista Software, Inc.
 *                     Anton Vorontsov <avorontsov@ru.mvista.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License (Version 2) as
 * published by the Free Software Foundation.
 */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/of_platform.h>
#include <linux/ata_platform.h>

static int __devinit pata_of_platform_probe(struct of_device *ofdev,
					    const struct of_device_id *match)
{
	int ret;
	struct device_node *dn = ofdev->node;
	struct resource io_res;
	struct resource ctl_res;
	struct resource irq_res;
	unsigned int reg_shift = 0;
	int pio_mode = 0;
	int pio_mask;
	const u32 *prop;

	ret = of_address_to_resource(dn, 0, &io_res);
	if (ret) {
		dev_err(&ofdev->dev, "can't get IO address from "
			"device tree\n");
		return -EINVAL;
	}

	if (of_device_is_compatible(dn, "electra-ide")) {
		/* Altstatus is really at offset 0x3f6 from the primary window
		 * on electra-ide. Adjust ctl_res and io_res accordingly.
		 */
		ctl_res = io_res;
		ctl_res.start = ctl_res.start+0x3f6;
		io_res.end = ctl_res.start-1;
	} else {
		ret = of_address_to_resource(dn, 1, &ctl_res);
		if (ret) {
			dev_err(&ofdev->dev, "can't get CTL address from "
				"device tree\n");
			return -EINVAL;
		}
	}

	ret = of_irq_to_resource(dn, 0, &irq_res);
	if (ret == NO_IRQ)
		irq_res.start = irq_res.end = 0;
	else
		irq_res.flags = 0;

	prop = of_get_property(dn, "reg-shift", NULL);
	if (prop)
		reg_shift = *prop;

	prop = of_get_property(dn, "pio-mode", NULL);
	if (prop) {
		pio_mode = *prop;
		if (pio_mode > 6) {
			dev_err(&ofdev->dev, "invalid pio-mode\n");
			return -EINVAL;
		}
	} else {
		dev_info(&ofdev->dev, "pio-mode unspecified, assuming PIO0\n");
	}

	pio_mask = 1 << pio_mode;
	pio_mask |= (1 << pio_mode) - 1;

	return __pata_platform_probe(&ofdev->dev, &io_res, &ctl_res, &irq_res,
				     reg_shift, pio_mask);
}

static int __devexit pata_of_platform_remove(struct of_device *ofdev)
{
	return __pata_platform_remove(&ofdev->dev);
}

static struct of_device_id pata_of_platform_match[] = {
	{ .compatible = "ata-generic", },
	{ .compatible = "electra-ide", },
	{},
};
MODULE_DEVICE_TABLE(of, pata_of_platform_match);

static struct of_platform_driver pata_of_platform_driver = {
	.name		= "pata_of_platform",
	.match_table	= pata_of_platform_match,
	.probe		= pata_of_platform_probe,
	.remove		= __devexit_p(pata_of_platform_remove),
};

static int __init pata_of_platform_init(void)
{
	return of_register_platform_driver(&pata_of_platform_driver);
}
module_init(pata_of_platform_init);

static void __exit pata_of_platform_exit(void)
{
	of_unregister_platform_driver(&pata_of_platform_driver);
}
module_exit(pata_of_platform_exit);

MODULE_DESCRIPTION("OF-platform PATA driver");
MODULE_AUTHOR("Anton Vorontsov <avorontsov@ru.mvista.com>");
MODULE_LICENSE("GPL");
