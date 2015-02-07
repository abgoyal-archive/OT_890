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
 * arch/arm/mach-clps711x/include/mach/entry-macro.S
 *
 * Low-level IRQ helper macros for CLPS711X-based platforms
 *
 * This file is licensed under  the terms of the GNU General Public
 * License version 2. This program is licensed "as is" without any
 * warranty of any kind, whether express or implied.
 */
#include <mach/hardware.h>
#include <asm/hardware/clps7111.h>

		.macro	disable_fiq
		.endm

		.macro	get_irqnr_preamble, base, tmp
		.endm

		.macro	arch_ret_to_user, tmp1, tmp2
		.endm

#if (INTSR2 - INTSR1) != (INTMR2 - INTMR1)
#error INTSR stride != INTMR stride
#endif

		.macro	get_irqnr_and_base, irqnr, stat, base, mask
		mov	\base, #CLPS7111_BASE
		ldr	\stat, [\base, #INTSR1]
		ldr	\mask, [\base, #INTMR1]
		mov	\irqnr, #4
		mov	\mask, \mask, lsl #16
		and	\stat, \stat, \mask, lsr #16
		movs	\stat, \stat, lsr #4
		bne	1001f

		add	\base, \base, #INTSR2 - INTSR1
		ldr	\stat, [\base, #INTSR1]
		ldr	\mask, [\base, #INTMR1]
		mov	\irqnr, #16
		mov	\mask, \mask, lsl #16
		and	\stat, \stat, \mask, lsr #16

1001:		tst	\stat, #255
		addeq	\irqnr, \irqnr, #8
		moveq	\stat, \stat, lsr #8
		tst	\stat, #15
		addeq	\irqnr, \irqnr, #4
		moveq	\stat, \stat, lsr #4
		tst	\stat, #3
		addeq	\irqnr, \irqnr, #2
		moveq	\stat, \stat, lsr #2
		tst	\stat, #1
		addeq	\irqnr, \irqnr, #1
		moveq	\stat, \stat, lsr #1
		tst	\stat, #1			@ bit 0 should be set
		.endm


