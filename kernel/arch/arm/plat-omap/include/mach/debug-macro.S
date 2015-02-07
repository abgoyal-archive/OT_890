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

/* arch/arm/plat-omap/include/mach/debug-macro.S
 *
 * Debugging macro include header
 *
 *  Copyright (C) 1994-1999 Russell King
 *  Moved from linux/arch/arm/kernel/debug.S by Ben Dooks
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
*/

		.macro	addruart,rx
		mrc	p15, 0, \rx, c1, c0
		tst	\rx, #1			@ MMU enabled?
#ifdef CONFIG_ARCH_OMAP1
		moveq	\rx, #0xff000000	@ physical base address
		movne	\rx, #0xfe000000	@ virtual base
		orr	\rx, \rx, #0x00fb0000
#ifdef CONFIG_OMAP_LL_DEBUG_UART3
		orr	\rx, \rx, #0x00009000	@ UART 3
#endif
#if defined(CONFIG_OMAP_LL_DEBUG_UART2) || defined(CONFIG_OMAP_LL_DEBUG_UART3)
		orr	\rx, \rx, #0x00000800	@ UART 2 & 3
#endif

#elif  CONFIG_ARCH_OMAP2
		moveq	\rx, #0x48000000	@ physical base address
		movne	\rx, #0xd8000000	@ virtual base
		orr	\rx, \rx, #0x0006a000
#ifdef CONFIG_OMAP_LL_DEBUG_UART2
		add	\rx, \rx, #0x00002000	@ UART 2
#endif
#ifdef CONFIG_OMAP_LL_DEBUG_UART3
		add	\rx, \rx, #0x00004000	@ UART 3
#endif

#elif	CONFIG_ARCH_OMAP3
		moveq	\rx, #0x48000000	@ physical base address
		movne	\rx, #0xd8000000	@ virtual base
		orr	\rx, \rx, #0x0006a000
#ifdef CONFIG_OMAP_LL_DEBUG_UART2
		add	\rx, \rx, #0x00002000	@ UART 2
#endif
#ifdef CONFIG_OMAP_LL_DEBUG_UART3
		add	\rx, \rx, #0x00fb0000	@ UART 3
		add	\rx, \rx, #0x00006000
#endif
#endif
		.endm

		.macro	senduart,rd,rx
		strb	\rd, [\rx]
		.endm

		.macro	busyuart,rd,rx
1001:		ldrb	\rd, [\rx, #(0x5 << 2)]	@ OMAP-1510 and friends
		and	\rd, \rd, #0x60
		teq	\rd, #0x60
		beq	1002f
		ldrb	\rd, [\rx, #(0x5 << 0)]	@ OMAP-730 only
		and	\rd, \rd, #0x60
		teq	\rd, #0x60
		bne	1001b
1002:
		.endm

		.macro	waituart,rd,rx
		.endm
