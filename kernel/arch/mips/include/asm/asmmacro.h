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
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 *
 * Copyright (C) 2003 Ralf Baechle
 */
#ifndef _ASM_ASMMACRO_H
#define _ASM_ASMMACRO_H

#include <asm/hazards.h>

#ifdef CONFIG_32BIT
#include <asm/asmmacro-32.h>
#endif
#ifdef CONFIG_64BIT
#include <asm/asmmacro-64.h>
#endif
#ifdef CONFIG_MIPS_MT_SMTC
#include <asm/mipsmtregs.h>
#endif

#ifdef CONFIG_MIPS_MT_SMTC
	.macro	local_irq_enable reg=t0
	mfc0	\reg, CP0_TCSTATUS
	ori	\reg, \reg, TCSTATUS_IXMT
	xori	\reg, \reg, TCSTATUS_IXMT
	mtc0	\reg, CP0_TCSTATUS
	_ehb
	.endm

	.macro	local_irq_disable reg=t0
	mfc0	\reg, CP0_TCSTATUS
	ori	\reg, \reg, TCSTATUS_IXMT
	mtc0	\reg, CP0_TCSTATUS
	_ehb
	.endm
#elif defined(CONFIG_CPU_MIPSR2)
	.macro	local_irq_enable reg=t0
	ei
	irq_enable_hazard
	.endm

	.macro	local_irq_disable reg=t0
	di
	irq_disable_hazard
	.endm
#else
	.macro	local_irq_enable reg=t0
	mfc0	\reg, CP0_STATUS
	ori	\reg, \reg, 1
	mtc0	\reg, CP0_STATUS
	irq_enable_hazard
	.endm

	.macro	local_irq_disable reg=t0
	mfc0	\reg, CP0_STATUS
	ori	\reg, \reg, 1
	xori	\reg, \reg, 1
	mtc0	\reg, CP0_STATUS
	irq_disable_hazard
	.endm
#endif /* CONFIG_MIPS_MT_SMTC */

/*
 * Temporary until all gas have MT ASE support
 */
	.macro	DMT	reg=0
	.word	0x41600bc1 | (\reg << 16)
	.endm

	.macro	EMT	reg=0
	.word	0x41600be1 | (\reg << 16)
	.endm

	.macro	DVPE	reg=0
	.word	0x41600001 | (\reg << 16)
	.endm

	.macro	EVPE	reg=0
	.word	0x41600021 | (\reg << 16)
	.endm

	.macro	MFTR	rt=0, rd=0, u=0, sel=0
	 .word	0x41000000 | (\rt << 16) | (\rd << 11) | (\u << 5) | (\sel)
	.endm

	.macro	MTTR	rt=0, rd=0, u=0, sel=0
	 .word	0x41800000 | (\rt << 16) | (\rd << 11) | (\u << 5) | (\sel)
	.endm

#endif /* _ASM_ASMMACRO_H */
