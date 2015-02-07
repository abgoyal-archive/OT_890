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
 * Au12x0/Au1550 PSC ALSA ASoC audio support.
 *
 * (c) 2007-2008 MSC Vertriebsges.m.b.H.,
 *	Manuel Lauss <mano@roarinelk.homelinux.net>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * NOTE: all of these drivers can only work with a SINGLE instance
 *	 of a PSC. Multiple independent audio devices are impossible
 *	 with ASoC v1.
 */

#ifndef _AU1X_PCM_H
#define _AU1X_PCM_H

extern struct snd_soc_dai au1xpsc_ac97_dai;
extern struct snd_soc_dai au1xpsc_i2s_dai;
extern struct snd_soc_platform au1xpsc_soc_platform;
extern struct snd_ac97_bus_ops soc_ac97_ops;

struct au1xpsc_audio_data {
	void __iomem *mmio;

	unsigned long cfg;
	unsigned long rate;

	unsigned long pm[2];
	struct resource *ioarea;
};

#define PCM_TX	0
#define PCM_RX	1

#define SUBSTREAM_TYPE(substream) \
	((substream)->stream == SNDRV_PCM_STREAM_PLAYBACK ? PCM_TX : PCM_RX)

/* easy access macros */
#define PSC_CTRL(x)	((unsigned long)((x)->mmio) + PSC_CTRL_OFFSET)
#define PSC_SEL(x)	((unsigned long)((x)->mmio) + PSC_SEL_OFFSET)
#define I2S_STAT(x)	((unsigned long)((x)->mmio) + PSC_I2SSTAT_OFFSET)
#define I2S_CFG(x)	((unsigned long)((x)->mmio) + PSC_I2SCFG_OFFSET)
#define I2S_PCR(x)	((unsigned long)((x)->mmio) + PSC_I2SPCR_OFFSET)
#define AC97_CFG(x)	((unsigned long)((x)->mmio) + PSC_AC97CFG_OFFSET)
#define AC97_CDC(x)	((unsigned long)((x)->mmio) + PSC_AC97CDC_OFFSET)
#define AC97_EVNT(x)	((unsigned long)((x)->mmio) + PSC_AC97EVNT_OFFSET)
#define AC97_PCR(x)	((unsigned long)((x)->mmio) + PSC_AC97PCR_OFFSET)
#define AC97_RST(x)	((unsigned long)((x)->mmio) + PSC_AC97RST_OFFSET)
#define AC97_STAT(x)	((unsigned long)((x)->mmio) + PSC_AC97STAT_OFFSET)

#endif
