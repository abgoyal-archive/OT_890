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
 * Instantiate mmio-mapped RTC chips based on device tree information
 *
 * Copyright 2007 David Gibson <dwg@au1.ibm.com>, IBM Corporation.
 *
 * This program is free software; you can redistribute  it and/or modify it
 * under  the terms of  the GNU General  Public License as published by the
 * Free Software Foundation;  either version 2 of the  License, or (at your
 * option) any later version.
 */
#include <linux/kernel.h>
#include <linux/of.h>
#include <linux/init.h>
#include <linux/of_platform.h>

static __initdata struct {
	const char *compatible;
	char *plat_name;
} of_rtc_table[] = {
	{ "ds1743-nvram", "rtc-ds1742" },
};

void __init of_instantiate_rtc(void)
{
	struct device_node *node;
	int err;
	int i;

	for (i = 0; i < ARRAY_SIZE(of_rtc_table); i++) {
		char *plat_name = of_rtc_table[i].plat_name;

		for_each_compatible_node(node, NULL,
					 of_rtc_table[i].compatible) {
			struct resource *res;

			res = kmalloc(sizeof(*res), GFP_KERNEL);
			if (!res) {
				printk(KERN_ERR "OF RTC: Out of memory "
				       "allocating resource structure for %s\n",
				       node->full_name);
				continue;
			}

			err = of_address_to_resource(node, 0, res);
			if (err) {
				printk(KERN_ERR "OF RTC: Error "
				       "translating resources for %s\n",
				       node->full_name);
				continue;
			}

			printk(KERN_INFO "OF_RTC: %s is a %s @ 0x%llx-0x%llx\n",
			       node->full_name, plat_name,
			       (unsigned long long)res->start,
			       (unsigned long long)res->end);
			platform_device_register_simple(plat_name, -1, res, 1);
		}
	}
}
