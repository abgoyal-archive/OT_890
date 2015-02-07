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
 * wm8971.h  --  audio driver for WM8971
 *
 * Copyright 2005 Lab126, Inc.
 *
 * Author: Kenneth Kiraly <kiraly@lab126.com>
 *
 * This program is free software; you can redistribute  it and/or modify it
 * under  the terms of  the GNU General  Public License as published by the
 * Free Software Foundation;  either version 2 of the  License, or (at your
 * option) any later version.
 *
 */

#ifndef _WM8971_H
#define _WM8971_H

#define WM8971_LINVOL	0x00
#define WM8971_RINVOL	0x01
#define WM8971_LOUT1V	0x02
#define WM8971_ROUT1V	0x03
#define WM8971_ADCDAC	0x05
#define WM8971_IFACE	0x07
#define WM8971_SRATE	0x08
#define WM8971_LDAC		0x0a
#define WM8971_RDAC		0x0b
#define WM8971_BASS		0x0c
#define WM8971_TREBLE	0x0d
#define WM8971_RESET	0x0f
#define WM8971_ALC1		0x11
#define	WM8971_ALC2		0x12
#define	WM8971_ALC3		0x13
#define WM8971_NGATE	0x14
#define WM8971_LADC		0x15
#define WM8971_RADC		0x16
#define	WM8971_ADCTL1	0x17
#define	WM8971_ADCTL2	0x18
#define WM8971_PWR1		0x19
#define WM8971_PWR2		0x1a
#define	WM8971_ADCTL3	0x1b
#define WM8971_ADCIN	0x1f
#define	WM8971_LADCIN	0x20
#define	WM8971_RADCIN	0x21
#define WM8971_LOUTM1	0x22
#define WM8971_LOUTM2	0x23
#define WM8971_ROUTM1	0x24
#define WM8971_ROUTM2	0x25
#define WM8971_MOUTM1	0x26
#define WM8971_MOUTM2	0x27
#define WM8971_LOUT2V	0x28
#define WM8971_ROUT2V	0x29
#define WM8971_MOUTV	0x2A

#define WM8971_SYSCLK	0

struct wm8971_setup_data {
	int i2c_bus;
	unsigned short i2c_address;
};

extern struct snd_soc_dai wm8971_dai;
extern struct snd_soc_codec_device soc_codec_dev_wm8971;

#endif
