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
 *  linux/arch/arm/lib/findbit.S
 *
 *  Copyright (C) 1995-2000 Russell King
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * 16th March 2001 - John Ripley <jripley@sonicblue.com>
 *   Fixed so that "size" is an exclusive not an inclusive quantity.
 *   All users of these functions expect exclusive sizes, and may
 *   also call with zero size.
 * Reworked by rmk.
 */
#include <linux/linkage.h>
#include <asm/assembler.h>
                .text

/*
 * Purpose  : Find a 'zero' bit
 * Prototype: int find_first_zero_bit(void *addr, unsigned int maxbit);
 */
ENTRY(_find_first_zero_bit_le)
		teq	r1, #0	
		beq	3f
		mov	r2, #0
1:		ldrb	r3, [r0, r2, lsr #3]
		eors	r3, r3, #0xff		@ invert bits
		bne	.L_found		@ any now set - found zero bit
		add	r2, r2, #8		@ next bit pointer
2:		cmp	r2, r1			@ any more?
		blo	1b
3:		mov	r0, r1			@ no free bits
		mov	pc, lr
ENDPROC(_find_first_zero_bit_le)

/*
 * Purpose  : Find next 'zero' bit
 * Prototype: int find_next_zero_bit(void *addr, unsigned int maxbit, int offset)
 */
ENTRY(_find_next_zero_bit_le)
		teq	r1, #0
		beq	3b
		ands	ip, r2, #7
		beq	1b			@ If new byte, goto old routine
		ldrb	r3, [r0, r2, lsr #3]
		eor	r3, r3, #0xff		@ now looking for a 1 bit
		movs	r3, r3, lsr ip		@ shift off unused bits
		bne	.L_found
		orr	r2, r2, #7		@ if zero, then no bits here
		add	r2, r2, #1		@ align bit pointer
		b	2b			@ loop for next bit
ENDPROC(_find_next_zero_bit_le)

/*
 * Purpose  : Find a 'one' bit
 * Prototype: int find_first_bit(const unsigned long *addr, unsigned int maxbit);
 */
ENTRY(_find_first_bit_le)
		teq	r1, #0	
		beq	3f
		mov	r2, #0
1:		ldrb	r3, [r0, r2, lsr #3]
		movs	r3, r3
		bne	.L_found		@ any now set - found zero bit
		add	r2, r2, #8		@ next bit pointer
2:		cmp	r2, r1			@ any more?
		blo	1b
3:		mov	r0, r1			@ no free bits
		mov	pc, lr
ENDPROC(_find_first_bit_le)

/*
 * Purpose  : Find next 'one' bit
 * Prototype: int find_next_zero_bit(void *addr, unsigned int maxbit, int offset)
 */
ENTRY(_find_next_bit_le)
		teq	r1, #0
		beq	3b
		ands	ip, r2, #7
		beq	1b			@ If new byte, goto old routine
		ldrb	r3, [r0, r2, lsr #3]
		movs	r3, r3, lsr ip		@ shift off unused bits
		bne	.L_found
		orr	r2, r2, #7		@ if zero, then no bits here
		add	r2, r2, #1		@ align bit pointer
		b	2b			@ loop for next bit
ENDPROC(_find_next_bit_le)

#ifdef __ARMEB__

ENTRY(_find_first_zero_bit_be)
		teq	r1, #0
		beq	3f
		mov	r2, #0
1:		eor	r3, r2, #0x18		@ big endian byte ordering
		ldrb	r3, [r0, r3, lsr #3]
		eors	r3, r3, #0xff		@ invert bits
		bne	.L_found		@ any now set - found zero bit
		add	r2, r2, #8		@ next bit pointer
2:		cmp	r2, r1			@ any more?
		blo	1b
3:		mov	r0, r1			@ no free bits
		mov	pc, lr
ENDPROC(_find_first_zero_bit_be)

ENTRY(_find_next_zero_bit_be)
		teq	r1, #0
		beq	3b
		ands	ip, r2, #7
		beq	1b			@ If new byte, goto old routine
		eor	r3, r2, #0x18		@ big endian byte ordering
		ldrb	r3, [r0, r3, lsr #3]
		eor	r3, r3, #0xff		@ now looking for a 1 bit
		movs	r3, r3, lsr ip		@ shift off unused bits
		bne	.L_found
		orr	r2, r2, #7		@ if zero, then no bits here
		add	r2, r2, #1		@ align bit pointer
		b	2b			@ loop for next bit
ENDPROC(_find_next_zero_bit_be)

ENTRY(_find_first_bit_be)
		teq	r1, #0
		beq	3f
		mov	r2, #0
1:		eor	r3, r2, #0x18		@ big endian byte ordering
		ldrb	r3, [r0, r3, lsr #3]
		movs	r3, r3
		bne	.L_found		@ any now set - found zero bit
		add	r2, r2, #8		@ next bit pointer
2:		cmp	r2, r1			@ any more?
		blo	1b
3:		mov	r0, r1			@ no free bits
		mov	pc, lr
ENDPROC(_find_first_bit_be)

ENTRY(_find_next_bit_be)
		teq	r1, #0
		beq	3b
		ands	ip, r2, #7
		beq	1b			@ If new byte, goto old routine
		eor	r3, r2, #0x18		@ big endian byte ordering
		ldrb	r3, [r0, r3, lsr #3]
		movs	r3, r3, lsr ip		@ shift off unused bits
		bne	.L_found
		orr	r2, r2, #7		@ if zero, then no bits here
		add	r2, r2, #1		@ align bit pointer
		b	2b			@ loop for next bit
ENDPROC(_find_next_bit_be)

#endif

/*
 * One or more bits in the LSB of r3 are assumed to be set.
 */
.L_found:
#if __LINUX_ARM_ARCH__ >= 5
		rsb	r1, r3, #0
		and	r3, r3, r1
		clz	r3, r3
		rsb	r3, r3, #31
		add	r0, r2, r3
#else
		tst	r3, #0x0f
		addeq	r2, r2, #4
		movne	r3, r3, lsl #4
		tst	r3, #0x30
		addeq	r2, r2, #2
		movne	r3, r3, lsl #2
		tst	r3, #0x40
		addeq	r2, r2, #1
		mov	r0, r2
#endif
		mov	pc, lr

