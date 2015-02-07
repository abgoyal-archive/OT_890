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
 *  linux/arch/arm/lib/memzero.S
 *
 *  Copyright (C) 1995-2000 Russell King
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */
#include <linux/linkage.h>
#include <asm/assembler.h>

	.text
	.align	5
	.word	0
/*
 * Align the pointer in r0.  r3 contains the number of bytes that we are
 * mis-aligned by, and r1 is the number of bytes.  If r1 < 4, then we
 * don't bother; we use byte stores instead.
 */
1:	subs	r1, r1, #4		@ 1 do we have enough
	blt	5f			@ 1 bytes to align with?
	cmp	r3, #2			@ 1
	strltb	r2, [r0], #1		@ 1
	strleb	r2, [r0], #1		@ 1
	strb	r2, [r0], #1		@ 1
	add	r1, r1, r3		@ 1 (r1 = r1 - (4 - r3))
/*
 * The pointer is now aligned and the length is adjusted.  Try doing the
 * memzero again.
 */

ENTRY(__memzero)
	mov	r2, #0			@ 1
	ands	r3, r0, #3		@ 1 unaligned?
	bne	1b			@ 1
/*
 * r3 = 0, and we know that the pointer in r0 is aligned to a word boundary.
 */
	cmp	r1, #16			@ 1 we can skip this chunk if we
	blt	4f			@ 1 have < 16 bytes

#if ! CALGN(1)+0

/*
 * We need an extra register for this loop - save the return address and
 * use the LR
 */
	str	lr, [sp, #-4]!		@ 1
	mov	ip, r2			@ 1
	mov	lr, r2			@ 1

3:	subs	r1, r1, #64		@ 1 write 32 bytes out per loop
	stmgeia	r0!, {r2, r3, ip, lr}	@ 4
	stmgeia	r0!, {r2, r3, ip, lr}	@ 4
	stmgeia	r0!, {r2, r3, ip, lr}	@ 4
	stmgeia	r0!, {r2, r3, ip, lr}	@ 4
	bgt	3b			@ 1
	ldmeqfd	sp!, {pc}		@ 1/2 quick exit
/*
 * No need to correct the count; we're only testing bits from now on
 */
	tst	r1, #32			@ 1
	stmneia	r0!, {r2, r3, ip, lr}	@ 4
	stmneia	r0!, {r2, r3, ip, lr}	@ 4
	tst	r1, #16			@ 1 16 bytes or more?
	stmneia	r0!, {r2, r3, ip, lr}	@ 4
	ldr	lr, [sp], #4		@ 1

#else

/*
 * This version aligns the destination pointer in order to write
 * whole cache lines at once.
 */

	stmfd	sp!, {r4-r7, lr}
	mov	r4, r2
	mov	r5, r2
	mov	r6, r2
	mov	r7, r2
	mov	ip, r2
	mov	lr, r2

	cmp	r1, #96
	andgts	ip, r0, #31
	ble	3f

	rsb	ip, ip, #32
	sub	r1, r1, ip
	movs	ip, ip, lsl #(32 - 4)
	stmcsia	r0!, {r4, r5, r6, r7}
	stmmiia	r0!, {r4, r5}
	movs	ip, ip, lsl #2
	strcs	r2, [r0], #4

3:	subs	r1, r1, #64
	stmgeia	r0!, {r2-r7, ip, lr}
	stmgeia	r0!, {r2-r7, ip, lr}
	bgt	3b
	ldmeqfd	sp!, {r4-r7, pc}

	tst	r1, #32
	stmneia	r0!, {r2-r7, ip, lr}
	tst	r1, #16
	stmneia	r0!, {r4-r7}
	ldmfd	sp!, {r4-r7, lr}

#endif

4:	tst	r1, #8			@ 1 8 bytes or more?
	stmneia	r0!, {r2, r3}		@ 2
	tst	r1, #4			@ 1 4 bytes or more?
	strne	r2, [r0], #4		@ 1
/*
 * When we get here, we've got less than 4 bytes to zero.  We
 * may have an unaligned pointer as well.
 */
5:	tst	r1, #2			@ 1 2 bytes or more?
	strneb	r2, [r0], #1		@ 1
	strneb	r2, [r0], #1		@ 1
	tst	r1, #1			@ 1 a byte left over
	strneb	r2, [r0], #1		@ 1
	mov	pc, lr			@ 1
ENDPROC(__memzero)
