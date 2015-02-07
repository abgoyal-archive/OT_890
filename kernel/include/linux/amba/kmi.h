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
 *  linux/include/asm-arm/hardware/amba_kmi.h
 *
 *  Internal header file for AMBA KMI ports
 *
 *  Copyright (C) 2000 Deep Blue Solutions Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 *
 * ---------------------------------------------------------------------------
 *  From ARM PrimeCell(tm) PS2 Keyboard/Mouse Interface (PL050) Technical
 *  Reference Manual - ARM DDI 0143B - see http://www.arm.com/
 * ---------------------------------------------------------------------------
 */
#ifndef ASM_ARM_HARDWARE_AMBA_KMI_H
#define ASM_ARM_HARDWARE_AMBA_KMI_H

/*
 * KMI control register:
 *  KMICR_TYPE       0 = PS2/AT mode, 1 = No line control bit mode
 *  KMICR_RXINTREN   1 = enable RX interrupts
 *  KMICR_TXINTREN   1 = enable TX interrupts
 *  KMICR_EN         1 = enable KMI
 *  KMICR_FD         1 = force KMI data low
 *  KMICR_FC         1 = force KMI clock low
 */
#define KMICR		(KMI_BASE + 0x00)
#define KMICR_TYPE		(1 << 5)
#define KMICR_RXINTREN		(1 << 4)
#define KMICR_TXINTREN		(1 << 3)
#define KMICR_EN		(1 << 2)
#define KMICR_FD		(1 << 1)
#define KMICR_FC		(1 << 0)

/*
 * KMI status register:
 *  KMISTAT_TXEMPTY  1 = transmitter register empty
 *  KMISTAT_TXBUSY   1 = currently sending data
 *  KMISTAT_RXFULL   1 = receiver register ready to be read
 *  KMISTAT_RXBUSY   1 = currently receiving data
 *  KMISTAT_RXPARITY parity of last databyte received
 *  KMISTAT_IC       current level of KMI clock input
 *  KMISTAT_ID       current level of KMI data input
 */
#define KMISTAT		(KMI_BASE + 0x04)
#define KMISTAT_TXEMPTY		(1 << 6)
#define KMISTAT_TXBUSY		(1 << 5)
#define KMISTAT_RXFULL		(1 << 4)
#define KMISTAT_RXBUSY		(1 << 3)
#define KMISTAT_RXPARITY	(1 << 2)
#define KMISTAT_IC		(1 << 1)
#define KMISTAT_ID		(1 << 0)

/*
 * KMI data register
 */
#define KMIDATA		(KMI_BASE + 0x08)

/*
 * KMI clock divisor: to generate 8MHz internal clock
 *  div = (ref / 8MHz) - 1; 0 <= div <= 15
 */
#define KMICLKDIV	(KMI_BASE + 0x0c)

/*
 * KMI interrupt register:
 *  KMIIR_TXINTR     1 = transmit interrupt asserted
 *  KMIIR_RXINTR     1 = receive interrupt asserted
 */
#define KMIIR		(KMI_BASE + 0x10)
#define KMIIR_TXINTR		(1 << 1)
#define KMIIR_RXINTR		(1 << 0)

/*
 * The size of the KMI primecell
 */
#define KMI_SIZE	(0x100)

#endif
