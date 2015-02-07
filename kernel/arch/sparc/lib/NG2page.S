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

/* NG2page.S: Niagara-2 optimized clear and copy page.
 *
 * Copyright (C) 2007 (davem@davemloft.net)
 */

#include <asm/asi.h>
#include <asm/page.h>
#include <asm/visasm.h>

	.text
	.align	32

	/* This is heavily simplified from the sun4u variants
	 * because Niagara-2 does not have any D-cache aliasing issues.
	 */
NG2copy_user_page:	/* %o0=dest, %o1=src, %o2=vaddr */
	prefetch	[%o1 + 0x00], #one_read
	prefetch	[%o1 + 0x40], #one_read
	VISEntryHalf
	set		PAGE_SIZE, %g7
	sub		%o0, %o1, %g3
1:	stxa		%g0, [%o1 + %g3] ASI_BLK_INIT_QUAD_LDD_P
	subcc		%g7, 64, %g7
	ldda		[%o1] ASI_BLK_P, %f0
	stda		%f0, [%o1 + %g3] ASI_BLK_P
	add		%o1, 64, %o1
	bne,pt		%xcc, 1b
	 prefetch	[%o1 + 0x40], #one_read
	membar		#Sync
	VISExitHalf
	retl
	 nop

#define BRANCH_ALWAYS	0x10680000
#define NOP		0x01000000
#define NG_DO_PATCH(OLD, NEW)	\
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

	.globl	niagara2_patch_pageops
	.type	niagara2_patch_pageops,#function
niagara2_patch_pageops:
	NG_DO_PATCH(copy_user_page, NG2copy_user_page)
	NG_DO_PATCH(_clear_page, NGclear_page)
	NG_DO_PATCH(clear_user_page, NGclear_user_page)
	retl
	 nop
	.size	niagara2_patch_pageops,.-niagara2_patch_pageops
