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
 * arch/arm/mach-ep93xx/include/mach/entry-macro.S
 * IRQ demultiplexing for EP93xx
 *
 * Copyright (C) 2006 Lennert Buytenhek <buytenh@wantstofly.org>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or (at
 * your option) any later version.
 */
#include <mach/ep93xx-regs.h>

		.macro	disable_fiq
		.endm

		.macro  get_irqnr_preamble, base, tmp
		.endm

		.macro  arch_ret_to_user, tmp1, tmp2
		.endm

		.macro	get_irqnr_and_base, irqnr, irqstat, base, tmp
		ldr	\base, =(EP93XX_AHB_VIRT_BASE)
		orr	\base, \base, #0x000b0000
		mov	\irqnr, #0
		ldr	\irqstat, [\base]		@ lower 32 interrupts
		cmp	\irqstat, #0
		bne	1001f

		eor	\base, \base, #0x00070000
		ldr	\irqstat, [\base]		@ upper 32 interrupts
		cmp	\irqstat, #0
		beq	1002f
		mov	\irqnr, #0x20

1001:
		movs	\tmp, \irqstat, lsl #16
		movne	\irqstat, \tmp
		addeq	\irqnr, \irqnr, #16

		movs	\tmp, \irqstat, lsl #8
		movne	\irqstat, \tmp
		addeq	\irqnr, \irqnr, #8

		movs	\tmp, \irqstat, lsl #4
		movne	\irqstat, \tmp
		addeq	\irqnr, \irqnr, #4

		movs	\tmp, \irqstat, lsl #2
		movne	\irqstat, \tmp
		addeq	\irqnr, \irqnr, #2

		movs	\tmp, \irqstat, lsl #1
		addeq	\irqnr, \irqnr, #1
		orrs	\base, \base, #1

1002:
		.endm
