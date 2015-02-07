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
 * Copyright (c) 2006 Dave Airlie <airlied@linux.ie>
 * Copyright © 2006-2008 Intel Corporation
 *   Jesse Barnes <jesse.barnes@intel.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice (including the next
 * paragraph) shall be included in all copies or substantial portions of the
 * Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 *
 * Authors:
 *	Eric Anholt <eric@anholt.net>
 */
#include <linux/i2c.h>
#include <linux/i2c-id.h>
#include <linux/i2c-algo-bit.h>
#include "drmP.h"
#include "drm.h"
#include "intel_drv.h"
#include "i915_drm.h"
#include "i915_drv.h"

/*
 * Intel GPIO access functions
 */

#define I2C_RISEFALL_TIME 20

static int get_clock(void *data)
{
	struct intel_i2c_chan *chan = data;
	struct drm_i915_private *dev_priv = chan->drm_dev->dev_private;
	u32 val;

	val = I915_READ(chan->reg);
	return ((val & GPIO_CLOCK_VAL_IN) != 0);
}

static int get_data(void *data)
{
	struct intel_i2c_chan *chan = data;
	struct drm_i915_private *dev_priv = chan->drm_dev->dev_private;
	u32 val;

	val = I915_READ(chan->reg);
	return ((val & GPIO_DATA_VAL_IN) != 0);
}

static void set_clock(void *data, int state_high)
{
	struct intel_i2c_chan *chan = data;
	struct drm_device *dev = chan->drm_dev;
	struct drm_i915_private *dev_priv = chan->drm_dev->dev_private;
	u32 reserved = 0, clock_bits;

	/* On most chips, these bits must be preserved in software. */
	if (!IS_I830(dev) && !IS_845G(dev))
		reserved = I915_READ(chan->reg) & (GPIO_DATA_PULLUP_DISABLE |
						   GPIO_CLOCK_PULLUP_DISABLE);

	if (state_high)
		clock_bits = GPIO_CLOCK_DIR_IN | GPIO_CLOCK_DIR_MASK;
	else
		clock_bits = GPIO_CLOCK_DIR_OUT | GPIO_CLOCK_DIR_MASK |
			GPIO_CLOCK_VAL_MASK;
	I915_WRITE(chan->reg, reserved | clock_bits);
	udelay(I2C_RISEFALL_TIME); /* wait for the line to change state */
}

static void set_data(void *data, int state_high)
{
	struct intel_i2c_chan *chan = data;
	struct drm_device *dev = chan->drm_dev;
	struct drm_i915_private *dev_priv = chan->drm_dev->dev_private;
	u32 reserved = 0, data_bits;

	/* On most chips, these bits must be preserved in software. */
	if (!IS_I830(dev) && !IS_845G(dev))
		reserved = I915_READ(chan->reg) & (GPIO_DATA_PULLUP_DISABLE |
						   GPIO_CLOCK_PULLUP_DISABLE);

	if (state_high)
		data_bits = GPIO_DATA_DIR_IN | GPIO_DATA_DIR_MASK;
	else
		data_bits = GPIO_DATA_DIR_OUT | GPIO_DATA_DIR_MASK |
			GPIO_DATA_VAL_MASK;

	I915_WRITE(chan->reg, reserved | data_bits);
	udelay(I2C_RISEFALL_TIME); /* wait for the line to change state */
}

/**
 * intel_i2c_create - instantiate an Intel i2c bus using the specified GPIO reg
 * @dev: DRM device
 * @output: driver specific output device
 * @reg: GPIO reg to use
 * @name: name for this bus
 *
 * Creates and registers a new i2c bus with the Linux i2c layer, for use
 * in output probing and control (e.g. DDC or SDVO control functions).
 *
 * Possible values for @reg include:
 *   %GPIOA
 *   %GPIOB
 *   %GPIOC
 *   %GPIOD
 *   %GPIOE
 *   %GPIOF
 *   %GPIOG
 *   %GPIOH
 * see PRM for details on how these different busses are used.
 */
struct intel_i2c_chan *intel_i2c_create(struct drm_device *dev, const u32 reg,
					const char *name)
{
	struct intel_i2c_chan *chan;

	chan = kzalloc(sizeof(struct intel_i2c_chan), GFP_KERNEL);
	if (!chan)
		goto out_free;

	chan->drm_dev = dev;
	chan->reg = reg;
	snprintf(chan->adapter.name, I2C_NAME_SIZE, "intel drm %s", name);
	chan->adapter.owner = THIS_MODULE;
	chan->adapter.algo_data	= &chan->algo;
	chan->adapter.dev.parent = &dev->pdev->dev;
	chan->algo.setsda = set_data;
	chan->algo.setscl = set_clock;
	chan->algo.getsda = get_data;
	chan->algo.getscl = get_clock;
	chan->algo.udelay = 20;
	chan->algo.timeout = usecs_to_jiffies(2200);
	chan->algo.data = chan;

	i2c_set_adapdata(&chan->adapter, chan);

	if(i2c_bit_add_bus(&chan->adapter))
		goto out_free;

	/* JJJ:  raise SCL and SDA? */
	set_data(chan, 1);
	set_clock(chan, 1);
	udelay(20);

	return chan;

out_free:
	kfree(chan);
	return NULL;
}

/**
 * intel_i2c_destroy - unregister and free i2c bus resources
 * @output: channel to free
 *
 * Unregister the adapter from the i2c layer, then free the structure.
 */
void intel_i2c_destroy(struct intel_i2c_chan *chan)
{
	if (!chan)
		return;

	i2c_del_adapter(&chan->adapter);
	kfree(chan);
}
