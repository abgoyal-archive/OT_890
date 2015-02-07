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
 * linux/sound/arm/bf5xx-ac97.h
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#ifndef _BF5XX_AC97_H
#define _BF5XX_AC97_H

extern struct snd_ac97_bus_ops bf5xx_ac97_ops;
extern struct snd_ac97 *ac97;
/* Frame format in memory, only support stereo currently */
struct ac97_frame {
	u16 ac97_tag;		/* slot 0 */
	u16 ac97_addr;		/* slot 1 */
	u16 ac97_data;		/* slot 2 */
	u16 ac97_pcm_l;		/*slot 3:front left*/
	u16 ac97_pcm_r;		/*slot 4:front left*/
#if defined(CONFIG_SND_BF5XX_MULTICHAN_SUPPORT)
	u16 ac97_mdm_l1;
	u16 ac97_center;	/*slot 6:center*/
	u16 ac97_sl;		/*slot 7:surround left*/
	u16 ac97_sr;		/*slot 8:surround right*/
	u16 ac97_lfe;		/*slot 9:lfe*/
#endif
} __attribute__ ((packed));

/* Speaker location */
#define SP_FL		0x0001
#define SP_FR		0x0010
#define SP_FC		0x0002
#define SP_LFE		0x0020
#define SP_SL		0x0004
#define SP_SR		0x0040

#define SP_STEREO	(SP_FL | SP_FR)
#define SP_2DOT1	(SP_FL | SP_FR | SP_LFE)
#define SP_QUAD		(SP_FL | SP_FR | SP_SL | SP_SR)
#define SP_5DOT1	(SP_FL | SP_FR | SP_FC | SP_LFE | SP_SL | SP_SR)

#define TAG_VALID		0x8000
#define TAG_CMD			0x6000
#define TAG_PCM_LEFT		0x1000
#define TAG_PCM_RIGHT		0x0800
#define TAG_PCM_MDM_L1		0x0400
#define TAG_PCM_CENTER		0x0200
#define TAG_PCM_SL		0x0100
#define TAG_PCM_SR		0x0080
#define TAG_PCM_LFE		0x0040

extern struct snd_soc_dai bfin_ac97_dai;

void bf5xx_pcm_to_ac97(struct ac97_frame *dst, const __u16 *src, \
		size_t count, unsigned int chan_mask);

void bf5xx_ac97_to_pcm(const struct ac97_frame *src, __u16 *dst, \
		size_t count);

#endif
