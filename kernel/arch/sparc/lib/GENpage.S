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

/* GENpage.S: Generic clear and copy page.
 *
 * Copyright (C) 2007 (davem@davemloft.net)
 */
#include <asm/page.h>

	.text
	.align	32

GENcopy_user_page:
	set	PAGE_SIZE, %g7
1:	ldx	[%o1 + 0x00], %o2
	ldx	[%o1 + 0x08], %o3
	ldx	[%o1 + 0x10], %o4
	ldx	[%o1 + 0x18], %o5
	stx	%o2, [%o0 + 0x00]
	stx	%o3, [%o0 + 0x08]
	stx	%o4, [%o0 + 0x10]
	stx	%o5, [%o0 + 0x18]
	ldx	[%o1 + 0x20], %o2
	ldx	[%o1 + 0x28], %o3
	ldx	[%o1 + 0x30], %o4
	ldx	[%o1 + 0x38], %o5
	stx	%o2, [%o0 + 0x20]
	stx	%o3, [%o0 + 0x28]
	stx	%o4, [%o0 + 0x30]
	stx	%o5, [%o0 + 0x38]
	subcc	%g7, 64, %g7
	add	%o1, 64, %o1
	bne,pt	%xcc, 1b
	 add	%o0, 64, %o0
	retl
	 nop

GENclear_page:
GENclear_user_page:
	set	PAGE_SIZE, %g7
1:	stx	%g0, [%o0 + 0x00]
	stx	%g0, [%o0 + 0x08]
	stx	%g0, [%o0 + 0x10]
	stx	%g0, [%o0 + 0x18]
	stx	%g0, [%o0 + 0x20]
	stx	%g0, [%o0 + 0x28]
	stx	%g0, [%o0 + 0x30]
	stx	%g0, [%o0 + 0x38]
	subcc	%g7, 64, %g7
	bne,pt	%xcc, 1b
	 add	%o0, 64, %o0

#define BRANCH_ALWAYS	0x10680000
#define NOP		0x01000000
#define GEN_DO_PATCH(OLD, NEW)	\
	sethi	%hi(NEW), %g1; \
	or	%g1, %lo(NEW), %g1; \
	sethi	%hi(OLD), %g2; \
	or	%g2, %lo(OLD), %g2; \
	sub	%g1, %g2, %g1; \
	sethi	%hi(BRANCH_ALWAYS), %g3; \
	sll	%g1, 11, %g1; \
	srl	%g1, 11 + 2, %g1; \
	or	%g3, %lo(BRANCH_ALWAYS), %g3; \
	or	%g3, %g1, %g3; \
	stw	%g3, [%g2]; \
	sethi	%hi(NOP), %g3; \
	or	%g3, %lo(NOP), %g3; \
	stw	%g3, [%g2 + 0x4]; \
	flush	%g2;

	.globl	generic_patch_pageops
	.type	generic_patch_pageops,#function
generic_patch_pageops:
	GEN_DO_PATCH(copy_user_page, GENcopy_user_page)
	GEN_DO_PATCH(_clear_page, GENclear_page)
	GEN_DO_PATCH(clear_user_page, GENclear_user_page)
	retl
	 nop
	.size	generic_patch_pageops,.-generic_patch_pageops
