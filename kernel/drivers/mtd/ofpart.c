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
 * Flash partitions described by the OF (or flattened) device tree
 *
 * Copyright (C) 2006 MontaVista Software Inc.
 * Author: Vitaly Wool <vwool@ru.mvista.com>
 *
 * Revised to handle newer style flash binding by:
 *   Copyright (C) 2007 David Gibson, IBM Corporation.
 *
 * This program is free software; you can redistribute  it and/or modify it
 * under  the terms of  the GNU General  Public License as published by the
 * Free Software Foundation;  either version 2 of the  License, or (at your
 * option) any later version.
 */

#include <linux/module.h>
#include <linux/init.h>
#include <linux/of.h>
#include <linux/mtd/mtd.h>
#include <linux/mtd/partitions.h>

int __devinit of_mtd_parse_partitions(struct device *dev,
                                      struct device_node *node,
                                      struct mtd_partition **pparts)
{
	const char *partname;
	struct device_node *pp;
	int nr_parts, i;

	/* First count the subnodes */
	pp = NULL;
	nr_parts = 0;
	while ((pp = of_get_next_child(node, pp)))
		nr_parts++;

	if (nr_parts == 0)
		return 0;

	*pparts = kzalloc(nr_parts * sizeof(**pparts), GFP_KERNEL);
	if (!*pparts)
		return -ENOMEM;

	pp = NULL;
	i = 0;
	while ((pp = of_get_next_child(node, pp))) {
		const u32 *reg;
		int len;

		reg = of_get_property(pp, "reg", &len);
		if (!reg || (len != 2 * sizeof(u32))) {
			of_node_put(pp);
			dev_err(dev, "Invalid 'reg' on %s\n", node->full_name);
			kfree(*pparts);
			*pparts = NULL;
			return -EINVAL;
		}
		(*pparts)[i].offset = reg[0];
		(*pparts)[i].size = reg[1];

		partname = of_get_property(pp, "label", &len);
		if (!partname)
			partname = of_get_property(pp, "name", &len);
		(*pparts)[i].name = (char *)partname;

		if (of_get_property(pp, "read-only", &len))
			(*pparts)[i].mask_flags = MTD_WRITEABLE;

		i++;
	}

	return nr_parts;
}
EXPORT_SYMBOL(of_mtd_parse_partitions);

MODULE_LICENSE("GPL");
