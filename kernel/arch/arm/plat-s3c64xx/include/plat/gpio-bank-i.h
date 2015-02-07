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

/* linux/arch/arm/plat-s3c64xx/include/plat/gpio-bank-i.h
 *
 * Copyright 2008 Openmoko, Inc.
 * Copyright 2008 Simtec Electronics
 * 	Ben Dooks <ben@simtec.co.uk>
 * 	http://armlinux.simtec.co.uk/
 *
 * GPIO Bank I register and configuration definitions
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
*/

#define S3C64XX_GPICON			(S3C64XX_GPI_BASE + 0x00)
#define S3C64XX_GPIDAT			(S3C64XX_GPI_BASE + 0x04)
#define S3C64XX_GPIPUD			(S3C64XX_GPI_BASE + 0x08)
#define S3C64XX_GPICONSLP		(S3C64XX_GPI_BASE + 0x0c)
#define S3C64XX_GPIPUDSLP		(S3C64XX_GPI_BASE + 0x10)

#define S3C64XX_GPI_CONMASK(__gpio)	(0x3 << ((__gpio) * 2))
#define S3C64XX_GPI_INPUT(__gpio)	(0x0 << ((__gpio) * 2))
#define S3C64XX_GPI_OUTPUT(__gpio)	(0x1 << ((__gpio) * 2))

#define S3C64XX_GPI0_VD0		(0x02 << 0)
#define S3C64XX_GPI1_VD1		(0x02 << 2)
#define S3C64XX_GPI2_VD2		(0x02 << 4)
#define S3C64XX_GPI3_VD3		(0x02 << 6)
#define S3C64XX_GPI4_VD4		(0x02 << 8)
#define S3C64XX_GPI5_VD5		(0x02 << 10)
#define S3C64XX_GPI6_VD6		(0x02 << 12)
#define S3C64XX_GPI7_VD7		(0x02 << 14)
#define S3C64XX_GPI8_VD8		(0x02 << 16)
#define S3C64XX_GPI9_VD9		(0x02 << 18)
#define S3C64XX_GPI10_VD10		(0x02 << 20)
#define S3C64XX_GPI11_VD11		(0x02 << 22)
#define S3C64XX_GPI12_VD12		(0x02 << 24)
#define S3C64XX_GPI13_VD13		(0x02 << 26)
#define S3C64XX_GPI14_VD14		(0x02 << 28)
#define S3C64XX_GPI15_VD15		(0x02 << 30)
