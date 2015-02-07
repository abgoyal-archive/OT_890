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
 * arch/arm/mach-ixp2000/include/mach/entry-macro.S
 *
 * Low-level IRQ helper macros for IXP2000-based platforms
 *
 * This file is licensed under  the terms of the GNU General Public
 * License version 2. This program is licensed "as is" without any
 * warranty of any kind, whether express or implied.
 */
#include <mach/irqs.h>

		.macro  disable_fiq
		.endm

		.macro  get_irqnr_preamble, base, tmp
		.endm

		.macro  arch_ret_to_user, tmp1, tmp2
		.endm

		.macro  get_irqnr_and_base, irqnr, irqstat, base, tmp

		mov	\irqnr, #0x0              @clear out irqnr as default
                mov	\base, #0xfe000000
		orr	\base, \base, #0x00e00000
		orr	\base, \base, #0x08
		ldr	\irqstat, [\base]         @ get interrupts

		cmp	\irqstat, #0
		beq	1001f

		clz     \irqnr, \irqstat
		mov     \base, #31
		subs    \irqnr, \base, \irqnr

		/*
		 * We handle PCIA and PCIB here so we don't have an
		 * extra layer of code just to check these two bits.
		 */
		cmp	\irqnr, #IRQ_IXP2000_PCI
		bne	1001f

		mov	\base, #0xfe000000
		orr	\base, \base, #0x00c00000
		orr	\base, \base, #0x00000100
		orr	\base, \base, #0x00000058
		ldr	\irqstat, [\base]

		mov	\tmp, #(1<<26)
		tst	\irqstat, \tmp
		movne	\irqnr, #IRQ_IXP2000_PCIA
		bne	1001f

		mov	\tmp, #(1<<27)
		tst	\irqstat, \tmp
		movne	\irqnr, #IRQ_IXP2000_PCIB

1001:
		.endm

