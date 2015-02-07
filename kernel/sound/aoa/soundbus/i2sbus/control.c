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
 * i2sbus driver -- bus control routines
 *
 * Copyright 2006 Johannes Berg <johannes@sipsolutions.net>
 *
 * GPL v2, can be found in COPYING.
 */

#include <linux/kernel.h>
#include <linux/delay.h>

#include <asm/io.h>
#include <asm/prom.h>
#include <asm/macio.h>
#include <asm/pmac_feature.h>
#include <asm/pmac_pfunc.h>
#include <asm/keylargo.h>

#include "i2sbus.h"

int i2sbus_control_init(struct macio_dev* dev, struct i2sbus_control **c)
{
	*c = kzalloc(sizeof(struct i2sbus_control), GFP_KERNEL);
	if (!*c)
		return -ENOMEM;

	INIT_LIST_HEAD(&(*c)->list);

	(*c)->macio = dev->bus->chip;
	return 0;
}

void i2sbus_control_destroy(struct i2sbus_control *c)
{
	kfree(c);
}

/* this is serialised externally */
int i2sbus_control_add_dev(struct i2sbus_control *c,
			   struct i2sbus_dev *i2sdev)
{
	struct device_node *np;

	np = i2sdev->sound.ofdev.node;
	i2sdev->enable = pmf_find_function(np, "enable");
	i2sdev->cell_enable = pmf_find_function(np, "cell-enable");
	i2sdev->clock_enable = pmf_find_function(np, "clock-enable");
	i2sdev->cell_disable = pmf_find_function(np, "cell-disable");
	i2sdev->clock_disable = pmf_find_function(np, "clock-disable");

	/* if the bus number is not 0 or 1 we absolutely need to use
	 * the platform functions -- there's nothing in Darwin that
	 * would allow seeing a system behind what the FCRs are then,
	 * and I don't want to go parsing a bunch of platform functions
	 * by hand to try finding a system... */
	if (i2sdev->bus_number != 0 && i2sdev->bus_number != 1 &&
	    (!i2sdev->enable ||
	     !i2sdev->cell_enable || !i2sdev->clock_enable ||
	     !i2sdev->cell_disable || !i2sdev->clock_disable)) {
		pmf_put_function(i2sdev->enable);
		pmf_put_function(i2sdev->cell_enable);
		pmf_put_function(i2sdev->clock_enable);
		pmf_put_function(i2sdev->cell_disable);
		pmf_put_function(i2sdev->clock_disable);
		return -ENODEV;
	}

	list_add(&i2sdev->item, &c->list);

	return 0;
}

void i2sbus_control_remove_dev(struct i2sbus_control *c,
			       struct i2sbus_dev *i2sdev)
{
	/* this is serialised externally */
	list_del(&i2sdev->item);
	if (list_empty(&c->list))
		i2sbus_control_destroy(c);
}

int i2sbus_control_enable(struct i2sbus_control *c,
			  struct i2sbus_dev *i2sdev)
{
	struct pmf_args args = { .count = 0 };
	struct macio_chip *macio = c->macio;

	if (i2sdev->enable)
		return pmf_call_one(i2sdev->enable, &args);

	if (macio == NULL || macio->base == NULL)
		return -ENODEV;

	switch (i2sdev->bus_number) {
	case 0:
		/* these need to be locked or done through
		 * newly created feature calls! */
		MACIO_BIS(KEYLARGO_FCR1, KL1_I2S0_ENABLE);
		break;
	case 1:
		MACIO_BIS(KEYLARGO_FCR1, KL1_I2S1_ENABLE);
		break;
	default:
		return -ENODEV;
	}
	return 0;
}

int i2sbus_control_cell(struct i2sbus_control *c,
			struct i2sbus_dev *i2sdev,
			int enable)
{
	struct pmf_args args = { .count = 0 };
	struct macio_chip *macio = c->macio;

	switch (enable) {
	case 0:
		if (i2sdev->cell_disable)
			return pmf_call_one(i2sdev->cell_disable, &args);
		break;
	case 1:
		if (i2sdev->cell_enable)
			return pmf_call_one(i2sdev->cell_enable, &args);
		break;
	default:
		printk(KERN_ERR "i2sbus: INVALID CELL ENABLE VALUE\n");
		return -ENODEV;
	}

	if (macio == NULL || macio->base == NULL)
		return -ENODEV;

	switch (i2sdev->bus_number) {
	case 0:
		if (enable)
			MACIO_BIS(KEYLARGO_FCR1, KL1_I2S0_CELL_ENABLE);
		else
			MACIO_BIC(KEYLARGO_FCR1, KL1_I2S0_CELL_ENABLE);
		break;
	case 1:
		if (enable)
			MACIO_BIS(KEYLARGO_FCR1, KL1_I2S1_CELL_ENABLE);
		else
			MACIO_BIC(KEYLARGO_FCR1, KL1_I2S1_CELL_ENABLE);
		break;
	default:
		return -ENODEV;
	}
	return 0;
}

int i2sbus_control_clock(struct i2sbus_control *c,
			 struct i2sbus_dev *i2sdev,
			 int enable)
{
	struct pmf_args args = { .count = 0 };
	struct macio_chip *macio = c->macio;

	switch (enable) {
	case 0:
		if (i2sdev->clock_disable)
			return pmf_call_one(i2sdev->clock_disable, &args);
		break;
	case 1:
		if (i2sdev->clock_enable)
			return pmf_call_one(i2sdev->clock_enable, &args);
		break;
	default:
		printk(KERN_ERR "i2sbus: INVALID CLOCK ENABLE VALUE\n");
		return -ENODEV;
	}

	if (macio == NULL || macio->base == NULL)
		return -ENODEV;

	switch (i2sdev->bus_number) {
	case 0:
		if (enable)
			MACIO_BIS(KEYLARGO_FCR1, KL1_I2S0_CLK_ENABLE_BIT);
		else
			MACIO_BIC(KEYLARGO_FCR1, KL1_I2S0_CLK_ENABLE_BIT);
		break;
	case 1:
		if (enable)
			MACIO_BIS(KEYLARGO_FCR1, KL1_I2S1_CLK_ENABLE_BIT);
		else
			MACIO_BIC(KEYLARGO_FCR1, KL1_I2S1_CLK_ENABLE_BIT);
		break;
	default:
		return -ENODEV;
	}
	return 0;
}
