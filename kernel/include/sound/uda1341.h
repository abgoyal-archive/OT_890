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
 *  linux/include/linux/l3/uda1341.h
 *
 * Philips UDA1341 mixer device driver for ALSA
 *
 * Copyright (c) 2002 Tomas Kasparek <tomas.kasparek@seznam.cz>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License.
 *
 * History:
 *
 * 2002-03-13 Tomas Kasparek Initial release - based on uda1341.h from OSS
 * 2002-03-30 Tomas Kasparek Proc filesystem support, complete mixer and DSP
 *                           features support
 */

#define UDA1341_ALSA_NAME "snd-uda1341"

/*
 * Default rate set after inicialization
 */
#define AUDIO_RATE_DEFAULT	44100

/*
 * UDA1341 L3 address and command types
 */
#define UDA1341_L3ADDR		5
#define UDA1341_DATA0		(UDA1341_L3ADDR << 2 | 0)
#define UDA1341_DATA1		(UDA1341_L3ADDR << 2 | 1)
#define UDA1341_STATUS		(UDA1341_L3ADDR << 2 | 2)

enum uda1341_onoff {
	OFF=0,
	ON,
};

enum uda1341_format {
	I2S=0,
	LSB16,
	LSB18,
	LSB20,
	MSB,
	LSB16MSB,
	LSB18MSB,
	LSB20MSB,        
};

enum uda1341_fs {
	F512=0,
	F384,
	F256,
	Funused,
};

enum uda1341_peak {
	BEFORE=0,
	AFTER,
};

enum uda1341_filter {
	FLAT=0,
	MIN,
	MIN2,
	MAX,
};

enum uda1341_mixer {
	DOUBLE,
	LINE,
	MIC,
	MIXER,
};

enum uda1341_deemp {
	NONE,
	D32,
	D44,
	D48,
};

enum uda1341_config {
	CMD_READ_REG = 0,
	CMD_RESET,
	CMD_FS,
	CMD_FORMAT,
	CMD_OGAIN,
	CMD_IGAIN,
	CMD_DAC,
	CMD_ADC,
	CMD_VOLUME,
	CMD_BASS,
	CMD_TREBBLE,
	CMD_PEAK,
	CMD_DEEMP,
	CMD_MUTE,        
	CMD_FILTER,
	CMD_CH1,
	CMD_CH2,
	CMD_MIC,       
	CMD_MIXER,
	CMD_AGC,
	CMD_IG,
	CMD_AGC_TIME,
	CMD_AGC_LEVEL,
#ifdef CONFIG_PM
	CMD_SUSPEND,
	CMD_RESUME,
#endif
	CMD_LAST,
};

enum write_through {
	//used in update_bits (write_cfg) to avoid l3_write - just update local copy of regs.
	REGS_ONLY=0,
	//update local regs and write value to uda1341 - do l3_write
	FLUSH,
};

int __init snd_chip_uda1341_mixer_new(struct snd_card *card, struct l3_client **clnt);

/*
 * Local variables:
 * indent-tabs-mode: t
 * End:
 */
