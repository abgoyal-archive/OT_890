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

/* Common header-file of the Linux driver for the TwinhanDTV Alpha/MagicBoxII
 * USB2.0 DVB-T receiver.
 *
 * Copyright (C) 2004-5 Patrick Boettcher (patrick.boettcher@desy.de)
 *
 * Thanks to Twinhan who kindly provided hardware and information.
 *
 *	This program is free software; you can redistribute it and/or modify it
 *	under the terms of the GNU General Public License as published by the Free
 *	Software Foundation, version 2.
 *
 * see Documentation/dvb/README.dvb-usb for more information
 */
#ifndef _DVB_USB_VP7045_H_
#define _DVB_USB_VP7045_H_

#define DVB_USB_LOG_PREFIX "vp7045"
#include "dvb-usb.h"

/* vp7045 commands */

/* Twinhan Vendor requests */
#define TH_COMMAND_IN                     0xC0
#define TH_COMMAND_OUT                    0xC1

/* command bytes */
#define TUNER_REG_READ                    0x03
#define TUNER_REG_WRITE                   0x04

#define RC_VAL_READ                       0x05
 #define RC_NO_KEY                        0x44

#define SET_TUNER_POWER                   0x06
#define CHECK_TUNER_POWER                 0x12
 #define Tuner_Power_ON                   1
 #define Tuner_Power_OFF                  0

#define GET_USB_SPEED                     0x07
 #define USB_SPEED_LOW                    0
 #define USB_SPEED_FULL                   1
 #define USB_SPEED_HIGH                   2

#define LOCK_TUNER_COMMAND                0x09

#define TUNER_SIGNAL_READ                 0x0A

/* FX2 eeprom */
#define SET_EE_VALUE                      0x10
#define GET_EE_VALUE                      0x11
 #define FX2_ID_ADDR                      0x00
 #define VID_MSB_ADDR                     0x02
 #define VID_LSB_ADDR                     0x01
 #define PID_MSB_ADDR                     0x04
 #define PID_LSB_ADDR                     0x03
 #define MAC_0_ADDR                       0x07
 #define MAC_1_ADDR                       0x08
 #define MAC_2_ADDR                       0x09
 #define MAC_3_ADDR                       0x0a
 #define MAC_4_ADDR                       0x0b
 #define MAC_5_ADDR                       0x0c

#define RESET_FX2                         0x13

#define FW_VERSION_READ                   0x0B
#define VENDOR_STRING_READ                0x0C
#define PRODUCT_STRING_READ               0x0D
#define FW_BCD_VERSION_READ               0x14

extern struct dvb_frontend * vp7045_fe_attach(struct dvb_usb_device *d);
extern int vp7045_usb_op(struct dvb_usb_device *d, u8 cmd, u8 *out, int outlen, u8 *in, int inlen,int msec);
extern u8 vp7045_read_reg(struct dvb_usb_device *d, u8 reg);

#endif
