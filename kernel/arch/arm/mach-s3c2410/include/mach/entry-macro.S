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
 * arch/arm/mach-s3c2410/include/mach/entry-macro.S
 *
 * Low-level IRQ helper macros for S3C2410-based platforms
 *
 * This file is licensed under  the terms of the GNU General Public
 * License version 2. This program is licensed "as is" without any
 * warranty of any kind, whether express or implied.
*/

/* We have a problem that the INTOFFSET register does not always
 * show one interrupt. Occasionally we get two interrupts through
 * the prioritiser, and this causes the INTOFFSET register to show
 * what looks like the logical-or of the two interrupt numbers.
 *
 * Thanks to Klaus, Shannon, et al for helping to debug this problem
*/

#define INTPND		(0x10)
#define INTOFFSET	(0x14)

#include <mach/hardware.h>
#include <asm/irq.h>

	.macro  get_irqnr_preamble, base, tmp
	.endm

	.macro  arch_ret_to_user, tmp1, tmp2
	.endm

	.macro	get_irqnr_and_base, irqnr, irqstat, base, tmp

		mov	\base, #S3C24XX_VA_IRQ

		@@ try the interrupt offset register, since it is there

		ldr	\irqstat, [ \base, #INTPND ]
		teq	\irqstat, #0
		beq	1002f
		ldr	\irqnr, [ \base, #INTOFFSET ]
		mov	\tmp, #1
		tst	\irqstat, \tmp, lsl \irqnr
		bne	1001f

		@@ the number specified is not a valid irq, so try
		@@ and work it out for ourselves

		mov	\irqnr, #0		@@ start here

		@@ work out which irq (if any) we got

		movs	\tmp, \irqstat, lsl#16
		addeq	\irqnr, \irqnr, #16
		moveq	\irqstat, \irqstat, lsr#16
		tst	\irqstat, #0xff
		addeq	\irqnr, \irqnr, #8
		moveq	\irqstat, \irqstat, lsr#8
		tst	\irqstat, #0xf
		addeq	\irqnr, \irqnr, #4
		moveq	\irqstat, \irqstat, lsr#4
		tst	\irqstat, #0x3
		addeq	\irqnr, \irqnr, #2
		moveq	\irqstat, \irqstat, lsr#2
		tst	\irqstat, #0x1
		addeq	\irqnr, \irqnr, #1

		@@ we have the value
1001:
		adds	\irqnr, \irqnr, #IRQ_EINT0
1002:
		@@ exit here, Z flag unset if IRQ

	.endm

		/* currently don't need an disable_fiq macro */

		.macro	disable_fiq
		.endm
