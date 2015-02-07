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
 * arch/arm/mach-lh7a40x/include/mach/entry-macro.S
 *
 * Low-level IRQ helper macros for LH7A40x platforms
 *
 * This file is licensed under  the terms of the GNU General Public
 * License version 2. This program is licensed "as is" without any
 * warranty of any kind, whether express or implied.
 */
#include <mach/hardware.h>
#include <mach/irqs.h>

/* In order to allow there to be support for both of the processor
   classes at the same time, we make a hack here that isn't very
   pretty.  At startup, the link pointed to with the
   branch_irq_lh7a400 symbol is replaced with a NOP when the CPU is
   detected as a lh7a404.

   *** FIXME: we should clean this up so that there is only one
	      implementation for each CPU's design.

*/

#if defined (CONFIG_ARCH_LH7A400) && defined (CONFIG_ARCH_LH7A404)

		.macro	disable_fiq
		.endm

		.macro  get_irqnr_preamble, base, tmp
		.endm

		.macro  arch_ret_to_user, tmp1, tmp2
		.endm

		.macro	get_irqnr_and_base, irqnr, irqstat, base, tmp

branch_irq_lh7a400: b 1000f

@ Implementation of the LH7A404 get_irqnr_and_base.

		mov	\irqnr, #0			@ VIC1 irq base
		mov	\base, #io_p2v(0x80000000)	@ APB registers
		add	\base, \base, #0x8000
		ldr	\tmp, [\base, #0x0030]		@ VIC1_VECTADDR
		tst	\tmp, #VA_VECTORED		@ Direct vectored
		bne	1002f
		tst	\tmp, #VA_VIC1DEFAULT		@ Default vectored VIC1
		ldrne	\irqstat, [\base, #0]		@ VIC1_IRQSTATUS
		bne	1001f
		add	\base, \base, #(0xa000 - 0x8000)
		ldr	\tmp, [\base, #0x0030]		@ VIC2_VECTADDR
		tst	\tmp, #VA_VECTORED		@ Direct vectored
		bne	1002f
		ldr	\irqstat, [\base, #0]		@ VIC2_IRQSTATUS
		mov	\irqnr, #32			@ VIC2 irq base

1001:		movs	\irqstat, \irqstat, lsr #1	@ Shift into carry
		bcs	1008f				@ Bit set; irq found
		add	\irqnr, \irqnr, #1
		bne	1001b				@ Until no bits
		b	1009f				@ Nothing?  Hmm.
1002:		and	\irqnr, \tmp, #0x3f		@ Mask for valid bits
1008:		movs	\irqstat, #1			@ Force !Z
		str	\tmp, [\base, #0x0030]		@ Clear vector
		b	1009f

@ Implementation of the LH7A400 get_irqnr_and_base.

1000:		mov	\irqnr, #0
		mov	\base, #io_p2v(0x80000000)	@ APB registers
		ldr	\irqstat, [\base, #0x500]	@ PIC INTSR

1001:		movs	\irqstat, \irqstat, lsr #1	@ Shift into carry
		bcs	1008f				@ Bit set; irq found
		add	\irqnr, \irqnr, #1
		bne	1001b				@ Until no bits
		b	1009f				@ Nothing?  Hmm.
1008:		movs	\irqstat, #1			@ Force !Z

1009:
               .endm



#elif defined (CONFIG_ARCH_LH7A400)
		.macro	disable_fiq
		.endm

		.macro  get_irqnr_preamble, base, tmp
		.endm

		.macro  arch_ret_to_user, tmp1, tmp2
		.endm

		.macro	get_irqnr_and_base, irqnr, irqstat, base, tmp
		mov	\irqnr, #0
		mov	\base, #io_p2v(0x80000000)	@ APB registers
		ldr	\irqstat, [\base, #0x500]	@ PIC INTSR

1001:		movs	\irqstat, \irqstat, lsr #1	@ Shift into carry
		bcs	1008f				@ Bit set; irq found
		add	\irqnr, \irqnr, #1
		bne	1001b				@ Until no bits
		b	1009f				@ Nothing?  Hmm.
1008:		movs	\irqstat, #1			@ Force !Z
1009:
               .endm

#elif defined(CONFIG_ARCH_LH7A404)

		.macro	disable_fiq
		.endm

		.macro  get_irqnr_preamble, base, tmp
		.endm

		.macro  arch_ret_to_user, tmp1, tmp2
		.endm

		.macro	get_irqnr_and_base, irqnr, irqstat, base, tmp
		mov	\irqnr, #0			@ VIC1 irq base
		mov	\base, #io_p2v(0x80000000)	@ APB registers
		add	\base, \base, #0x8000
		ldr	\tmp, [\base, #0x0030]		@ VIC1_VECTADDR
		tst	\tmp, #VA_VECTORED		@ Direct vectored
		bne	1002f
		tst	\tmp, #VA_VIC1DEFAULT		@ Default vectored VIC1
		ldrne	\irqstat, [\base, #0]		@ VIC1_IRQSTATUS
		bne	1001f
		add	\base, \base, #(0xa000 - 0x8000)
		ldr	\tmp, [\base, #0x0030]		@ VIC2_VECTADDR
		tst	\tmp, #VA_VECTORED		@ Direct vectored
		bne	1002f
		ldr	\irqstat, [\base, #0]		@ VIC2_IRQSTATUS
		mov	\irqnr, #32			@ VIC2 irq base

1001:		movs	\irqstat, \irqstat, lsr #1	@ Shift into carry
		bcs	1008f				@ Bit set; irq found
		add	\irqnr, \irqnr, #1
		bne	1001b				@ Until no bits
		b	1009f				@ Nothing?  Hmm.
1002:		and	\irqnr, \tmp, #0x3f		@ Mask for valid bits
1008:		movs	\irqstat, #1			@ Force !Z
		str	\tmp, [\base, #0x0030]		@ Clear vector
1009:
               .endm
#endif


