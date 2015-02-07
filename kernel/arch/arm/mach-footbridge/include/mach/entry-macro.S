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
 * arch/arm/mach-footbridge/include/mach/entry-macro.S
 *
 * Low-level IRQ helper macros for footbridge-based platforms
 *
 * This file is licensed under  the terms of the GNU General Public
 * License version 2. This program is licensed "as is" without any
 * warranty of any kind, whether express or implied.
 */
#include <mach/hardware.h>
#include <mach/irqs.h>
#include <asm/hardware/dec21285.h>

		.equ	dc21285_high, ARMCSR_BASE & 0xff000000
		.equ	dc21285_low, ARMCSR_BASE & 0x00ffffff

		.macro	disable_fiq
		.endm

		.macro  get_irqnr_preamble, base, tmp
		mov	\base, #dc21285_high
		.if	dc21285_low
		orr	\base, \base, #dc21285_low
		.endif
		.endm

		.macro  arch_ret_to_user, tmp1, tmp2
		.endm

		.macro	get_irqnr_and_base, irqnr, irqstat, base, tmp
		ldr	\irqstat, [\base, #0x180]	@ get interrupts

		mov	\irqnr, #IRQ_SDRAMPARITY
		tst	\irqstat, #IRQ_MASK_SDRAMPARITY
		bne	1001f

		tst	\irqstat, #IRQ_MASK_UART_RX
		movne	\irqnr, #IRQ_CONRX
		bne	1001f

		tst	\irqstat, #IRQ_MASK_DMA1
		movne	\irqnr, #IRQ_DMA1
		bne	1001f

		tst	\irqstat, #IRQ_MASK_DMA2
		movne	\irqnr, #IRQ_DMA2
		bne	1001f

		tst	\irqstat, #IRQ_MASK_IN0
		movne	\irqnr, #IRQ_IN0
		bne	1001f

		tst	\irqstat, #IRQ_MASK_IN1
		movne	\irqnr, #IRQ_IN1
		bne	1001f

		tst	\irqstat, #IRQ_MASK_IN2
		movne	\irqnr, #IRQ_IN2
		bne	1001f

		tst	\irqstat, #IRQ_MASK_IN3
		movne	\irqnr, #IRQ_IN3
		bne	1001f

		tst	\irqstat, #IRQ_MASK_PCI
		movne	\irqnr, #IRQ_PCI
		bne	1001f

		tst	\irqstat, #IRQ_MASK_DOORBELLHOST
		movne	\irqnr, #IRQ_DOORBELLHOST
		bne     1001f

		tst	\irqstat, #IRQ_MASK_I2OINPOST
		movne	\irqnr, #IRQ_I2OINPOST
		bne	1001f

		tst	\irqstat, #IRQ_MASK_TIMER1
		movne	\irqnr, #IRQ_TIMER1
		bne	1001f

		tst	\irqstat, #IRQ_MASK_TIMER2
		movne	\irqnr, #IRQ_TIMER2
		bne	1001f

		tst	\irqstat, #IRQ_MASK_TIMER3
		movne	\irqnr, #IRQ_TIMER3
		bne	1001f

		tst	\irqstat, #IRQ_MASK_UART_TX
		movne	\irqnr, #IRQ_CONTX
		bne	1001f

		tst	\irqstat, #IRQ_MASK_PCI_ABORT
		movne	\irqnr, #IRQ_PCI_ABORT
		bne	1001f

		tst	\irqstat, #IRQ_MASK_PCI_SERR
		movne	\irqnr, #IRQ_PCI_SERR
		bne	1001f

		tst	\irqstat, #IRQ_MASK_DISCARD_TIMER
		movne	\irqnr, #IRQ_DISCARD_TIMER
		bne	1001f

		tst	\irqstat, #IRQ_MASK_PCI_DPERR
		movne	\irqnr, #IRQ_PCI_DPERR
		bne	1001f

		tst	\irqstat, #IRQ_MASK_PCI_PERR
		movne	\irqnr, #IRQ_PCI_PERR
1001:
		.endm

