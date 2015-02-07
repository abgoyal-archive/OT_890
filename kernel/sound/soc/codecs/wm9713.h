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
 * wm9713.h  --  WM9713 Soc Audio driver
 */

#ifndef _WM9713_H
#define _WM9713_H

/* clock inputs */
#define WM9713_CLKA_PIN			0
#define WM9713_CLKB_PIN			1

/* clock divider ID's */
#define WM9713_PCMCLK_DIV		0
#define WM9713_CLKA_MULT		1
#define WM9713_CLKB_MULT		2
#define WM9713_HIFI_DIV			3
#define WM9713_PCMBCLK_DIV		4
#define WM9713_PCMCLK_PLL_DIV           5
#define WM9713_HIFI_PLL_DIV             6

/* Calculate the appropriate bit mask for the external PCM clock divider */
#define WM9713_PCMDIV(x)	((x - 1) << 8)

/* Calculate the appropriate bit mask for the external HiFi clock divider */
#define WM9713_HIFIDIV(x)	((x - 1) << 12)

/* MCLK clock mulitipliers */
#define WM9713_CLKA_X1		(0 << 1)
#define WM9713_CLKA_X2		(1 << 1)
#define WM9713_CLKB_X1		(0 << 2)
#define WM9713_CLKB_X2		(1 << 2)

/* MCLK clock MUX */
#define WM9713_CLK_MUX_A		(0 << 0)
#define WM9713_CLK_MUX_B		(1 << 0)

/* Voice DAI BCLK divider */
#define WM9713_PCMBCLK_DIV_1	(0 << 9)
#define WM9713_PCMBCLK_DIV_2	(1 << 9)
#define WM9713_PCMBCLK_DIV_4	(2 << 9)
#define WM9713_PCMBCLK_DIV_8	(3 << 9)
#define WM9713_PCMBCLK_DIV_16	(4 << 9)

#define WM9713_DAI_AC97_HIFI	0
#define WM9713_DAI_AC97_AUX		1
#define WM9713_DAI_PCM_VOICE	2

extern struct snd_soc_codec_device soc_codec_dev_wm9713;
extern struct snd_soc_dai wm9713_dai[3];

int wm9713_reset(struct snd_soc_codec *codec,  int try_warm);

#endif
