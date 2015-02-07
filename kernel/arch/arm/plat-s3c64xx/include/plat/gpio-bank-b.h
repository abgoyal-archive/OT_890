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

/* linux/arch/arm/plat-s3c64xx/include/plat/gpio-bank-b.h
 *
 * Copyright 2008 Openmoko, Inc.
 * Copyright 2008 Simtec Electronics
 * 	Ben Dooks <ben@simtec.co.uk>
 * 	http://armlinux.simtec.co.uk/
 *
 * GPIO Bank B register and configuration definitions
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
*/

#define S3C64XX_GPBCON			(S3C64XX_GPB_BASE + 0x00)
#define S3C64XX_GPBDAT			(S3C64XX_GPB_BASE + 0x04)
#define S3C64XX_GPBPUD			(S3C64XX_GPB_BASE + 0x08)
#define S3C64XX_GPBCONSLP		(S3C64XX_GPB_BASE + 0x0c)
#define S3C64XX_GPBPUDSLP		(S3C64XX_GPB_BASE + 0x10)

#define S3C64XX_GPB_CONMASK(__gpio)	(0xf << ((__gpio) * 4))
#define S3C64XX_GPB_INPUT(__gpio)	(0x0 << ((__gpio) * 4))
#define S3C64XX_GPB_OUTPUT(__gpio)	(0x1 << ((__gpio) * 4))

#define S3C64XX_GPB0_UART_RXD2		(0x02 << 0)
#define S3C64XX_GPB0_EXTDMA_REQ		(0x03 << 0)
#define S3C64XX_GPB0_IrDA_RXD		(0x04 << 0)
#define S3C64XX_GPB0_ADDR_CF0		(0x05 << 0)
#define S3C64XX_GPB0_EINT_G1_8		(0x07 << 0)

#define S3C64XX_GPB1_UART_TXD2		(0x02 << 4)
#define S3C64XX_GPB1_EXTDMA_ACK		(0x03 << 4)
#define S3C64XX_GPB1_IrDA_TXD		(0x04 << 4)
#define S3C64XX_GPB1_ADDR_CF1		(0x05 << 4)
#define S3C64XX_GPB1_EINT_G1_9		(0x07 << 4)

#define S3C64XX_GPB2_UART_RXD3		(0x02 << 8)
#define S3C64XX_GPB2_IrDA_RXD		(0x03 << 8)
#define S3C64XX_GPB2_EXTDMA_REQ		(0x04 << 8)
#define S3C64XX_GPB2_ADDR_CF2		(0x05 << 8)
#define S3C64XX_GPB2_I2C_SCL1		(0x06 << 8)
#define S3C64XX_GPB2_EINT_G1_10		(0x07 << 8)

#define S3C64XX_GPB3_UART_TXD3		(0x02 << 12)
#define S3C64XX_GPB3_IrDA_TXD		(0x03 << 12)
#define S3C64XX_GPB3_EXTDMA_ACK		(0x04 << 12)
#define S3C64XX_GPB3_I2C_SDA1		(0x06 << 12)
#define S3C64XX_GPB3_EINT_G1_11		(0x07 << 12)

#define S3C64XX_GPB4_IrDA_SDBW		(0x02 << 16)
#define S3C64XX_GPB4_CAM_FIELD		(0x03 << 16)
#define S3C64XX_GPB4_CF_DATA_DIR	(0x04 << 16)
#define S3C64XX_GPB4_EINT_G1_12		(0x07 << 16)

#define S3C64XX_GPB5_I2C_SCL0		(0x02 << 20)
#define S3C64XX_GPB5_EINT_G1_13		(0x07 << 20)

#define S3C64XX_GPB6_I2C_SDA0		(0x02 << 24)
#define S3C64XX_GPB6_EINT_G1_14		(0x07 << 24)

