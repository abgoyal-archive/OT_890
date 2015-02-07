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
 *  Driver for Zarlink DVB-T MT352 demodulator
 *
 *  Written by Holger Waechtler <holger@qanu.de>
 *	 and Daniel Mack <daniel@qanu.de>
 *
 *  AVerMedia AVerTV DVB-T 771 support by
 *       Wolfram Joost <dbox2@frokaschwei.de>
 *
 *  Support for Samsung TDTC9251DH01C(M) tuner
 *  Copyright (C) 2004 Antonio Mancuso <antonio.mancuso@digitaltelevision.it>
 *                     Amauri  Celani  <acelani@essegi.net>
 *
 *  DVICO FusionHDTV DVB-T1 and DVICO FusionHDTV DVB-T Lite support by
 *       Christopher Pascoe <c.pascoe@itee.uq.edu.au>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.=
 */

#ifndef MT352_H
#define MT352_H

#include <linux/dvb/frontend.h>

struct mt352_config
{
	/* the demodulator's i2c address */
	u8 demod_address;

	/* frequencies in kHz */
	int adc_clock;  // default: 20480
	int if2;        // default: 36166

	/* set if no pll is connected to the secondary i2c bus */
	int no_tuner;

	/* Initialise the demodulator and PLL. Cannot be NULL */
	int (*demod_init)(struct dvb_frontend* fe);
};

#if defined(CONFIG_DVB_MT352) || (defined(CONFIG_DVB_MT352_MODULE) && defined(MODULE))
extern struct dvb_frontend* mt352_attach(const struct mt352_config* config,
					 struct i2c_adapter* i2c);
#else
static inline struct dvb_frontend* mt352_attach(const struct mt352_config* config,
					 struct i2c_adapter* i2c)
{
	printk(KERN_WARNING "%s: driver disabled by Kconfig\n", __func__);
	return NULL;
}
#endif // CONFIG_DVB_MT352

static inline int mt352_write(struct dvb_frontend *fe, u8 *buf, int len) {
	int r = 0;
	if (fe->ops.write)
		r = fe->ops.write(fe, buf, len);
	return r;
}

#endif // MT352_H
