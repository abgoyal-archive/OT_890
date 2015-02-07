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

  Broadcom B43legacy wireless driver

  Copyright (c) 2005 Martin Langer <martin-langer@gmx.de>,
		     Stefano Brivio <stefano.brivio@polimi.it>
		     Michael Buesch <mbuesch@freenet.de>
		     Danny van Dyk <kugelfang@gentoo.org>
		     Andreas Jaggi <andreas.jaggi@waterwave.ch>

  Some parts of the code in this file are derived from the ipw2200
  driver  Copyright(c) 2003 - 2004 Intel Corporation.

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; see the file COPYING.  If not, write to
  the Free Software Foundation, Inc., 51 Franklin Steet, Fifth Floor,
  Boston, MA 02110-1301, USA.

*/

#ifndef B43legacy_RADIO_H_
#define B43legacy_RADIO_H_

#include "b43legacy.h"


#define B43legacy_RADIO_DEFAULT_CHANNEL_BG	6

/* Force antenna 0. */
#define B43legacy_RADIO_TXANTENNA_0		0
/* Force antenna 1. */
#define B43legacy_RADIO_TXANTENNA_1		1
/* Use the RX antenna, that was selected for the most recently
 * received good PLCP header.
 */
#define B43legacy_RADIO_TXANTENNA_LASTPLCP	3
#define B43legacy_RADIO_TXANTENNA_DEFAULT	B43legacy_RADIO_TXANTENNA_LASTPLCP

#define B43legacy_RADIO_INTERFMODE_NONE		0
#define B43legacy_RADIO_INTERFMODE_NONWLAN	1
#define B43legacy_RADIO_INTERFMODE_MANUALWLAN	2
#define B43legacy_RADIO_INTERFMODE_AUTOWLAN	3


void b43legacy_radio_lock(struct b43legacy_wldev *dev);
void b43legacy_radio_unlock(struct b43legacy_wldev *dev);

u16 b43legacy_radio_read16(struct b43legacy_wldev *dev, u16 offset);
void b43legacy_radio_write16(struct b43legacy_wldev *dev, u16 offset, u16 val);

u16 b43legacy_radio_init2050(struct b43legacy_wldev *dev);

void b43legacy_radio_turn_on(struct b43legacy_wldev *dev);
void b43legacy_radio_turn_off(struct b43legacy_wldev *dev, bool force);

int b43legacy_radio_selectchannel(struct b43legacy_wldev *dev, u8 channel,
				  int synthetic_pu_workaround);

void b43legacy_radio_set_txpower_a(struct b43legacy_wldev *dev, u16 txpower);
void b43legacy_radio_set_txpower_bg(struct b43legacy_wldev *dev,
				    u16 baseband_attenuation, u16 attenuation,
				    u16 txpower);

u16 b43legacy_default_baseband_attenuation(struct b43legacy_wldev *dev);
u16 b43legacy_default_radio_attenuation(struct b43legacy_wldev *dev);
u16 b43legacy_default_txctl1(struct b43legacy_wldev *dev);

void b43legacy_radio_set_txantenna(struct b43legacy_wldev *dev, u32 val);

void b43legacy_radio_clear_tssi(struct b43legacy_wldev *dev);

u8 b43legacy_radio_aci_detect(struct b43legacy_wldev *dev, u8 channel);
u8 b43legacy_radio_aci_scan(struct b43legacy_wldev *dev);

int b43legacy_radio_set_interference_mitigation(struct b43legacy_wldev *dev,
						int mode);

void b43legacy_calc_nrssi_slope(struct b43legacy_wldev *dev);
void b43legacy_calc_nrssi_threshold(struct b43legacy_wldev *dev);
s16 b43legacy_nrssi_hw_read(struct b43legacy_wldev *dev, u16 offset);
void b43legacy_nrssi_hw_write(struct b43legacy_wldev *dev, u16 offset, s16 val);
void b43legacy_nrssi_hw_update(struct b43legacy_wldev *dev, u16 val);
void b43legacy_nrssi_mem_update(struct b43legacy_wldev *dev);

void b43legacy_radio_set_tx_iq(struct b43legacy_wldev *dev);
u16 b43legacy_radio_calibrationvalue(struct b43legacy_wldev *dev);

#endif /* B43legacy_RADIO_H_ */
