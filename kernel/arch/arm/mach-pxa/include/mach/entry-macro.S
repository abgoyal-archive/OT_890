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
 * arch/arm/mach-pxa/include/mach/entry-macro.S
 *
 * Low-level IRQ helper macros for PXA-based platforms
 *
 * This file is licensed under  the terms of the GNU General Public
 * License version 2. This program is licensed "as is" without any
 * warranty of any kind, whether express or implied.
 */
#include <mach/hardware.h>
#include <mach/irqs.h>

		.macro	disable_fiq
		.endm

		.macro  get_irqnr_preamble, base, tmp
		.endm

		.macro  arch_ret_to_user, tmp1, tmp2
		.endm

		.macro	get_irqnr_and_base, irqnr, irqstat, base, tmp
		mrc	p15, 0, \tmp, c0, c0, 0		@ CPUID
		mov	\tmp, \tmp, lsr #13
		and	\tmp, \tmp, #0x7		@ Core G
		cmp	\tmp, #1
		bhi	1004f

		mov	\base, #io_p2v(0x40000000)	@ IIR Ctl = 0x40d00000
		add	\base, \base, #0x00d00000
		ldr	\irqstat, [\base, #0]		@ ICIP
		ldr	\irqnr, [\base, #4]		@ ICMR
		b	1002f

1004:
		mrc	p6, 0, \irqstat, c6, c0, 0	@ ICIP2
		mrc	p6, 0, \irqnr, c7, c0, 0	@ ICMR2
		ands	\irqnr, \irqstat, \irqnr
		beq	1003f
		rsb	\irqstat, \irqnr, #0
		and	\irqstat, \irqstat, \irqnr
		clz	\irqnr, \irqstat
		rsb	\irqnr, \irqnr, #31
		add	\irqnr, \irqnr, #(32 + PXA_IRQ(0))
		b	1001f
1003:
		mrc	p6, 0, \irqstat, c0, c0, 0	@ ICIP
		mrc	p6, 0, \irqnr, c1, c0, 0	@ ICMR
1002:
		ands	\irqnr, \irqstat, \irqnr
		beq	1001f
		rsb	\irqstat, \irqnr, #0
		and	\irqstat, \irqstat, \irqnr
		clz	\irqnr, \irqstat
		rsb	\irqnr, \irqnr, #(31 + PXA_IRQ(0))
1001:
		.endm
