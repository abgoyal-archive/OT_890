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

#ifndef _DVB_USB_DIGITV_H_
#define _DVB_USB_DIGITV_H_

#define DVB_USB_LOG_PREFIX "digitv"
#include "dvb-usb.h"

struct digitv_state {
    int is_nxt6000;
};

/* protocol (from usblogging and the SDK:
 *
 * Always 7 bytes bulk message(s) for controlling
 *
 * First byte describes the command. Reads are 2 consecutive transfer (as always).
 *
 * General structure:
 *
 * write or first message of a read:
 * <cmdbyte> VV <len> B0 B1 B2 B3
 *
 * second message of a read
 * <cmdbyte> VV <len> R0 R1 R2 R3
 *
 * whereas 0 < len <= 4
 *
 * I2C address is stored somewhere inside the device.
 *
 * 0x01 read from EEPROM
 *  VV = offset; B* = 0; R* = value(s)
 *
 * 0x02 read register of the COFDM
 *  VV = register; B* = 0; R* = value(s)
 *
 * 0x05 write register of the COFDM
 *  VV = register; B* = value(s);
 *
 * 0x06 write to the tuner (only for NXT6000)
 *  VV = 0; B* = PLL data; len = 4;
 *
 * 0x03 read remote control
 *  VV = 0; B* = 0; len = 4; R* = key
 *
 * 0x07 write to the remote (don't know why one should this, resetting ?)
 *  VV = 0; B* = key; len = 4;
 *
 * 0x08 write remote type
 *  VV = 0; B[0] = 0x01, len = 4
 *
 * 0x09 write device init
 *  TODO
 */
#define USB_READ_EEPROM         1

#define USB_READ_COFDM          2
#define USB_WRITE_COFDM         5

#define USB_WRITE_TUNER         6

#define USB_READ_REMOTE         3
#define USB_WRITE_REMOTE        7
#define USB_WRITE_REMOTE_TYPE   8

#define USB_DEV_INIT            9

#endif
