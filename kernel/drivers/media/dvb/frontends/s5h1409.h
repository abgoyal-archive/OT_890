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
    Samsung S5H1409 VSB/QAM demodulator driver

    Copyright (C) 2006 Steven Toth <stoth@linuxtv.org>

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

#ifndef __S5H1409_H__
#define __S5H1409_H__

#include <linux/dvb/frontend.h>

struct s5h1409_config {
	/* the demodulator's i2c address */
	u8 demod_address;

	/* serial/parallel output */
#define S5H1409_PARALLEL_OUTPUT 0
#define S5H1409_SERIAL_OUTPUT   1
	u8 output_mode;

	/* GPIO Setting */
#define S5H1409_GPIO_OFF 0
#define S5H1409_GPIO_ON  1
	u8 gpio;

	/* IF Freq for QAM in KHz, VSB is hardcoded to 5380 */
	u16 qam_if;

	/* Spectral Inversion */
#define S5H1409_INVERSION_OFF 0
#define S5H1409_INVERSION_ON  1
	u8 inversion;

	/* Return lock status based on tuner lock, or demod lock */
#define S5H1409_TUNERLOCKING 0
#define S5H1409_DEMODLOCKING 1
	u8 status_mode;

	/* MPEG signal timing */
#define S5H1409_MPEGTIMING_CONTINOUS_INVERTING_CLOCK       0
#define S5H1409_MPEGTIMING_CONTINOUS_NONINVERTING_CLOCK    1
#define S5H1409_MPEGTIMING_NONCONTINOUS_INVERTING_CLOCK    2
#define S5H1409_MPEGTIMING_NONCONTINOUS_NONINVERTING_CLOCK 3
	u16 mpeg_timing;
};

#if defined(CONFIG_DVB_S5H1409) || (defined(CONFIG_DVB_S5H1409_MODULE) \
	&& defined(MODULE))
extern struct dvb_frontend *s5h1409_attach(const struct s5h1409_config *config,
					   struct i2c_adapter *i2c);
#else
static inline struct dvb_frontend *s5h1409_attach(
	const struct s5h1409_config *config,
	struct i2c_adapter *i2c)
{
	printk(KERN_WARNING "%s: driver disabled by Kconfig\n", __func__);
	return NULL;
}
#endif /* CONFIG_DVB_S5H1409 */

#endif /* __S5H1409_H__ */

/*
 * Local variables:
 * c-basic-offset: 8
 */
