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

/* $Id: strlen.S,v 1.2 2001/06/29 14:07:15 gniibe Exp $
 *
 * "strlen" implementation of SuperH
 *
 * Copyright (C) 1999  Kaz Kojima
 *
 */

/* size_t strlen (const char *s)  */

#include <linux/linkage.h>
ENTRY(strlen)
	mov	r4,r0
	and	#3,r0
	tst	r0,r0
	bt/s	1f
	 mov	#0,r2

	add	#-1,r0
	shll2	r0
	shll	r0
	braf	r0
	 nop

	mov.b	@r4+,r1
	tst	r1,r1
	bt	8f
	add	#1,r2

	mov.b	@r4+,r1
	tst	r1,r1
	bt	8f
	add	#1,r2

	mov.b	@r4+,r1
	tst	r1,r1
	bt	8f
	add	#1,r2	

1:
	mov	#0,r3
2:
	mov.l	@r4+,r1
	cmp/str	r3,r1
	bf/s	2b
	 add	#4,r2

	add	#-4,r2
#ifndef __LITTLE_ENDIAN__
	swap.b	r1,r1
	swap.w	r1,r1
	swap.b	r1,r1
#endif
	extu.b	r1,r0
	tst	r0,r0
	bt/s	8f
	 shlr8	r1
	add	#1,r2
	extu.b	r1,r0
	tst	r0,r0
	bt/s	8f
	 shlr8	r1
	add	#1,r2
	extu.b	r1,r0
	tst	r0,r0
	bt	8f
	add	#1,r2
8:
	rts
	 mov	r2,r0
