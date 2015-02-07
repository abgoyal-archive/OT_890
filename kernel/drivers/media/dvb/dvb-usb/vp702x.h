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

#ifndef _DVB_USB_VP7021_H_
#define _DVB_USB_VP7021_H_

#define DVB_USB_LOG_PREFIX "vp702x"
#include "dvb-usb.h"

extern int dvb_usb_vp702x_debug;
#define deb_info(args...) dprintk(dvb_usb_vp702x_debug,0x01,args)
#define deb_xfer(args...) dprintk(dvb_usb_vp702x_debug,0x02,args)
#define deb_rc(args...)   dprintk(dvb_usb_vp702x_debug,0x04,args)
#define deb_fe(args...)   dprintk(dvb_usb_vp702x_debug,0x08,args)

/* commands are read and written with USB control messages */

/* consecutive read/write operation */
#define REQUEST_OUT		0xB2
#define REQUEST_IN		0xB3

/* the out-buffer of these consecutive operations contain sub-commands when b[0] = 0
 * request: 0xB2; i: 0; v: 0; b[0] = 0, b[1] = subcmd, additional buffer
 * the returning buffer looks as follows
 * request: 0xB3; i: 0; v: 0; b[0] = 0xB3, additional buffer */

#define GET_TUNER_STATUS	0x05
/* additional in buffer:
 * 0   1   2    3              4   5   6               7       8
 * N/A N/A 0x05 signal-quality N/A N/A signal-strength lock==0 N/A */

#define GET_SYSTEM_STRING	0x06
/* additional in buffer:
 * 0   1   2   3   4   5   6   7   8
 * N/A 'U' 'S' 'B' '7' '0' '2' 'X' N/A */

#define SET_DISEQC_CMD		0x08
/* additional out buffer:
 * 0    1  2  3  4
 * len  X1 X2 X3 X4
 * additional in buffer:
 * 0   1 2
 * N/A 0 0   b[1] == b[2] == 0 -> success, failure otherwise */

#define SET_LNB_POWER		0x09
/* additional out buffer:
 * 0    1    2
 * 0x00 0xff 1 = on, 0 = off
 * additional in buffer:
 * 0   1 2
 * N/A 0 0   b[1] == b[2] == 0 -> success failure otherwise */

#define GET_MAC_ADDRESS		0x0A
/* #define GET_MAC_ADDRESS   0x0B */
/* additional in buffer:
 * 0   1   2            3    4    5    6    7    8
 * N/A N/A 0x0A or 0x0B MAC0 MAC1 MAC2 MAC3 MAC4 MAC5 */

#define SET_PID_FILTER		0x11
/* additional in buffer:
 * 0        1        ... 14       15       16
 * PID0_MSB PID0_LSB ... PID7_MSB PID7_LSB PID_active (bits) */

/* request: 0xB2; i: 0; v: 0;
 * b[0] != 0 -> tune and lock a channel
 * 0     1     2       3      4      5      6    7
 * freq0 freq1 divstep srate0 srate1 srate2 flag chksum
 */

/* one direction requests */
#define READ_REMOTE_REQ		0xB4
/* IN  i: 0; v: 0; b[0] == request, b[1] == key */

#define READ_PID_NUMBER_REQ	0xB5
/* IN  i: 0; v: 0; b[0] == request, b[1] == 0, b[2] = pid number */

#define WRITE_EEPROM_REQ	0xB6
/* OUT i: offset; v: value to write; no extra buffer */

#define READ_EEPROM_REQ		0xB7
/* IN  i: bufferlen; v: offset; buffer with bufferlen bytes */

#define READ_STATUS		0xB8
/* IN  i: 0; v: 0; bufferlen 10 */

#define READ_TUNER_REG_REQ	0xB9
/* IN  i: 0; v: register; b[0] = value */

#define READ_FX2_REG_REQ	0xBA
/* IN  i: offset; v: 0; b[0] = value */

#define WRITE_FX2_REG_REQ	0xBB
/* OUT i: offset; v: value to write; 1 byte extra buffer */

#define SET_TUNER_POWER_REQ	0xBC
/* IN  i: 0 = power off, 1 = power on */

#define WRITE_TUNER_REG_REQ	0xBD
/* IN  i: register, v: value to write, no extra buffer */

#define RESET_TUNER		0xBE
/* IN  i: 0, v: 0, no extra buffer */

extern struct dvb_frontend * vp702x_fe_attach(struct dvb_usb_device *d);

extern int vp702x_usb_inout_op(struct dvb_usb_device *d, u8 *o, int olen, u8 *i, int ilen, int msec);
extern int vp702x_usb_in_op(struct dvb_usb_device *d, u8 req, u16 value, u16 index, u8 *b, int blen);

#endif
