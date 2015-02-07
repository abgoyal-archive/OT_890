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
 * arch/arm/mach-w90x900/include/mach/regs-irq.h
 *
 * Copyright (c) 2008 Nuvoton technology corporation
 * All rights reserved.
 *
 * Wan ZongShun <mcuos.com@gmail.com>
 *
 * Based on arch/arm/mach-s3c2410/include/mach/regs-irq.h
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */

#ifndef ___ASM_ARCH_REGS_IRQ_H
#define ___ASM_ARCH_REGS_IRQ_H

/* Advance Interrupt Controller (AIC) Registers */

#define AIC_BA    		W90X900_VA_IRQ

#define REG_AIC_IRQSC		(AIC_BA+0x80)
#define REG_AIC_GEN		(AIC_BA+0x84)
#define REG_AIC_GASR		(AIC_BA+0x88)
#define REG_AIC_GSCR		(AIC_BA+0x8C)
#define REG_AIC_IRSR		(AIC_BA+0x100)
#define REG_AIC_IASR		(AIC_BA+0x104)
#define REG_AIC_ISR		(AIC_BA+0x108)
#define REG_AIC_IPER		(AIC_BA+0x10C)
#define REG_AIC_ISNR		(AIC_BA+0x110)
#define REG_AIC_IMR		(AIC_BA+0x114)
#define REG_AIC_OISR		(AIC_BA+0x118)
#define REG_AIC_MECR		(AIC_BA+0x120)
#define REG_AIC_MDCR		(AIC_BA+0x124)
#define REG_AIC_SSCR		(AIC_BA+0x128)
#define REG_AIC_SCCR		(AIC_BA+0x12C)
#define REG_AIC_EOSCR		(AIC_BA+0x130)
#define AIC_IPER		(0x10C)
#define AIC_ISNR		(0x110)

/*16-18 bits of REG_AIC_GEN define irq(2-4) group*/

#define TIMER2_IRQ		(1 << 16)
#define TIMER3_IRQ		(1 << 17)
#define TIMER4_IRQ		(1 << 18)
#define TIME_GROUP_IRQ		(TIMER2_IRQ|TIMER3_IRQ|TIMER4_IRQ)

#endif /* ___ASM_ARCH_REGS_IRQ_H */
