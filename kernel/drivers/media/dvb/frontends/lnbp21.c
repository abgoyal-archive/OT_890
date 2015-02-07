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
 * lnbp21.h - driver for lnb supply and control ic lnbp21
 *
 * Copyright (C) 2006 Oliver Endriss
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 * Or, point your browser to http://www.gnu.org/copyleft/gpl.html
 *
 *
 * the project's page is at http://www.linuxtv.org
 */
#include <linux/delay.h>
#include <linux/errno.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/string.h>
#include <linux/slab.h>

#include "dvb_frontend.h"
#include "lnbp21.h"

struct lnbp21 {
	u8			config;
	u8			override_or;
	u8			override_and;
	struct i2c_adapter	*i2c;
};

static int lnbp21_set_voltage(struct dvb_frontend *fe, fe_sec_voltage_t voltage)
{
	struct lnbp21 *lnbp21 = (struct lnbp21 *) fe->sec_priv;
	struct i2c_msg msg = {	.addr = 0x08, .flags = 0,
				.buf = &lnbp21->config,
				.len = sizeof(lnbp21->config) };

	lnbp21->config &= ~(LNBP21_VSEL | LNBP21_EN);

	switch(voltage) {
	case SEC_VOLTAGE_OFF:
		break;
	case SEC_VOLTAGE_13:
		lnbp21->config |= LNBP21_EN;
		break;
	case SEC_VOLTAGE_18:
		lnbp21->config |= (LNBP21_EN | LNBP21_VSEL);
		break;
	default:
		return -EINVAL;
	};

	lnbp21->config |= lnbp21->override_or;
	lnbp21->config &= lnbp21->override_and;

	return (i2c_transfer(lnbp21->i2c, &msg, 1) == 1) ? 0 : -EIO;
}

static int lnbp21_enable_high_lnb_voltage(struct dvb_frontend *fe, long arg)
{
	struct lnbp21 *lnbp21 = (struct lnbp21 *) fe->sec_priv;
	struct i2c_msg msg = {	.addr = 0x08, .flags = 0,
				.buf = &lnbp21->config,
				.len = sizeof(lnbp21->config) };

	if (arg)
		lnbp21->config |= LNBP21_LLC;
	else
		lnbp21->config &= ~LNBP21_LLC;

	lnbp21->config |= lnbp21->override_or;
	lnbp21->config &= lnbp21->override_and;

	return (i2c_transfer(lnbp21->i2c, &msg, 1) == 1) ? 0 : -EIO;
}

static void lnbp21_release(struct dvb_frontend *fe)
{
	/* LNBP power off */
	lnbp21_set_voltage(fe, SEC_VOLTAGE_OFF);

	/* free data */
	kfree(fe->sec_priv);
	fe->sec_priv = NULL;
}

struct dvb_frontend *lnbp21_attach(struct dvb_frontend *fe, struct i2c_adapter *i2c, u8 override_set, u8 override_clear)
{
	struct lnbp21 *lnbp21 = kmalloc(sizeof(struct lnbp21), GFP_KERNEL);
	if (!lnbp21)
		return NULL;

	/* default configuration */
	lnbp21->config = LNBP21_ISEL;
	lnbp21->i2c = i2c;
	fe->sec_priv = lnbp21;

	/* bits which should be forced to '1' */
	lnbp21->override_or = override_set;

	/* bits which should be forced to '0' */
	lnbp21->override_and = ~override_clear;

	/* detect if it is present or not */
	if (lnbp21_set_voltage(fe, SEC_VOLTAGE_OFF)) {
		kfree(lnbp21);
		return NULL;
	}

	/* install release callback */
	fe->ops.release_sec = lnbp21_release;

	/* override frontend ops */
	fe->ops.set_voltage = lnbp21_set_voltage;
	fe->ops.enable_high_lnb_voltage = lnbp21_enable_high_lnb_voltage;

	return fe;
}
EXPORT_SYMBOL(lnbp21_attach);

MODULE_DESCRIPTION("Driver for lnb supply and control ic lnbp21");
MODULE_AUTHOR("Oliver Endriss");
MODULE_LICENSE("GPL");
