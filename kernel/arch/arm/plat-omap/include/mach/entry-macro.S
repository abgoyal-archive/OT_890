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
 * arch/arm/plat-omap/include/mach/entry-macro.S
 *
 * Low-level IRQ helper macros for OMAP-based platforms
 *
 * This file is licensed under  the terms of the GNU General Public
 * License version 2. This program is licensed "as is" without any
 * warranty of any kind, whether express or implied.
 */
#include <mach/hardware.h>
#include <mach/io.h>
#include <mach/irqs.h>

#if defined(CONFIG_ARCH_OMAP1)

#if defined(CONFIG_ARCH_OMAP730) && \
	(defined(CONFIG_ARCH_OMAP15XX) || defined(CONFIG_ARCH_OMAP16XX))
#error "FIXME: OMAP730 doesn't support multiple-OMAP"
#elif defined(CONFIG_ARCH_OMAP730)
#define INT_IH2_IRQ		INT_730_IH2_IRQ
#elif defined(CONFIG_ARCH_OMAP15XX)
#define INT_IH2_IRQ		INT_1510_IH2_IRQ
#elif defined(CONFIG_ARCH_OMAP16XX)
#define INT_IH2_IRQ		INT_1610_IH2_IRQ
#else
#warning "IH2 IRQ defaulted"
#define INT_IH2_IRQ		INT_1510_IH2_IRQ
#endif

 		.macro	disable_fiq
		.endm

		.macro  get_irqnr_preamble, base, tmp
		.endm

		.macro  arch_ret_to_user, tmp1, tmp2
		.endm

		.macro	get_irqnr_and_base, irqnr, irqstat, base, tmp
		ldr	\base, =IO_ADDRESS(OMAP_IH1_BASE)
		ldr	\irqnr, [\base, #IRQ_ITR_REG_OFFSET]
		ldr	\tmp, [\base, #IRQ_MIR_REG_OFFSET]
		mov	\irqstat, #0xffffffff
		bic	\tmp, \irqstat, \tmp
		tst	\irqnr, \tmp
		beq	1510f

		ldr	\irqnr, [\base, #IRQ_SIR_FIQ_REG_OFFSET]
		cmp	\irqnr, #0
		ldreq	\irqnr, [\base, #IRQ_SIR_IRQ_REG_OFFSET]
		cmpeq	\irqnr, #INT_IH2_IRQ
		ldreq	\base, =IO_ADDRESS(OMAP_IH2_BASE)
		ldreq	\irqnr, [\base, #IRQ_SIR_IRQ_REG_OFFSET]
		addeqs	\irqnr, \irqnr, #32
1510:
		.endm

#endif
#if defined(CONFIG_ARCH_OMAP24XX) || defined(CONFIG_ARCH_OMAP34XX)

#if defined(CONFIG_ARCH_OMAP24XX)
#include <mach/omap24xx.h>
#endif
#if defined(CONFIG_ARCH_OMAP34XX)
#include <mach/omap34xx.h>
#endif

#define INTCPS_SIR_IRQ_OFFSET	0x0040		/* Active interrupt offset */
#define	ACTIVEIRQ_MASK		0x7f		/* Active interrupt bits */

		.macro	disable_fiq
		.endm

		.macro  get_irqnr_preamble, base, tmp
		.endm

		.macro  arch_ret_to_user, tmp1, tmp2
		.endm

		.macro	get_irqnr_and_base, irqnr, irqstat, base, tmp
		ldr	\base, =OMAP2_VA_IC_BASE
		ldr	\irqnr, [\base, #0x98] /* IRQ pending reg 1 */
		cmp	\irqnr, #0x0
		bne	2222f
		ldr	\irqnr, [\base, #0xb8] /* IRQ pending reg 2 */
		cmp	\irqnr, #0x0
		bne	2222f
		ldr	\irqnr, [\base, #0xd8] /* IRQ pending reg 3 */
		cmp	\irqnr, #0x0
2222:
		ldrne	\irqnr, [\base, #INTCPS_SIR_IRQ_OFFSET]
		and	\irqnr, \irqnr, #ACTIVEIRQ_MASK /* Clear spurious bits */

		.endm

		.macro	irq_prio_table
		.endm

#endif
