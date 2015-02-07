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
 *  linux/arch/arm/kernel/debug.S
 *
 *  Copyright (C) 1994-1999 Russell King
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 *  32-bit debugging code
 */
#include <linux/linkage.h>

		.text

/*
 * Some debugging routines (useful if you've got MM problems and
 * printk isn't working).  For DEBUGGING ONLY!!!  Do not leave
 * references to these in a production kernel!
 */

#if defined(CONFIG_DEBUG_ICEDCC)
		@@ debug using ARM EmbeddedICE DCC channel

#if defined(CONFIG_CPU_V6)

		.macro	addruart, rx
		.endm

		.macro	senduart, rd, rx
		mcr	p14, 0, \rd, c0, c5, 0
		.endm

		.macro	busyuart, rd, rx
1001:
		mrc	p14, 0, \rx, c0, c1, 0
		tst	\rx, #0x20000000
		beq	1001b
		.endm

		.macro	waituart, rd, rx
		mov	\rd, #0x2000000
1001:
		subs	\rd, \rd, #1
		bmi	1002f
		mrc	p14, 0, \rx, c0, c1, 0
		tst	\rx, #0x20000000
		bne	1001b
1002:
		.endm

#else

		.macro	addruart, rx
		.endm

		.macro	senduart, rd, rx
		mcr	p14, 0, \rd, c1, c0, 0
		.endm

		.macro	busyuart, rd, rx
1001:
		mrc	p14, 0, \rx, c0, c0, 0
		tst	\rx, #2
		beq	1001b

		.endm

		.macro	waituart, rd, rx
		mov	\rd, #0x2000000
1001:
		subs	\rd, \rd, #1
		bmi	1002f
		mrc	p14, 0, \rx, c0, c0, 0
		tst	\rx, #2
		bne	1001b
1002:
		.endm

#endif	/* CONFIG_CPU_V6 */

#else
#include <mach/debug-macro.S>
#endif	/* CONFIG_DEBUG_ICEDCC */

/*
 * Useful debugging routines
 */
ENTRY(printhex8)
		mov	r1, #8
		b	printhex
ENDPROC(printhex8)

ENTRY(printhex4)
		mov	r1, #4
		b	printhex
ENDPROC(printhex4)

ENTRY(printhex2)
		mov	r1, #2
printhex:	adr	r2, hexbuf
		add	r3, r2, r1
		mov	r1, #0
		strb	r1, [r3]
1:		and	r1, r0, #15
		mov	r0, r0, lsr #4
		cmp	r1, #10
		addlt	r1, r1, #'0'
		addge	r1, r1, #'a' - 10
		strb	r1, [r3, #-1]!
		teq	r3, r2
		bne	1b
		mov	r0, r2
		b	printascii
ENDPROC(printhex2)

		.ltorg

ENTRY(printascii)
		addruart r3
		b	2f
1:		waituart r2, r3
		senduart r1, r3
		busyuart r2, r3
		teq	r1, #'\n'
		moveq	r1, #'\r'
		beq	1b
2:		teq	r0, #0
		ldrneb	r1, [r0], #1
		teqne	r1, #0
		bne	1b
		mov	pc, lr
ENDPROC(printascii)

ENTRY(printch)
		addruart r3
		mov	r1, r0
		mov	r0, #0
		b	1b
ENDPROC(printch)

hexbuf:		.space 16
