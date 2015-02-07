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
    tda18271.h - header for the Philips / NXP TDA18271 silicon tuner

    Copyright (C) 2007, 2008 Michael Krufky <mkrufky@linuxtv.org>

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/

#ifndef __TDA18271_H__
#define __TDA18271_H__

#include <linux/i2c.h>
#include "dvb_frontend.h"

struct tda18271_std_map_item {
	u16 if_freq;

	/* EP3[4:3] */
	unsigned int agc_mode:2;
	/* EP3[2:0] */
	unsigned int std:3;
	/* EP4[7] */
	unsigned int fm_rfn:1;
	/* EP4[4:2] */
	unsigned int if_lvl:3;
	/* EB22[6:0] */
	unsigned int rfagc_top:7;
};

struct tda18271_std_map {
	struct tda18271_std_map_item fm_radio;
	struct tda18271_std_map_item atv_b;
	struct tda18271_std_map_item atv_dk;
	struct tda18271_std_map_item atv_gh;
	struct tda18271_std_map_item atv_i;
	struct tda18271_std_map_item atv_l;
	struct tda18271_std_map_item atv_lc;
	struct tda18271_std_map_item atv_mn;
	struct tda18271_std_map_item atsc_6;
	struct tda18271_std_map_item dvbt_6;
	struct tda18271_std_map_item dvbt_7;
	struct tda18271_std_map_item dvbt_8;
	struct tda18271_std_map_item qam_6;
	struct tda18271_std_map_item qam_8;
};

enum tda18271_role {
	TDA18271_MASTER = 0,
	TDA18271_SLAVE,
};

enum tda18271_i2c_gate {
	TDA18271_GATE_AUTO = 0,
	TDA18271_GATE_ANALOG,
	TDA18271_GATE_DIGITAL,
};

struct tda18271_config {
	/* override default if freq / std settings (optional) */
	struct tda18271_std_map *std_map;

	/* master / slave tuner: master uses main pll, slave uses cal pll */
	enum tda18271_role role;

	/* use i2c gate provided by analog or digital demod */
	enum tda18271_i2c_gate gate;

	/* some i2c providers cant write all 39 registers at once */
	unsigned int small_i2c:1;
};

#if defined(CONFIG_MEDIA_TUNER_TDA18271) || (defined(CONFIG_MEDIA_TUNER_TDA18271_MODULE) && defined(MODULE))
extern struct dvb_frontend *tda18271_attach(struct dvb_frontend *fe, u8 addr,
					    struct i2c_adapter *i2c,
					    struct tda18271_config *cfg);
#else
static inline struct dvb_frontend *tda18271_attach(struct dvb_frontend *fe,
						   u8 addr,
						   struct i2c_adapter *i2c,
						   struct tda18271_config *cfg)
{
	printk(KERN_WARNING "%s: driver disabled by Kconfig\n", __func__);
	return NULL;
}
#endif

#endif /* __TDA18271_H__ */
