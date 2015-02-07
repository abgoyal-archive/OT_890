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
 * arch/arm/mach-ks8695/include/mach/regs-irq.h
 *
 * Copyright (C) 2006 Ben Dooks <ben@simtec.co.uk>
 * Copyright (C) 2006 Simtec Electronics
 *
 * KS8695 - IRQ registers and bit definitions
 *
 * This file is licensed under  the terms of the GNU General Public
 * License version 2. This program is licensed "as is" without any
 * warranty of any kind, whether express or implied.
 */

#ifndef KS8695_IRQ_H
#define KS8695_IRQ_H

#define KS8695_IRQ_OFFSET	(0xF0000 + 0xE200)
#define KS8695_IRQ_VA		(KS8695_IO_VA + KS8695_IRQ_OFFSET)
#define KS8695_IRQ_PA		(KS8695_IO_PA + KS8695_IRQ_OFFSET)


/*
 * Interrupt Controller registers
 */
#define KS8695_INTMC		(0x00)		/* Mode Control Register */
#define KS8695_INTEN		(0x04)		/* Interrupt Enable Register */
#define KS8695_INTST		(0x08)		/* Interrupt Status Register */
#define KS8695_INTPW		(0x0c)		/* Interrupt Priority (WAN MAC) */
#define KS8695_INTPH		(0x10)		/* Interrupt Priority (HPNA) [KS8695 only] */
#define KS8695_INTPL		(0x14)		/* Interrupt Priority (LAN MAC) */
#define KS8695_INTPT		(0x18)		/* Interrupt Priority (Timer) */
#define KS8695_INTPU		(0x1c)		/* Interrupt Priority (UART) */
#define KS8695_INTPE		(0x20)		/* Interrupt Priority (External Interrupt) */
#define KS8695_INTPC		(0x24)		/* Interrupt Priority (Communications Channel) */
#define KS8695_INTPBE		(0x28)		/* Interrupt Priority (Bus Error Response) */
#define KS8695_INTMS		(0x2c)		/* Interrupt Mask Status Register */
#define KS8695_INTHPF		(0x30)		/* Interrupt Pending Highest Priority (FIQ) */
#define KS8695_INTHPI		(0x34)		/* Interrupt Pending Highest Priority (IRQ) */


#endif
