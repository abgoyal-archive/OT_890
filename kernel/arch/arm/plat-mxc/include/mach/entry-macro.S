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
 *  Copyright (C) 2007 Lennert Buytenhek <buytenh@wantstofly.org>
 *  Copyright 2004-2007 Freescale Semiconductor, Inc. All Rights Reserved.
 */

/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#include <mach/hardware.h>

#define AVIC_NIMASK	0x04

	@ this macro disables fast irq (not implemented)
	.macro	disable_fiq
	.endm

	.macro  get_irqnr_preamble, base, tmp
	ldr	\base, =AVIC_IO_ADDRESS(AVIC_BASE_ADDR)
#ifdef CONFIG_MXC_IRQ_PRIOR
	ldr	r4, [\base, #AVIC_NIMASK]
#endif
	.endm

	.macro  arch_ret_to_user, tmp1, tmp2
	.endm

	@ this macro checks which interrupt occured
	@ and returns its number in irqnr
	@ and returns if an interrupt occured in irqstat
	.macro	get_irqnr_and_base, irqnr, irqstat, base, tmp
	@ Load offset & priority of the highest priority
	@ interrupt pending from AVIC_NIVECSR
	ldr	\irqstat, [\base, #0x40]
	@ Shift to get the decoded IRQ number, using ASR so
	@ 'no interrupt pending' becomes 0xffffffff
	mov	\irqnr, \irqstat, asr #16
	@ set zero flag if IRQ + 1 == 0
	adds	\tmp, \irqnr, #1
#ifdef CONFIG_MXC_IRQ_PRIOR
	bicne	\tmp, \irqstat, #0xFFFFFFE0
	strne	\tmp, [\base, #AVIC_NIMASK]
	streq	r4, [\base, #AVIC_NIMASK]
#endif
	.endm

	@ irq priority table (not used)
	.macro	irq_prio_table
	.endm
