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
	STB0899 Multistandard Frontend driver
	Copyright (C) Manu Abraham (abraham.manu@gmail.com)

	Copyright (C) ST Microelectronics

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

#ifndef __STB0899_DRV_H
#define __STB0899_DRV_H

#include <linux/kernel.h>
#include <linux/module.h>

#include "dvb_frontend.h"

#define STB0899_TSMODE_SERIAL		1
#define STB0899_CLKPOL_FALLING		2
#define STB0899_CLKNULL_PARITY		3
#define STB0899_SYNC_FORCED		4
#define STB0899_FECMODE_DSS		5

struct stb0899_s1_reg {
	u16	address;
	u8	data;
};

struct stb0899_s2_reg {
	u16	offset;
	u32	base_address;
	u32	data;
};

enum stb0899_inversion {
	IQ_SWAP_OFF	= 0,
	IQ_SWAP_ON,
	IQ_SWAP_AUTO
};

#define STB0899_GPIO00				0xf140
#define STB0899_GPIO01				0xf141
#define STB0899_GPIO02				0xf142
#define STB0899_GPIO03				0xf143
#define STB0899_GPIO04				0xf144
#define STB0899_GPIO05				0xf145
#define STB0899_GPIO06				0xf146
#define STB0899_GPIO07				0xf147
#define STB0899_GPIO08				0xf148
#define STB0899_GPIO09				0xf149
#define STB0899_GPIO10				0xf14a
#define STB0899_GPIO11				0xf14b
#define STB0899_GPIO12				0xf14c
#define STB0899_GPIO13				0xf14d
#define STB0899_GPIO14				0xf14e
#define STB0899_GPIO15				0xf14f
#define STB0899_GPIO16				0xf150
#define STB0899_GPIO17				0xf151
#define STB0899_GPIO18				0xf152
#define STB0899_GPIO19				0xf153
#define STB0899_GPIO20				0xf154

#define STB0899_GPIOPULLUP			0x01 /* Output device is connected to Vdd */
#define STB0899_GPIOPULLDN			0x00 /* Output device is connected to Vss */

#define STB0899_POSTPROC_GPIO_POWER		0x00
#define STB0899_POSTPROC_GPIO_LOCK		0x01

/*
 * Post process output configuration control
 * 1. POWER ON/OFF		(index 0)
 * 2. FE_HAS_LOCK/LOCK_LOSS	(index 1)
 *
 * @gpio 	= one of the above listed GPIO's
 * @level	= output state: pulled up or low
 */
struct stb0899_postproc {
	u16	gpio;
	u8	level;
};

struct stb0899_config {
	const struct stb0899_s1_reg	*init_dev;
	const struct stb0899_s2_reg	*init_s2_demod;
	const struct stb0899_s1_reg	*init_s1_demod;
	const struct stb0899_s2_reg	*init_s2_fec;
	const struct stb0899_s1_reg	*init_tst;

	const struct stb0899_postproc	*postproc;

	enum stb0899_inversion		inversion;

	u32	xtal_freq;

	u8	demod_address;
	u8	ts_output_mode;
	u8	block_sync_mode;
	u8	ts_pfbit_toggle;

	u8	clock_polarity;
	u8	data_clk_parity;
	u8	fec_mode;
	u8	data_output_ctl;
	u8	data_fifo_mode;
	u8	out_rate_comp;
	u8	i2c_repeater;
//	int	inversion;
	int	lo_clk;
	int	hi_clk;

	u32	esno_ave;
	u32	esno_quant;
	u32	avframes_coarse;
	u32	avframes_fine;
	u32	miss_threshold;
	u32	uwp_threshold_acq;
	u32	uwp_threshold_track;
	u32	uwp_threshold_sof;
	u32	sof_search_timeout;

	u32	btr_nco_bits;
	u32	btr_gain_shift_offset;
	u32	crl_nco_bits;
	u32	ldpc_max_iter;

	int (*tuner_set_frequency)(struct dvb_frontend *fe, u32 frequency);
	int (*tuner_get_frequency)(struct dvb_frontend *fe, u32 *frequency);
	int (*tuner_set_bandwidth)(struct dvb_frontend *fe, u32 bandwidth);
	int (*tuner_get_bandwidth)(struct dvb_frontend *fe, u32 *bandwidth);
	int (*tuner_set_rfsiggain)(struct dvb_frontend *fe, u32 rf_gain);
};

#if defined(CONFIG_DVB_STB0899) || (defined(CONFIG_DVB_STB0899_MODULE) && defined(MODULE))

extern struct dvb_frontend *stb0899_attach(struct stb0899_config *config,
					   struct i2c_adapter *i2c);

#else

static inline struct dvb_frontend *stb0899_attach(struct stb0899_config *config,
						  struct i2c_adapter *i2c)
{
	printk(KERN_WARNING "%s: Driver disabled by Kconfig\n", __func__);
	return NULL;
}

#endif //CONFIG_DVB_STB0899


#endif
