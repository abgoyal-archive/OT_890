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
    TDA10021/TDA10023  - Single Chip Cable Channel Receiver driver module
			 used on the the Siemens DVB-C cards

    Copyright (C) 1999 Convergence Integrated Media GmbH <ralph@convergence.de>
    Copyright (C) 2004 Markus Schulz <msc@antzsystem.de>
		   Support for TDA10021

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

#ifndef TDA1002x_H
#define TDA1002x_H

#include <linux/dvb/frontend.h>

struct tda1002x_config {
	/* the demodulator's i2c address */
	u8 demod_address;
	u8 invert;
};

enum tda10023_output_mode {
	TDA10023_OUTPUT_MODE_PARALLEL_A = 0xe0,
	TDA10023_OUTPUT_MODE_PARALLEL_B = 0xa1,
	TDA10023_OUTPUT_MODE_PARALLEL_C = 0xa0,
	TDA10023_OUTPUT_MODE_SERIAL, /* TODO: not implemented */
};

struct tda10023_config {
	/* the demodulator's i2c address */
	u8 demod_address;
	u8 invert;

	/* clock settings */
	u32 xtal; /* defaults: 28920000 */
	u8 pll_m; /* defaults: 8 */
	u8 pll_p; /* defaults: 4 */
	u8 pll_n; /* defaults: 1 */

	/* MPEG2 TS output mode */
	u8 output_mode;

	/* input freq offset + baseband conversion type */
	u16 deltaf;
};

#if defined(CONFIG_DVB_TDA10021) || (defined(CONFIG_DVB_TDA10021_MODULE) && defined(MODULE))
extern struct dvb_frontend* tda10021_attach(const struct tda1002x_config* config,
					    struct i2c_adapter* i2c, u8 pwm);
#else
static inline struct dvb_frontend* tda10021_attach(const struct tda1002x_config* config,
					    struct i2c_adapter* i2c, u8 pwm)
{
	printk(KERN_WARNING "%s: driver disabled by Kconfig\n", __func__);
	return NULL;
}
#endif // CONFIG_DVB_TDA10021

#if defined(CONFIG_DVB_TDA10023) || \
	(defined(CONFIG_DVB_TDA10023_MODULE) && defined(MODULE))
extern struct dvb_frontend *tda10023_attach(
	const struct tda10023_config *config,
	struct i2c_adapter *i2c, u8 pwm);
#else
static inline struct dvb_frontend *tda10023_attach(
	const struct tda10023_config *config,
	struct i2c_adapter *i2c, u8 pwm)
{
	printk(KERN_WARNING "%s: driver disabled by Kconfig\n", __func__);
	return NULL;
}
#endif // CONFIG_DVB_TDA10023

#endif // TDA1002x_H
