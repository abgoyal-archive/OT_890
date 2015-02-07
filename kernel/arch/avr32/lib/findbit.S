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
 * Copyright (C) 2006 Atmel Corporation
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */
#include <linux/linkage.h>

	.text
	/*
	 * unsigned long find_first_zero_bit(const unsigned long *addr,
	 *				     unsigned long size)
	 */
ENTRY(find_first_zero_bit)
	cp.w	r11, 0
	reteq	r11
	mov	r9, r11
1:	ld.w	r8, r12[0]
	com	r8
	brne	.L_found
	sub	r12, -4
	sub	r9, 32
	brgt	1b
	retal	r11

	/*
	 * unsigned long find_next_zero_bit(const unsigned long *addr,
	 *				    unsigned long size,
	 *				    unsigned long offset)
	 */
ENTRY(find_next_zero_bit)
	lsr	r8, r10, 5
	sub	r9, r11, r10
	retle	r11

	lsl	r8, 2
	add	r12, r8
	andl	r10, 31, COH
	breq	1f

	/* offset is not word-aligned. Handle the first (32 - r10) bits */
	ld.w	r8, r12[0]
	com	r8
	sub	r12, -4
	lsr	r8, r8, r10
	brne	.L_found

	/* r9 = r9 - (32 - r10) = r9 + r10 - 32 */
	add	r9, r10
	sub	r9, 32
	retle	r11

	/* Main loop. offset must be word-aligned */
1:	ld.w	r8, r12[0]
	com	r8
	brne	.L_found
	sub	r12, -4
	sub	r9, 32
	brgt	1b
	retal	r11

	/* Common return path for when a bit is actually found. */
.L_found:
	brev	r8
	clz	r10, r8
	rsub	r9, r11
	add	r10, r9

	/* XXX: If we don't have to return exactly "size" when the bit
	   is not found, we may drop this "min" thing */
	min	r12, r11, r10
	retal	r12

	/*
	 * unsigned long find_first_bit(const unsigned long *addr,
	 *				unsigned long size)
	 */
ENTRY(find_first_bit)
	cp.w	r11, 0
	reteq	r11
	mov	r9, r11
1:	ld.w	r8, r12[0]
	cp.w	r8, 0
	brne	.L_found
	sub	r12, -4
	sub	r9, 32
	brgt	1b
	retal	r11

	/*
	 * unsigned long find_next_bit(const unsigned long *addr,
	 *			       unsigned long size,
	 *			       unsigned long offset)
	 */
ENTRY(find_next_bit)
	lsr	r8, r10, 5
	sub	r9, r11, r10
	retle	r11

	lsl	r8, 2
	add	r12, r8
	andl	r10, 31, COH
	breq	1f

	/* offset is not word-aligned. Handle the first (32 - r10) bits */
	ld.w	r8, r12[0]
	sub	r12, -4
	lsr	r8, r8, r10
	brne	.L_found

	/* r9 = r9 - (32 - r10) = r9 + r10 - 32 */
	add	r9, r10
	sub	r9, 32
	retle	r11

	/* Main loop. offset must be word-aligned */
1:	ld.w	r8, r12[0]
	cp.w	r8, 0
	brne	.L_found
	sub	r12, -4
	sub	r9, 32
	brgt	1b
	retal	r11

ENTRY(generic_find_next_le_bit)
	lsr	r8, r10, 5
	sub	r9, r11, r10
	retle	r11

	lsl	r8, 2
	add	r12, r8
	andl	r10, 31, COH
	breq	1f

	/* offset is not word-aligned. Handle the first (32 - r10) bits */
	ldswp.w	r8, r12[0]
	sub	r12, -4
	lsr	r8, r8, r10
	brne	.L_found

	/* r9 = r9 - (32 - r10) = r9 + r10 - 32 */
	add	r9, r10
	sub	r9, 32
	retle	r11

	/* Main loop. offset must be word-aligned */
1:	ldswp.w	r8, r12[0]
	cp.w	r8, 0
	brne	.L_found
	sub	r12, -4
	sub	r9, 32
	brgt	1b
	retal	r11

ENTRY(generic_find_next_zero_le_bit)
	lsr	r8, r10, 5
	sub	r9, r11, r10
	retle	r11

	lsl	r8, 2
	add	r12, r8
	andl	r10, 31, COH
	breq	1f

	/* offset is not word-aligned. Handle the first (32 - r10) bits */
	ldswp.w	r8, r12[0]
	sub	r12, -4
	com	r8
	lsr	r8, r8, r10
	brne	.L_found

	/* r9 = r9 - (32 - r10) = r9 + r10 - 32 */
	add	r9, r10
	sub	r9, 32
	retle	r11

	/* Main loop. offset must be word-aligned */
1:	ldswp.w	r8, r12[0]
	com	r8
	brne	.L_found
	sub	r12, -4
	sub	r9, 32
	brgt	1b
	retal	r11
