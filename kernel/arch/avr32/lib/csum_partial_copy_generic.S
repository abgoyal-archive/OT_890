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
 * Copyright (C) 2004-2006 Atmel Corporation
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */
#include <asm/errno.h>
#include <asm/asm.h>

	/*
	 * unsigned int csum_partial_copy_generic(const char *src, char *dst, int len
	 *					  int sum, int *src_err_ptr,
	 *					  int *dst_err_ptr)
	 *
	 * Copy src to dst while checksumming, otherwise like csum_partial.
	 */

	.macro ld_src size, reg, ptr
9999:	ld.\size \reg, \ptr
	.section __ex_table, "a"
	.long	9999b, fixup_ld_src
	.previous
	.endm

	.macro st_dst size, ptr, reg
9999:	st.\size \ptr, \reg
	.section __ex_table, "a"
	.long	9999b, fixup_st_dst
	.previous
	.endm

	.text
	.global	csum_partial_copy_generic
	.type	csum_partial_copy_generic,"function"
	.align	1
csum_partial_copy_generic:
	pushm	r4-r7,lr

	/* The inner loop */
1:	sub	r10, 4
	brlt	5f
2:	ld_src	w, r5, r12++
	st_dst	w, r11++, r5
	add	r9, r5
	acr	r9
	sub	r10, 4
	brge	2b

	/* return if we had a whole number of words */
5:	sub	r10, -4
	brne	7f

6:	mov	r12, r9
	popm	r4-r7,pc

	/* handle additional bytes at the tail */
7:	mov	r5, 0
	mov	r4, 32
8:	ld_src	ub, r6, r12++
	st_dst	b, r11++, r6
	lsl	r5, 8
	sub	r4, 8
	bfins	r5, r6, 0, 8
	sub	r10, 1
	brne	8b

	lsl	r5, r5, r4
	add	r9, r5
	acr	r9
	rjmp	6b

	/* Exception handler */
	.section .fixup,"ax"
	.align	1
fixup_ld_src:
	mov	r9, -EFAULT
	cp.w	r8, 0
	breq	1f
	st.w	r8[0], r9

1:	/*
	 * TODO: zero the complete destination - computing the rest
	 * is too much work
	 */

	mov	r9, 0
	rjmp	6b

fixup_st_dst:
	mov	r9, -EFAULT
	lddsp	r8, sp[20]
	cp.w	r8, 0
	breq	1f
	st.w	r8[0], r9
1:	mov	r9, 0
	rjmp	6b

	.previous
