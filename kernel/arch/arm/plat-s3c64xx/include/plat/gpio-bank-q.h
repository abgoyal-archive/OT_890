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

/* linux/arch/arm/plat-s3c64xx/include/plat/gpio-bank-q.h
 *
 * Copyright 2008 Openmoko, Inc.
 * Copyright 2008 Simtec Electronics
 * 	Ben Dooks <ben@simtec.co.uk>
 * 	http://armlinux.simtec.co.uk/
 *
 * GPIO Bank Q register and configuration definitions
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
*/

#define S3C64XX_GPQCON			(S3C64XX_GPQ_BASE + 0x00)
#define S3C64XX_GPQDAT			(S3C64XX_GPQ_BASE + 0x04)
#define S3C64XX_GPQPUD			(S3C64XX_GPQ_BASE + 0x08)
#define S3C64XX_GPQCONSLP		(S3C64XX_GPQ_BASE + 0x0c)
#define S3C64XX_GPQPUDSLP		(S3C64XX_GPQ_BASE + 0x10)

#define S3C64XX_GPQ_CONMASK(__gpio)	(0x3 << ((__gpio) * 2))
#define S3C64XX_GPQ_INPUT(__gpio)	(0x0 << ((__gpio) * 2))
#define S3C64XX_GPQ_OUTPUT(__gpio)	(0x1 << ((__gpio) * 2))

#define S3C64XX_GPQ0_MEM0_ADDR18_RAS	(0x02 << 0)
#define S3C64XX_GPQ0_EINT_G9_0		(0x03 << 0)

#define S3C64XX_GPQ1_MEM0_ADDR19_CAS	(0x02 << 2)
#define S3C64XX_GPQ1_EINT_G9_1		(0x03 << 2)

#define S3C64XX_GPQ2_EINT_G9_2		(0x03 << 4)

#define S3C64XX_GPQ3_EINT_G9_3		(0x03 << 6)

#define S3C64XX_GPQ4_EINT_G9_4		(0x03 << 8)

#define S3C64XX_GPQ5_EINT_G9_5		(0x03 << 10)

#define S3C64XX_GPQ6_EINT_G9_6		(0x03 << 12)

#define S3C64XX_GPQ7_MEM0_ADDR17_WENDMC	(0x02 << 14)
#define S3C64XX_GPQ7_EINT_G9_7		(0x03 << 14)

#define S3C64XX_GPQ8_MEM0_ADDR16_APDMC	(0x02 << 16)
#define S3C64XX_GPQ8_EINT_G9_8		(0x03 << 16)

