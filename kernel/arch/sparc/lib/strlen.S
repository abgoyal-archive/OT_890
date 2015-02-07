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

/* strlen.S: Sparc optimized strlen code
 * Hand optimized from GNU libc's strlen
 * Copyright (C) 1991,1996 Free Software Foundation
 * Copyright (C) 1996,2008 David S. Miller (davem@davemloft.net)
 * Copyright (C) 1996, 1997 Jakub Jelinek (jj@sunsite.mff.cuni.cz)
 */

#include <linux/linkage.h>
#include <asm/asm.h>

#define LO_MAGIC 0x01010101
#define HI_MAGIC 0x80808080

	.text
ENTRY(strlen)
	mov	%o0, %o1
	andcc	%o0, 3, %g0
	BRANCH32(be, pt, 9f)
	 sethi	%hi(HI_MAGIC), %o4
	ldub	[%o0], %o5
	BRANCH_REG_ZERO(pn, %o5, 11f)
	 add	%o0, 1, %o0
	andcc	%o0, 3, %g0
	BRANCH32(be, pn, 4f)
	 or	%o4, %lo(HI_MAGIC), %o3
	ldub	[%o0], %o5
	BRANCH_REG_ZERO(pn, %o5, 12f)
	 add	%o0, 1, %o0
	andcc	%o0, 3, %g0
	BRANCH32(be, pt, 5f)
	 sethi	%hi(LO_MAGIC), %o4
	ldub	[%o0], %o5
	BRANCH_REG_ZERO(pn, %o5, 13f)
	 add	%o0, 1, %o0
	BRANCH32(ba, pt, 8f)
	 or	%o4, %lo(LO_MAGIC), %o2
9:
	or	%o4, %lo(HI_MAGIC), %o3
4:
	sethi	%hi(LO_MAGIC), %o4
5:
	or	%o4, %lo(LO_MAGIC), %o2
8:
	ld	[%o0], %o5
2:
	sub	%o5, %o2, %o4
	andcc	%o4, %o3, %g0
	BRANCH32(be, pt, 8b)
	 add	%o0, 4, %o0

	/* Check every byte. */
	srl	%o5, 24, %g7
	andcc	%g7, 0xff, %g0
	BRANCH32(be, pn, 1f)
	 add	%o0, -4, %o4
	srl	%o5, 16, %g7
	andcc	%g7, 0xff, %g0
	BRANCH32(be, pn, 1f)
	 add	%o4, 1, %o4
	srl	%o5, 8, %g7
	andcc	%g7, 0xff, %g0
	BRANCH32(be, pn, 1f)
	 add	%o4, 1, %o4
	andcc	%o5, 0xff, %g0
	BRANCH32_ANNUL(bne, pt, 2b)
	 ld	[%o0], %o5
	add	%o4, 1, %o4
1:
	retl
	 sub	%o4, %o1, %o0
11:
	retl
	 mov	0, %o0
12:
	retl
	 mov	1, %o0
13:
	retl
	 mov	2, %o0
ENDPROC(strlen)
