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
 * arch/arm/mach-ks8695/include/mach/regs-gpio.h
 *
 * Copyright (C) 2007 Andrew Victor
 *
 * KS8695 - GPIO control registers and bit definitions.
 *
 * This file is licensed under  the terms of the GNU General Public
 * License version 2. This program is licensed "as is" without any
 * warranty of any kind, whether express or implied.
 */

#ifndef KS8695_GPIO_H
#define KS8695_GPIO_H

#define KS8695_GPIO_OFFSET	(0xF0000 + 0xE600)
#define KS8695_GPIO_VA		(KS8695_IO_VA + KS8695_GPIO_OFFSET)
#define KS8695_GPIO_PA		(KS8695_IO_PA + KS8695_GPIO_OFFSET)


#define KS8695_IOPM		(0x00)		/* I/O Port Mode Register */
#define KS8695_IOPC		(0x04)		/* I/O Port Control Register */
#define KS8695_IOPD		(0x08)		/* I/O Port Data Register */


/* Port Mode Register */
#define IOPM(x)			(1 << (x))	/* Mode for GPIO Pin x */

/* Port Control Register */
#define IOPC_IOTIM1EN		(1 << 17)	/* GPIO Pin for Timer1 Enable */
#define IOPC_IOTIM0EN		(1 << 16)	/* GPIO Pin for Timer0 Enable */
#define IOPC_IOEINT3EN		(1 << 15)	/* GPIO Pin for External/Soft Interrupt 3 Enable */
#define IOPC_IOEINT3TM		(7 << 12)	/* GPIO Pin for External/Soft Interrupt 3 Trigger Mode */
#define IOPC_IOEINT3_MODE(x)	((x) << 12)
#define IOPC_IOEINT2EN		(1 << 11)	/* GPIO Pin for External/Soft Interrupt 2 Enable */
#define IOPC_IOEINT2TM		(7 << 8)	/* GPIO Pin for External/Soft Interrupt 2 Trigger Mode */
#define IOPC_IOEINT2_MODE(x)	((x) << 8)
#define IOPC_IOEINT1EN		(1 << 7)	/* GPIO Pin for External/Soft Interrupt 1 Enable */
#define IOPC_IOEINT1TM		(7 << 4)	/* GPIO Pin for External/Soft Interrupt 1 Trigger Mode */
#define IOPC_IOEINT1_MODE(x)	((x) << 4)
#define IOPC_IOEINT0EN		(1 << 3)	/* GPIO Pin for External/Soft Interrupt 0 Enable */
#define IOPC_IOEINT0TM		(7 << 0)	/* GPIO Pin for External/Soft Interrupt 0 Trigger Mode */
#define IOPC_IOEINT0_MODE(x)	((x) << 0)

 /* Trigger Modes */
#define IOPC_TM_LOW		(0)		/* Level Detection (Active Low) */
#define IOPC_TM_HIGH		(1)		/* Level Detection (Active High) */
#define IOPC_TM_RISING		(2)		/* Rising Edge Detection */
#define IOPC_TM_FALLING		(4)		/* Falling Edge Detection */
#define IOPC_TM_EDGE		(6)		/* Both Edge Detection */

/* Port Data Register */
#define IOPD(x)			(1 << (x))	/* Signal Level of GPIO Pin x */

#endif
