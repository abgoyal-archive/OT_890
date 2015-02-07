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

	/*
	 * void *memcpy(void *to, const void *from, unsigned long n)
	 *
	 * This implementation does word-aligned loads in the main loop,
	 * possibly sacrificing alignment of stores.
	 *
	 * Hopefully, in most cases, both "to" and "from" will be
	 * word-aligned to begin with.
	 */
	.text
	.global	memcpy
	.type	memcpy, @function
memcpy:
	mov	r9, r11
	andl	r9, 3, COH
	brne	1f

	/* At this point, "from" is word-aligned */
2:	sub	r10, 4
	mov	r9, r12
	brlt	4f

3:	ld.w	r8, r11++
	sub	r10, 4
	st.w	r12++, r8
	brge	3b

4:	neg	r10
	reteq	r9

	/* Handle unaligned count */
	lsl	r10, 2
	add	pc, pc, r10
	ld.ub	r8, r11++
	st.b	r12++, r8
	ld.ub	r8, r11++
	st.b	r12++, r8
	ld.ub	r8, r11++
	st.b	r12++, r8
	retal	r9

	/* Handle unaligned "from" pointer */
1:	sub	r10, 4
	brlt	4b
	add	r10, r9
	lsl	r9, 2
	add	pc, pc, r9
	ld.ub	r8, r11++
	st.b	r12++, r8
	ld.ub	r8, r11++
	st.b	r12++, r8
	ld.ub	r8, r11++
	st.b	r12++, r8
	rjmp	2b
