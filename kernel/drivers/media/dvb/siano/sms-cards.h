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
 *  Card-specific functions for the Siano SMS1xxx USB dongle
 *
 *  Copyright (c) 2008 Michael Krufky <mkrufky@linuxtv.org>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License version 2 as
 *  published by the Free Software Foundation;
 *
 *  Software distributed under the License is distributed on an "AS IS"
 *  basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.
 *
 *  See the GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#ifndef __SMS_CARDS_H__
#define __SMS_CARDS_H__

#include <linux/usb.h>
#include "smscoreapi.h"

#define SMS_BOARD_UNKNOWN 0
#define SMS1XXX_BOARD_SIANO_STELLAR 1
#define SMS1XXX_BOARD_SIANO_NOVA_A  2
#define SMS1XXX_BOARD_SIANO_NOVA_B  3
#define SMS1XXX_BOARD_SIANO_VEGA    4
#define SMS1XXX_BOARD_HAUPPAUGE_CATAMOUNT 5
#define SMS1XXX_BOARD_HAUPPAUGE_OKEMO_A 6
#define SMS1XXX_BOARD_HAUPPAUGE_OKEMO_B 7
#define SMS1XXX_BOARD_HAUPPAUGE_WINDHAM 8
#define SMS1XXX_BOARD_HAUPPAUGE_TIGER_MINICARD 9
#define SMS1XXX_BOARD_HAUPPAUGE_TIGER_MINICARD_R2 10

struct sms_board {
	enum sms_device_type_st type;
	char *name, *fw[DEVICE_MODE_MAX];

	/* gpios */
	int led_power, led_hi, led_lo, lna_ctrl;
};

struct sms_board *sms_get_board(int id);

int sms_board_setup(struct smscore_device_t *coredev);

#define SMS_LED_OFF 0
#define SMS_LED_LO  1
#define SMS_LED_HI  2
int sms_board_led_feedback(struct smscore_device_t *coredev, int led);
int sms_board_power(struct smscore_device_t *coredev, int onoff);

extern struct usb_device_id smsusb_id_table[];

#endif /* __SMS_CARDS_H__ */
