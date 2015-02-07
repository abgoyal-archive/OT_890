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
 * Copyright (C) 2007 PA Semi, Inc
 *
 * Parts based on arch/powerpc/sysdev/fsl_soc.c:
 *
 * 2006 (c) MontaVista Software, Inc.
 *
 * This program is free software; you can redistribute  it and/or modify it
 * under  the terms of  the GNU General  Public License as published by the
 * Free Software Foundation;  either version 2 of the  License, or (at your
 * option) any later version.
 */

#include <linux/errno.h>
#include <linux/kernel.h>
#include <linux/pci.h>
#include <linux/of.h>
#include <linux/i2c.h>

#ifdef CONFIG_I2C_BOARDINFO
/* The below is from fsl_soc.c.  It's copied because since there are no
 * official bus bindings at this time it doesn't make sense to share across
 * the platforms, even though they happen to be common.
 */
struct i2c_driver_device {
	char    *of_device;
	char    *i2c_type;
};

static struct i2c_driver_device i2c_devices[] __initdata = {
	{"dallas,ds1338",  "ds1338"},
};

static int __init find_i2c_driver(struct device_node *node,
				     struct i2c_board_info *info)
{
	int i;

	for (i = 0; i < ARRAY_SIZE(i2c_devices); i++) {
		if (!of_device_is_compatible(node, i2c_devices[i].of_device))
			continue;
		if (strlcpy(info->type, i2c_devices[i].i2c_type,
			    I2C_NAME_SIZE) >= I2C_NAME_SIZE)
			return -ENOMEM;
		return 0;
	}
	return -ENODEV;
}

static int __init pasemi_register_i2c_devices(void)
{
	struct pci_dev *pdev;
	struct device_node *adap_node;
	struct device_node *node;

	pdev = NULL;
	while ((pdev = pci_get_device(PCI_VENDOR_ID_PASEMI, 0xa003, pdev))) {
		adap_node = pci_device_to_OF_node(pdev);

		if (!adap_node)
			continue;

		node = NULL;
		while ((node = of_get_next_child(adap_node, node))) {
			struct i2c_board_info info = {};
			const u32 *addr;
			int len;

			addr = of_get_property(node, "reg", &len);
			if (!addr || len < sizeof(int) ||
			    *addr > (1 << 10) - 1) {
				printk(KERN_WARNING
					"pasemi_register_i2c_devices: "
					"invalid i2c device entry\n");
				continue;
			}

			info.irq = irq_of_parse_and_map(node, 0);
			if (info.irq == NO_IRQ)
				info.irq = -1;

			if (find_i2c_driver(node, &info) < 0)
				continue;

			info.addr = *addr;

			i2c_register_board_info(PCI_FUNC(pdev->devfn), &info,
						1);
		}
	}
	return 0;
}
device_initcall(pasemi_register_i2c_devices);
#endif
