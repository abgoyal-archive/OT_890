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
 * USB Driver for ALi m5602 based webcams
 *
 * Copyright (C) 2008 Erik Andrén
 * Copyright (C) 2007 Ilyes Gouta. Based on the m5603x Linux Driver Project.
 * Copyright (C) 2005 m5603x Linux Driver Project <m5602@x3ng.com.br>
 *
 * Portions of code to USB interface and ALi driver software,
 * Copyright (c) 2006 Willem Duinker
 * v4l2 interface modeled after the V4L2 driver
 * for SN9C10x PC Camera Controllers
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation, version 2.
 *
 */

#ifndef M5602_BRIDGE_H_
#define M5602_BRIDGE_H_

#include "gspca.h"

#define MODULE_NAME "ALi m5602"

/*****************************************************************************/

#define M5602_XB_SENSOR_TYPE		0x00
#define M5602_XB_SENSOR_CTRL		0x01
#define M5602_XB_LINE_OF_FRAME_H	0x02
#define M5602_XB_LINE_OF_FRAME_L	0x03
#define M5602_XB_PIX_OF_LINE_H		0x04
#define M5602_XB_PIX_OF_LINE_L		0x05
#define M5602_XB_VSYNC_PARA		0x06
#define M5602_XB_HSYNC_PARA		0x07
#define M5602_XB_TEST_MODE_1		0x08
#define M5602_XB_TEST_MODE_2		0x09
#define M5602_XB_SIG_INI		0x0a
#define M5602_XB_DS_PARA		0x0e
#define M5602_XB_TRIG_PARA		0x0f
#define M5602_XB_CLK_PD			0x10
#define M5602_XB_MCU_CLK_CTRL		0x12
#define M5602_XB_MCU_CLK_DIV		0x13
#define M5602_XB_SEN_CLK_CTRL		0x14
#define M5602_XB_SEN_CLK_DIV		0x15
#define M5602_XB_AUD_CLK_CTRL		0x16
#define M5602_XB_AUD_CLK_DIV		0x17
#define M5602_XB_DEVCTR1		0x41
#define M5602_XB_EPSETR0		0x42
#define M5602_XB_EPAFCTR		0x47
#define M5602_XB_EPBFCTR		0x49
#define M5602_XB_EPEFCTR		0x4f
#define M5602_XB_TEST_REG		0x53
#define M5602_XB_ALT2SIZE		0x54
#define M5602_XB_ALT3SIZE		0x55
#define M5602_XB_OBSFRAME		0x56
#define M5602_XB_PWR_CTL		0x59
#define M5602_XB_ADC_CTRL		0x60
#define M5602_XB_ADC_DATA		0x61
#define M5602_XB_MISC_CTRL		0x62
#define M5602_XB_SNAPSHOT		0x63
#define M5602_XB_SCRATCH_1		0x64
#define M5602_XB_SCRATCH_2		0x65
#define M5602_XB_SCRATCH_3		0x66
#define M5602_XB_SCRATCH_4		0x67
#define M5602_XB_I2C_CTRL		0x68
#define M5602_XB_I2C_CLK_DIV		0x69
#define M5602_XB_I2C_DEV_ADDR		0x6a
#define M5602_XB_I2C_REG_ADDR		0x6b
#define M5602_XB_I2C_DATA		0x6c
#define M5602_XB_I2C_STATUS		0x6d
#define M5602_XB_GPIO_DAT_H		0x70
#define M5602_XB_GPIO_DAT_L		0x71
#define M5602_XB_GPIO_DIR_H		0x72
#define M5602_XB_GPIO_DIR_L		0x73
#define M5602_XB_GPIO_EN_H		0x74
#define M5602_XB_GPIO_EN_L		0x75
#define M5602_XB_GPIO_DAT		0x76
#define M5602_XB_GPIO_DIR		0x77
#define M5602_XB_MISC_CTL		0x70

#define I2C_BUSY 0x80

/*****************************************************************************/

/* Driver info */
#define DRIVER_AUTHOR "ALi m5602 Linux Driver Project"
#define DRIVER_DESC "ALi m5602 webcam driver"

#define M5602_ISOC_ENDPOINT_ADDR 0x81
#define M5602_INTR_ENDPOINT_ADDR 0x82

#define M5602_URB_MSG_TIMEOUT   5000

/*****************************************************************************/

/* A skeleton used for sending messages to the m5602 bridge */
static const unsigned char bridge_urb_skeleton[] = {
	0x13, 0x00, 0x81, 0x00
};

/* A skeleton used for sending messages to the sensor */
static const unsigned char sensor_urb_skeleton[] = {
	0x23, M5602_XB_GPIO_EN_H, 0x81, 0x06,
	0x23, M5602_XB_MISC_CTRL, 0x81, 0x80,
	0x13, M5602_XB_I2C_DEV_ADDR, 0x81, 0x00,
	0x13, M5602_XB_I2C_REG_ADDR, 0x81, 0x00,
	0x13, M5602_XB_I2C_DATA, 0x81, 0x00,
	0x13, M5602_XB_I2C_CTRL, 0x81, 0x11
};

struct sd {
	struct gspca_dev gspca_dev;

	/* The name of the m5602 camera */
	char *name;

	/* A pointer to the currently connected sensor */
	struct m5602_sensor *sensor;

	struct sd_desc *desc;

	/* The current frame's id, used to detect frame boundaries */
	u8 frame_id;

	/* The current frame count */
	u32 frame_count;
};

int m5602_read_bridge(
	struct sd *sd, u8 address, u8 *i2c_data);

int m5602_write_bridge(
	struct sd *sd, u8 address, u8 i2c_data);

int m5602_write_sensor(struct sd *sd, const u8 address,
		       u8 *i2c_data, const u8 len);

int m5602_read_sensor(struct sd *sd, const u8 address,
		      u8 *i2c_data, const u8 len);

#endif
