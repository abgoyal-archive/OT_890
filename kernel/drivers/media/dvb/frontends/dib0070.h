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
 * Linux-DVB Driver for DiBcom's DiB0070 base-band RF Tuner.
 *
 * Copyright (C) 2005-7 DiBcom (http://www.dibcom.fr/)
 *
 * This program is free software; you can redistribute it and/or
 *	modify it under the terms of the GNU General Public License as
 *	published by the Free Software Foundation, version 2.
 */
#ifndef DIB0070_H
#define DIB0070_H

struct dvb_frontend;
struct i2c_adapter;

#define DEFAULT_DIB0070_I2C_ADDRESS 0x60

struct dib0070_config {
	u8 i2c_address;

	/* tuner pins controlled externally */
	int (*reset) (struct dvb_frontend *, int);
	int (*sleep) (struct dvb_frontend *, int);

	/*  offset in kHz */
	int freq_offset_khz_uhf;
	int freq_offset_khz_vhf;

	u8 osc_buffer_state; /* 0= normal, 1= tri-state */
	u32  clock_khz;
	u8 clock_pad_drive; /* (Drive + 1) * 2mA */

	u8 invert_iq; /* invert Q - in case I or Q is inverted on the board */

	u8 force_crystal_mode; /* if == 0 -> decision is made in the driver default: <24 -> 2, >=24 -> 1 */

	u8 flip_chip;
};

#if defined(CONFIG_DVB_TUNER_DIB0070) || (defined(CONFIG_DVB_TUNER_DIB0070_MODULE) && defined(MODULE))
extern struct dvb_frontend *dib0070_attach(struct dvb_frontend *fe,
					   struct i2c_adapter *i2c,
					   struct dib0070_config *cfg);
extern u16 dib0070_wbd_offset(struct dvb_frontend *);
#else
static inline struct dvb_frontend *dib0070_attach(struct dvb_frontend *fe,
						  struct i2c_adapter *i2c,
						  struct dib0070_config *cfg)
{
	printk(KERN_WARNING "%s: driver disabled by Kconfig\n", __func__);
	return NULL;
}

static inline u16 dib0070_wbd_offset(struct dvb_frontend *fe)
{
	printk(KERN_WARNING "%s: driver disabled by Kconfig\n", __func__);
	return -ENODEV;
}
#endif

extern void dib0070_ctrl_agc_filter(struct dvb_frontend *, uint8_t open);

#endif
