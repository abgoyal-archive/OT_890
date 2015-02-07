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
 * arch/arm/include/asm/hardware/entry-macro-iomd.S
 *
 * Low-level IRQ helper macros for IOC/IOMD based platforms
 *
 * This file is licensed under  the terms of the GNU General Public
 * License version 2. This program is licensed "as is" without any
 * warranty of any kind, whether express or implied.
 */

/* IOC / IOMD based hardware */
#include <asm/hardware/iomd.h>

		.macro	disable_fiq
		mov	r12, #ioc_base_high
		.if	ioc_base_low
		orr	r12, r12, #ioc_base_low
		.endif
		strb	r12, [r12, #0x38]	@ Disable FIQ register
		.endm

		.macro	get_irqnr_and_base, irqnr, irqstat, base, tmp
		ldrb	\irqstat, [\base, #IOMD_IRQREQB]	@ get high priority first
		ldr	\tmp, =irq_prio_h
		teq	\irqstat, #0
#ifdef IOMD_BASE
		ldreqb	\irqstat, [\base, #IOMD_DMAREQ]	@ get dma
		addeq	\tmp, \tmp, #256		@ irq_prio_h table size
		teqeq	\irqstat, #0
		bne	2406f
#endif
		ldreqb	\irqstat, [\base, #IOMD_IRQREQA]	@ get low priority
		addeq	\tmp, \tmp, #256		@ irq_prio_d table size
		teqeq	\irqstat, #0
#ifdef IOMD_IRQREQC
		ldreqb	\irqstat, [\base, #IOMD_IRQREQC]
		addeq	\tmp, \tmp, #256		@ irq_prio_l table size
		teqeq	\irqstat, #0
#endif
#ifdef IOMD_IRQREQD
		ldreqb	\irqstat, [\base, #IOMD_IRQREQD]
		addeq	\tmp, \tmp, #256		@ irq_prio_lc table size
		teqeq	\irqstat, #0
#endif
2406:		ldrneb	\irqnr, [\tmp, \irqstat]	@ get IRQ number
		.endm

/*
 * Interrupt table (incorporates priority).  Please note that we
 * rely on the order of these tables (see above code).
 */
		.align	5
irq_prio_h:	.byte	 0, 8, 9, 8,10,10,10,10,11,11,11,11,10,10,10,10
		.byte	12, 8, 9, 8,10,10,10,10,11,11,11,11,10,10,10,10
		.byte	13,13,13,13,10,10,10,10,11,11,11,11,10,10,10,10
		.byte	13,13,13,13,10,10,10,10,11,11,11,11,10,10,10,10
		.byte	14,14,14,14,10,10,10,10,11,11,11,11,10,10,10,10
		.byte	14,14,14,14,10,10,10,10,11,11,11,11,10,10,10,10
		.byte	13,13,13,13,10,10,10,10,11,11,11,11,10,10,10,10
		.byte	13,13,13,13,10,10,10,10,11,11,11,11,10,10,10,10
		.byte	15,15,15,15,10,10,10,10,11,11,11,11,10,10,10,10
		.byte	15,15,15,15,10,10,10,10,11,11,11,11,10,10,10,10
		.byte	13,13,13,13,10,10,10,10,11,11,11,11,10,10,10,10
		.byte	13,13,13,13,10,10,10,10,11,11,11,11,10,10,10,10
		.byte	15,15,15,15,10,10,10,10,11,11,11,11,10,10,10,10
		.byte	15,15,15,15,10,10,10,10,11,11,11,11,10,10,10,10
		.byte	13,13,13,13,10,10,10,10,11,11,11,11,10,10,10,10
		.byte	13,13,13,13,10,10,10,10,11,11,11,11,10,10,10,10
#ifdef IOMD_BASE
irq_prio_d:	.byte	 0,16,17,16,18,16,17,16,19,16,17,16,18,16,17,16
		.byte	20,16,17,16,18,16,17,16,19,16,17,16,18,16,17,16
		.byte	21,16,17,16,18,16,17,16,19,16,17,16,18,16,17,16
		.byte	21,16,17,16,18,16,17,16,19,16,17,16,18,16,17,16
		.byte	22,16,17,16,18,16,17,16,19,16,17,16,18,16,17,16
		.byte	22,16,17,16,18,16,17,16,19,16,17,16,18,16,17,16
		.byte	21,16,17,16,18,16,17,16,19,16,17,16,18,16,17,16
		.byte	21,16,17,16,18,16,17,16,19,16,17,16,18,16,17,16
		.byte	23,16,17,16,18,16,17,16,19,16,17,16,18,16,17,16
		.byte	23,16,17,16,18,16,17,16,19,16,17,16,18,16,17,16
		.byte	21,16,17,16,18,16,17,16,19,16,17,16,18,16,17,16
		.byte	21,16,17,16,18,16,17,16,19,16,17,16,18,16,17,16
		.byte	22,16,17,16,18,16,17,16,19,16,17,16,18,16,17,16
		.byte	22,16,17,16,18,16,17,16,19,16,17,16,18,16,17,16
		.byte	21,16,17,16,18,16,17,16,19,16,17,16,18,16,17,16
		.byte	21,16,17,16,18,16,17,16,19,16,17,16,18,16,17,16
#endif
irq_prio_l:	.byte	 0, 0, 1, 0, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3
		.byte	 4, 0, 1, 0, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3
		.byte	 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5
		.byte	 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5
		.byte	 6, 6, 6, 6, 6, 6, 6, 6, 3, 3, 3, 3, 3, 3, 3, 3
		.byte	 6, 6, 6, 6, 6, 6, 6, 6, 3, 3, 3, 3, 3, 3, 3, 3
		.byte	 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5
		.byte	 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5
		.byte	 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7
		.byte	 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7
		.byte	 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7
		.byte	 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7
		.byte	 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7
		.byte	 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7
		.byte	 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7
		.byte	 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7
#ifdef IOMD_IRQREQC
irq_prio_lc:	.byte	24,24,25,24,26,26,26,26,27,27,27,27,27,27,27,27
		.byte	28,24,25,24,26,26,26,26,27,27,27,27,27,27,27,27
		.byte	29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29
		.byte	29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29
		.byte	30,30,30,30,30,30,30,30,27,27,27,27,27,27,27,27
		.byte	30,30,30,30,30,30,30,30,27,27,27,27,27,27,27,27
		.byte	29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29
		.byte	29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29
		.byte	31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31
		.byte	31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31
		.byte	31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31
		.byte	31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31
		.byte	31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31
		.byte	31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31
		.byte	31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31
		.byte	31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31
#endif
#ifdef IOMD_IRQREQD
irq_prio_ld:	.byte	40,40,41,40,42,42,42,42,43,43,43,43,43,43,43,43
		.byte	44,40,41,40,42,42,42,42,43,43,43,43,43,43,43,43
		.byte	45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45
		.byte	45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45
		.byte	46,46,46,46,46,46,46,46,43,43,43,43,43,43,43,43
		.byte	46,46,46,46,46,46,46,46,43,43,43,43,43,43,43,43
		.byte	45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45
		.byte	45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45
		.byte	47,47,47,47,47,47,47,47,47,47,47,47,47,47,47,47
		.byte	47,47,47,47,47,47,47,47,47,47,47,47,47,47,47,47
		.byte	47,47,47,47,47,47,47,47,47,47,47,47,47,47,47,47
		.byte	47,47,47,47,47,47,47,47,47,47,47,47,47,47,47,47
		.byte	47,47,47,47,47,47,47,47,47,47,47,47,47,47,47,47
		.byte	47,47,47,47,47,47,47,47,47,47,47,47,47,47,47,47
		.byte	47,47,47,47,47,47,47,47,47,47,47,47,47,47,47,47
		.byte	47,47,47,47,47,47,47,47,47,47,47,47,47,47,47,47
#endif

