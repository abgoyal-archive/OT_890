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
 * Cobalt IRQ definitions.
 *
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 *
 * Copyright (C) 1997 Cobalt Microserver
 * Copyright (C) 1997, 2003 Ralf Baechle
 * Copyright (C) 2001-2003 Liam Davies (ldavies@agile.tv)
 * Copyright (C) 2007 Yoichi Yuasa <yoichi_yuasa@tripeaks.co.jp>
 */
#ifndef _ASM_COBALT_IRQ_H
#define _ASM_COBALT_IRQ_H

/*
 * i8259 interrupts used on Cobalt:
 *
 *	8  - RTC
 *	9  - PCI slot
 *	14 - IDE0
 *	15 - IDE1(no connector on board)
 */
#define I8259A_IRQ_BASE			0

#define PCISLOT_IRQ			(I8259A_IRQ_BASE + 9)

/*
 * CPU interrupts used on Cobalt:
 *
 *	0 - Software interrupt 0 (unused)
 *	1 - Software interrupt 0 (unused)
 *	2 - cascade GT64111
 *	3 - ethernet or SCSI host controller
 *	4 - ethernet
 *	5 - 16550 UART
 *	6 - cascade i8259
 *	7 - CP0 counter
 */
#define MIPS_CPU_IRQ_BASE		16

#define GT641XX_CASCADE_IRQ		(MIPS_CPU_IRQ_BASE + 2)
#define RAQ2_SCSI_IRQ			(MIPS_CPU_IRQ_BASE + 3)
#define ETH0_IRQ			(MIPS_CPU_IRQ_BASE + 3)
#define QUBE1_ETH0_IRQ			(MIPS_CPU_IRQ_BASE + 4)
#define ETH1_IRQ			(MIPS_CPU_IRQ_BASE + 4)
#define SERIAL_IRQ			(MIPS_CPU_IRQ_BASE + 5)
#define SCSI_IRQ			(MIPS_CPU_IRQ_BASE + 5)
#define I8259_CASCADE_IRQ		(MIPS_CPU_IRQ_BASE + 6)

#define GT641XX_IRQ_BASE		24

#include <asm/irq_gt641xx.h>

#define NR_IRQS					(GT641XX_PCI_INT3_IRQ + 1)

#endif /* _ASM_COBALT_IRQ_H */
