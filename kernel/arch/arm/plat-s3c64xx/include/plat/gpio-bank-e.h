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

/* linux/arch/arm/plat-s3c64xx/include/plat/gpio-bank-e.h
 *
 * Copyright 2008 Openmoko, Inc.
 * Copyright 2008 Simtec Electronics
 * 	Ben Dooks <ben@simtec.co.uk>
 * 	http://armlinux.simtec.co.uk/
 *
 * GPIO Bank E register and configuration definitions
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
*/

#define S3C64XX_GPECON			(S3C64XX_GPE_BASE + 0x00)
#define S3C64XX_GPEDAT			(S3C64XX_GPE_BASE + 0x04)
#define S3C64XX_GPEPUD			(S3C64XX_GPE_BASE + 0x08)
#define S3C64XX_GPECONSLP		(S3C64XX_GPE_BASE + 0x0c)
#define S3C64XX_GPEPUDSLP		(S3C64XX_GPE_BASE + 0x10)

#define S3C64XX_GPE_CONMASK(__gpio)	(0xf << ((__gpio) * 4))
#define S3C64XX_GPE_INPUT(__gpio)	(0x0 << ((__gpio) * 4))
#define S3C64XX_GPE_OUTPUT(__gpio)	(0x1 << ((__gpio) * 4))

#define S3C64XX_GPE0_PCM1_SCLK		(0x02 << 0)
#define S3C64XX_GPE0_I2S1_CLK		(0x03 << 0)
#define S3C64XX_GPE0_AC97_BITCLK	(0x04 << 0)

#define S3C64XX_GPE1_PCM1_EXTCLK	(0x02 << 4)
#define S3C64XX_GPE1_I2S1_CDCLK		(0x03 << 4)
#define S3C64XX_GPE1_AC97_nRESET	(0x04 << 4)

#define S3C64XX_GPE2_PCM1_FSYNC		(0x02 << 8)
#define S3C64XX_GPE2_I2S1_LRCLK		(0x03 << 8)
#define S3C64XX_GPE2_AC97_SYNC		(0x04 << 8)

#define S3C64XX_GPE3_PCM1_SIN		(0x02 << 12)
#define S3C64XX_GPE3_I2S1_DI		(0x03 << 12)
#define S3C64XX_GPE3_AC97_SDI		(0x04 << 12)

#define S3C64XX_GPE4_PCM1_SOUT		(0x02 << 16)
#define S3C64XX_GPE4_I2S1_D0		(0x03 << 16)
#define S3C64XX_GPE4_AC97_SDO		(0x04 << 16)

