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
 * arch/arm/mach-w90x900/include/mach/regs-timer.h
 *
 * Copyright (c) 2008 Nuvoton technology corporation
 * All rights reserved.
 *
 * Wan ZongShun <mcuos.com@gmail.com>
 *
 * Based on arch/arm/mach-s3c2410/include/mach/regs-timer.h
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */

#ifndef __ASM_ARCH_REGS_TIMER_H
#define __ASM_ARCH_REGS_TIMER_H

/* Timer Registers */

#define TMR_BA			W90X900_VA_TIMER
#define REG_TCSR0		(TMR_BA+0x00)
#define REG_TCSR1		(TMR_BA+0x04)
#define REG_TICR0		(TMR_BA+0x08)
#define REG_TICR1		(TMR_BA+0x0C)
#define REG_TDR0		(TMR_BA+0x10)
#define REG_TDR1		(TMR_BA+0x14)
#define REG_TISR		(TMR_BA+0x18)
#define REG_WTCR		(TMR_BA+0x1C)
#define REG_TCSR2		(TMR_BA+0x20)
#define REG_TCSR3		(TMR_BA+0x24)
#define REG_TICR2		(TMR_BA+0x28)
#define REG_TICR3		(TMR_BA+0x2C)
#define REG_TDR2		(TMR_BA+0x30)
#define REG_TDR3		(TMR_BA+0x34)
#define REG_TCSR4		(TMR_BA+0x40)
#define REG_TICR4		(TMR_BA+0x48)
#define REG_TDR4		(TMR_BA+0x50)

#endif /*  __ASM_ARCH_REGS_TIMER_H */
