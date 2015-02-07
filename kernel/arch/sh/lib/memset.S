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

/* $Id: memset.S,v 1.1 2000/04/14 16:49:01 mjd Exp $
 *
 * "memset" implementation of SuperH
 *
 * Copyright (C) 1999  Niibe Yutaka
 *
 */

/*
 *            void *memset(void *s, int c, size_t n);
 */

#include <linux/linkage.h>

ENTRY(memset)
	tst	r6,r6
	bt/s	5f		! if n=0, do nothing
	 add	r6,r4
	mov	#12,r0
	cmp/gt	r6,r0
	bt/s	4f		! if it's too small, set a byte at once
	 mov	r4,r0
	and	#3,r0
	cmp/eq	#0,r0
	bt/s	2f		! It's aligned
	 sub	r0,r6
1:
	dt	r0
	bf/s	1b
	 mov.b	r5,@-r4
2:				! make VVVV
	extu.b	r5,r5
	swap.b	r5,r0		!   V0
	or	r0,r5		!   VV
	swap.w	r5,r0		! VV00
	or	r0,r5		! VVVV
	!
	mov	r6,r0
	shlr2	r0
	shlr	r0		! r0 = r6 >> 3
3:
	dt	r0
	mov.l	r5,@-r4		! set 8-byte at once
	bf/s	3b
	 mov.l	r5,@-r4
	!
	mov	#7,r0
	and	r0,r6
	tst	r6,r6
	bt	5f
	! fill bytes
4:
	dt	r6
	bf/s	4b
	 mov.b	r5,@-r4
5:
	rts
	 mov	r4,r0
