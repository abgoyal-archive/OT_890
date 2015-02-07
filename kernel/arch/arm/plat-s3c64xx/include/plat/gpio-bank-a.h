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

/* linux/arch/arm/plat-s3c64xx/include/plat/gpio-bank-a.h
 *
 * Copyright 2008 Openmoko, Inc.
 * Copyright 2008 Simtec Electronics
 * 	Ben Dooks <ben@simtec.co.uk>
 * 	http://armlinux.simtec.co.uk/
 *
 * GPIO Bank A register and configuration definitions
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
*/

#define S3C64XX_GPACON			(S3C64XX_GPA_BASE + 0x00)
#define S3C64XX_GPADAT			(S3C64XX_GPA_BASE + 0x04)
#define S3C64XX_GPAPUD			(S3C64XX_GPA_BASE + 0x08)
#define S3C64XX_GPACONSLP		(S3C64XX_GPA_BASE + 0x0c)
#define S3C64XX_GPAPUDSLP		(S3C64XX_GPA_BASE + 0x10)

#define S3C64XX_GPA_CONMASK(__gpio)	(0xf << ((__gpio) * 4))
#define S3C64XX_GPA_INPUT(__gpio)	(0x0 << ((__gpio) * 4))
#define S3C64XX_GPA_OUTPUT(__gpio)	(0x1 << ((__gpio) * 4))

#define S3C64XX_GPA0_UART_RXD0		(0x02 << 0)
#define S3C64XX_GPA0_EINT_G1_0		(0x07 << 0)

#define S3C64XX_GPA1_UART_TXD0		(0x02 << 4)
#define S3C64XX_GPA1_EINT_G1_1		(0x07 << 4)

#define S3C64XX_GPA2_UART_nCTS0		(0x02 << 8)
#define S3C64XX_GPA2_EINT_G1_2		(0x07 << 8)

#define S3C64XX_GPA3_UART_nRTS0		(0x02 << 12)
#define S3C64XX_GPA3_EINT_G1_3		(0x07 << 12)

#define S3C64XX_GPA4_UART_RXD1		(0x02 << 16)
#define S3C64XX_GPA4_EINT_G1_4		(0x07 << 16)

#define S3C64XX_GPA5_UART_TXD1		(0x02 << 20)
#define S3C64XX_GPA5_EINT_G1_5		(0x07 << 20)

#define S3C64XX_GPA6_UART_nCTS1		(0x02 << 24)
#define S3C64XX_GPA6_EINT_G1_6		(0x07 << 24)

#define S3C64XX_GPA7_UART_nRTS1		(0x02 << 28)
#define S3C64XX_GPA7_EINT_G1_7		(0x07 << 28)

