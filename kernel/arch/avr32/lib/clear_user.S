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
 * Copyright 2004-2006 Atmel Corporation
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */
#include <asm/page.h>
#include <asm/thread_info.h>
#include <asm/asm.h>

	.text
	.align	1
	.global	clear_user
	.type	clear_user, "function"
clear_user:
	branch_if_kernel r8, __clear_user
	ret_if_privileged r8, r12, r11, r11

	.global	__clear_user
	.type	__clear_user, "function"
__clear_user:
	mov	r9, r12
	mov	r8, 0
	andl	r9, 3, COH
	brne	5f

1:	sub	r11, 4
	brlt	2f

10:	st.w	r12++, r8
	sub	r11, 4
	brge	10b

2:	sub	r11, -4
	reteq	0

	/* Unaligned count or address */
	bld	r11, 1
	brcc	12f
11:	st.h	r12++, r8
	sub	r11, 2
	reteq	0
12:	st.b	r12++, r8
	retal	0

	/* Unaligned address */
5:	cp.w	r11, 4
	brlt	2b

	lsl	r9, 2
	add	pc, pc, r9
13:	st.b	r12++, r8
	sub	r11, 1
14:	st.b	r12++, r8
	sub	r11, 1
15:	st.b	r12++, r8
	sub	r11, 1
	rjmp	1b

	.size	clear_user, . - clear_user
	.size	__clear_user, . - __clear_user

	.section .fixup, "ax"
	.align	1
18:	sub	r11, -4
19:	retal	r11

	.section __ex_table, "a"
	.align	2
	.long	10b, 18b
	.long	11b, 19b
	.long	12b, 19b
	.long	13b, 19b
	.long	14b, 19b
	.long	15b, 19b
