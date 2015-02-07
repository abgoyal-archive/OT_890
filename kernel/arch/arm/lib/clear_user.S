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
 *  linux/arch/arm/lib/clear_user.S
 *
 *  Copyright (C) 1995, 1996,1997,1998 Russell King
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */
#include <linux/linkage.h>
#include <asm/assembler.h>

		.text

/* Prototype: int __clear_user(void *addr, size_t sz)
 * Purpose  : clear some user memory
 * Params   : addr - user memory address to clear
 *          : sz   - number of bytes to clear
 * Returns  : number of bytes NOT cleared
 */
ENTRY(__clear_user)
		stmfd	sp!, {r1, lr}
		mov	r2, #0
		cmp	r1, #4
		blt	2f
		ands	ip, r0, #3
		beq	1f
		cmp	ip, #2
USER(		strbt	r2, [r0], #1)
USER(		strlebt	r2, [r0], #1)
USER(		strltbt	r2, [r0], #1)
		rsb	ip, ip, #4
		sub	r1, r1, ip		@  7  6  5  4  3  2  1
1:		subs	r1, r1, #8		@ -1 -2 -3 -4 -5 -6 -7
USER(		strplt	r2, [r0], #4)
USER(		strplt	r2, [r0], #4)
		bpl	1b
		adds	r1, r1, #4		@  3  2  1  0 -1 -2 -3
USER(		strplt	r2, [r0], #4)
2:		tst	r1, #2			@ 1x 1x 0x 0x 1x 1x 0x
USER(		strnebt	r2, [r0], #1)
USER(		strnebt	r2, [r0], #1)
		tst	r1, #1			@ x1 x0 x1 x0 x1 x0 x1
USER(		strnebt	r2, [r0])
		mov	r0, #0
		ldmfd	sp!, {r1, pc}
ENDPROC(__clear_user)

		.section .fixup,"ax"
		.align	0
9001:		ldmfd	sp!, {r0, pc}
		.previous

