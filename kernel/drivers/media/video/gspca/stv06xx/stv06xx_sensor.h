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
 * Copyright (c) 2001 Jean-Fredric Clere, Nikolas Zimmermann, Georg Acher
 *		      Mark Cave-Ayland, Carlo E Prelz, Dick Streefland
 * Copyright (c) 2002, 2003 Tuukka Toivonen
 * Copyright (c) 2008 Erik Andrén
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * P/N 861037:      Sensor HDCS1000        ASIC STV0600
 * P/N 861050-0010: Sensor HDCS1000        ASIC STV0600
 * P/N 861050-0020: Sensor Photobit PB100  ASIC STV0600-1 - QuickCam Express
 * P/N 861055:      Sensor ST VV6410       ASIC STV0610   - LEGO cam
 * P/N 861075-0040: Sensor HDCS1000        ASIC
 * P/N 961179-0700: Sensor ST VV6410       ASIC STV0602   - Dexxa WebCam USB
 * P/N 861040-0000: Sensor ST VV6410       ASIC STV0610   - QuickCam Web
 */

#ifndef STV06XX_SENSOR_H_
#define STV06XX_SENSOR_H_

#include "stv06xx.h"

#define IS_850(sd)	((sd)->gspca_dev.dev->descriptor.idProduct == 0x850)
#define IS_870(sd)	((sd)->gspca_dev.dev->descriptor.idProduct == 0x870)
#define IS_1020(sd)	((sd)->sensor == &stv06xx_sensor_hdcs1020)

extern const struct stv06xx_sensor stv06xx_sensor_vv6410;
extern const struct stv06xx_sensor stv06xx_sensor_hdcs1x00;
extern const struct stv06xx_sensor stv06xx_sensor_hdcs1020;
extern const struct stv06xx_sensor stv06xx_sensor_pb0100;

#define STV06XX_MAX_CTRLS		(V4L2_CID_LASTP1 - V4L2_CID_BASE + 10)

struct stv06xx_sensor {
	/* Defines the name of a sensor */
	char name[32];

	/* Sensor i2c address */
	u8 i2c_addr;

	/* Flush value*/
	u8 i2c_flush;

	/* length of an i2c word */
	u8 i2c_len;

	/* Probes if the sensor is connected */
	int (*probe)(struct sd *sd);

	/* Performs a initialization sequence */
	int (*init)(struct sd *sd);

	/* Executed at device disconnect */
	void (*disconnect)(struct sd *sd);

	/* Reads a sensor register */
	int (*read_sensor)(struct sd *sd, const u8 address,
	      u8 *i2c_data, const u8 len);

	/* Writes to a sensor register */
	int (*write_sensor)(struct sd *sd, const u8 address,
	      u8 *i2c_data, const u8 len);

	/* Instructs the sensor to start streaming */
	int (*start)(struct sd *sd);

	/* Instructs the sensor to stop streaming */
	int (*stop)(struct sd *sd);

	/* Instructs the sensor to dump all its contents */
	int (*dump)(struct sd *sd);

	int nctrls;
	struct ctrl ctrls[STV06XX_MAX_CTRLS];

	char nmodes;
	struct v4l2_pix_format modes[];
};

#endif
