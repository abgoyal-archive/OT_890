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
 * Driver for DiBcom DiB3000MC/P-demodulator.
 *
 * Copyright (C) 2004-6 DiBcom (http://www.dibcom.fr/)
 * Copyright (C) 2004-5 Patrick Boettcher (patrick.boettcher\@desy.de)
 *
 * This code is partially based on the previous dib3000mc.c .
 *
 * This program is free software; you can redistribute it and/or
 *	modify it under the terms of the GNU General Public License as
 *	published by the Free Software Foundation, version 2.
 */
#ifndef DIB3000MC_H
#define DIB3000MC_H

#include "dibx000_common.h"

struct dib3000mc_config {
	struct dibx000_agc_config *agc;

	u8 phase_noise_mode;
	u8 impulse_noise_mode;

	u8  pwm3_inversion;
	u8  use_pwm3;
	u16 pwm3_value;

	u16 max_time;
	u16 ln_adc_level;

	u8 agc_command1 :1;
	u8 agc_command2 :1;

	u8 mobile_mode;

	u8 output_mpeg2_in_188_bytes;
};

#define DEFAULT_DIB3000MC_I2C_ADDRESS 16
#define DEFAULT_DIB3000P_I2C_ADDRESS  24

#if defined(CONFIG_DVB_DIB3000MC) || (defined(CONFIG_DVB_DIB3000MC_MODULE) && defined(MODULE))
extern struct dvb_frontend * dib3000mc_attach(struct i2c_adapter *i2c_adap, u8 i2c_addr, struct dib3000mc_config *cfg);
#else
static inline struct dvb_frontend * dib3000mc_attach(struct i2c_adapter *i2c_adap, u8 i2c_addr, struct dib3000mc_config *cfg)
{
	printk(KERN_WARNING "%s: driver disabled by Kconfig\n", __func__);
	return NULL;
}
#endif // CONFIG_DVB_DIB3000MC

extern int dib3000mc_i2c_enumeration(struct i2c_adapter *i2c, int no_of_demods, u8 default_addr, struct dib3000mc_config cfg[]);

extern struct i2c_adapter * dib3000mc_get_tuner_i2c_master(struct dvb_frontend *demod, int gating);

extern int dib3000mc_pid_control(struct dvb_frontend *fe, int index, int pid,int onoff);
extern int dib3000mc_pid_parse(struct dvb_frontend *fe, int onoff);

extern void dib3000mc_set_config(struct dvb_frontend *, struct dib3000mc_config *);

#endif
