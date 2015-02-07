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
 * Copy to/from userspace with optional address space checking.
 *
 * Copyright 2004-2006 Atmel Corporation
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */
#include <asm/page.h>
#include <asm/thread_info.h>
#include <asm/asm.h>

	/*
	 * __kernel_size_t
	 * __copy_user(void *to, const void *from, __kernel_size_t n)
	 *
	 * Returns the number of bytes not copied. Might be off by
	 * max 3 bytes if we get a fault in the main loop.
	 *
	 * The address-space checking functions simply fall through to
	 * the non-checking version.
	 */
	.text
	.align	1
	.global	copy_from_user
	.type	copy_from_user, @function
copy_from_user:
	branch_if_kernel r8, __copy_user
	ret_if_privileged r8, r11, r10, r10
	rjmp	__copy_user
	.size	copy_from_user, . - copy_from_user

	.global	copy_to_user
	.type	copy_to_user, @function
copy_to_user:
	branch_if_kernel r8, __copy_user
	ret_if_privileged r8, r12, r10, r10
	.size	copy_to_user, . - copy_to_user

	.global	__copy_user
	.type	__copy_user, @function
__copy_user:
	mov	r9, r11
	andl	r9, 3, COH
	brne	6f

	/* At this point, from is word-aligned */
1:	sub	r10, 4
	brlt	3f

2:
10:	ld.w	r8, r11++
11:	st.w	r12++, r8
	sub	r10, 4
	brge	2b

3:	sub	r10, -4
	reteq	0

	/*
	 * Handle unaligned count. Need to be careful with r10 here so
	 * that we return the correct value even if we get a fault
	 */
4:
20:	ld.ub	r8, r11++
21:	st.b	r12++, r8
	sub	r10, 1
	reteq	0
22:	ld.ub	r8, r11++
23:	st.b	r12++, r8
	sub	r10, 1
	reteq	0
24:	ld.ub	r8, r11++
25:	st.b	r12++, r8
	retal	0

	/* Handle unaligned from-pointer */
6:	cp.w	r10, 4
	brlt	4b
	rsub	r9, r9, 4

30:	ld.ub	r8, r11++
31:	st.b	r12++, r8
	sub	r10, 1
	sub	r9, 1
	breq	1b
32:	ld.ub	r8, r11++
33:	st.b	r12++, r8
	sub	r10, 1
	sub	r9, 1
	breq	1b
34:	ld.ub	r8, r11++
35:	st.b	r12++, r8
	sub	r10, 1
	rjmp	1b
	.size	__copy_user, . - __copy_user

	.section .fixup,"ax"
	.align	1
19:	sub	r10, -4
29:	retal	r10

	.section __ex_table,"a"
	.align	2
	.long	10b, 19b
	.long	11b, 19b
	.long	20b, 29b
	.long	21b, 29b
	.long	22b, 29b
	.long	23b, 29b
	.long	24b, 29b
	.long	25b, 29b
	.long	30b, 29b
	.long	31b, 29b
	.long	32b, 29b
	.long	33b, 29b
	.long	34b, 29b
	.long	35b, 29b
