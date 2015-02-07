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
 * Dummy clk implementations for powerpc.
 * These need to be overridden in platform code.
 */

#include <linux/clk.h>
#include <linux/err.h>
#include <linux/errno.h>
#include <linux/module.h>
#include <asm/clk_interface.h>

struct clk_interface clk_functions;

struct clk *clk_get(struct device *dev, const char *id)
{
	if (clk_functions.clk_get)
		return clk_functions.clk_get(dev, id);
	return ERR_PTR(-ENOSYS);
}
EXPORT_SYMBOL(clk_get);

void clk_put(struct clk *clk)
{
	if (clk_functions.clk_put)
		clk_functions.clk_put(clk);
}
EXPORT_SYMBOL(clk_put);

int clk_enable(struct clk *clk)
{
	if (clk_functions.clk_enable)
		return clk_functions.clk_enable(clk);
	return -ENOSYS;
}
EXPORT_SYMBOL(clk_enable);

void clk_disable(struct clk *clk)
{
	if (clk_functions.clk_disable)
		clk_functions.clk_disable(clk);
}
EXPORT_SYMBOL(clk_disable);

unsigned long clk_get_rate(struct clk *clk)
{
	if (clk_functions.clk_get_rate)
		return clk_functions.clk_get_rate(clk);
	return 0;
}
EXPORT_SYMBOL(clk_get_rate);

long clk_round_rate(struct clk *clk, unsigned long rate)
{
	if (clk_functions.clk_round_rate)
		return clk_functions.clk_round_rate(clk, rate);
	return -ENOSYS;
}
EXPORT_SYMBOL(clk_round_rate);

int clk_set_rate(struct clk *clk, unsigned long rate)
{
	if (clk_functions.clk_set_rate)
		return clk_functions.clk_set_rate(clk, rate);
	return -ENOSYS;
}
EXPORT_SYMBOL(clk_set_rate);

struct clk *clk_get_parent(struct clk *clk)
{
	if (clk_functions.clk_get_parent)
		return clk_functions.clk_get_parent(clk);
	return ERR_PTR(-ENOSYS);
}
EXPORT_SYMBOL(clk_get_parent);

int clk_set_parent(struct clk *clk, struct clk *parent)
{
	if (clk_functions.clk_set_parent)
		return clk_functions.clk_set_parent(clk, parent);
	return -ENOSYS;
}
EXPORT_SYMBOL(clk_set_parent);
