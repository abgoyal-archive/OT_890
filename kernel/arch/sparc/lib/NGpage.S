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

/* NGpage.S: Niagara optimize clear and copy page.
 *
 * Copyright (C) 2006 (davem@davemloft.net)
 */

#include <asm/asi.h>
#include <asm/page.h>

	.text
	.align	32

	/* This is heavily simplified from the sun4u variants
	 * because Niagara does not have any D-cache aliasing issues
	 * and also we don't need to use the FPU in order to implement
	 * an optimal page copy/clear.
	 */

NGcopy_user_page:	/* %o0=dest, %o1=src, %o2=vaddr */
	prefetch	[%o1 + 0x00], #one_read
	mov		8, %g1
	mov		16, %g2
	mov		24, %g3
	set		PAGE_SIZE, %g7

1:	ldda		[%o1 + %g0] ASI_BLK_INIT_QUAD_LDD_P, %o2
	ldda		[%o1 + %g2] ASI_BLK_INIT_QUAD_LDD_P, %o4
	prefetch	[%o1 + 0x40], #one_read
	add		%o1, 32, %o1
	stxa		%o2, [%o0 + %g0] ASI_BLK_INIT_QUAD_LDD_P
	stxa		%o3, [%o0 + %g1] ASI_BLK_INIT_QUAD_LDD_P
	ldda		[%o1 + %g0] ASI_BLK_INIT_QUAD_LDD_P, %o2
	stxa		%o4, [%o0 + %g2] ASI_BLK_INIT_QUAD_LDD_P
	stxa		%o5, [%o0 + %g3] ASI_BLK_INIT_QUAD_LDD_P
	ldda		[%o1 + %g2] ASI_BLK_INIT_QUAD_LDD_P, %o4
	add		%o1, 32, %o1
	add		%o0, 32, %o0
	stxa		%o2, [%o0 + %g0] ASI_BLK_INIT_QUAD_LDD_P
	stxa		%o3, [%o0 + %g1] ASI_BLK_INIT_QUAD_LDD_P
	stxa		%o4, [%o0 + %g2] ASI_BLK_INIT_QUAD_LDD_P
	stxa		%o5, [%o0 + %g3] ASI_BLK_INIT_QUAD_LDD_P
	subcc		%g7, 64, %g7
	bne,pt		%xcc, 1b
	 add		%o0, 32, %o0
	membar		#Sync
	retl
	 nop

	.globl		NGclear_page, NGclear_user_page
NGclear_page:		/* %o0=dest */
NGclear_user_page:	/* %o0=dest, %o1=vaddr */
	mov		8, %g1
	mov		16, %g2
	mov		24, %g3
	set		PAGE_SIZE, %g7

1:	stxa		%g0, [%o0 + %g0] ASI_BLK_INIT_QUAD_LDD_P
	stxa		%g0, [%o0 + %g1] ASI_BLK_INIT_QUAD_LDD_P
	stxa		%g0, [%o0 + %g2] ASI_BLK_INIT_QUAD_LDD_P
	stxa		%g0, [%o0 + %g3] ASI_BLK_INIT_QUAD_LDD_P
	add		%o0, 32, %o0
	stxa		%g0, [%o0 + %g0] ASI_BLK_INIT_QUAD_LDD_P
	stxa		%g0, [%o0 + %g1] ASI_BLK_INIT_QUAD_LDD_P
	stxa		%g0, [%o0 + %g2] ASI_BLK_INIT_QUAD_LDD_P
	stxa		%g0, [%o0 + %g3] ASI_BLK_INIT_QUAD_LDD_P
	subcc		%g7, 64, %g7
	bne,pt		%xcc, 1b
	 add		%o0, 32, %o0
	membar		#Sync
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

	.globl	niagara_patch_pageops
	.type	niagara_patch_pageops,#function
niagara_patch_pageops:
	NG_DO_PATCH(copy_user_page, NGcopy_user_page)
	NG_DO_PATCH(_clear_page, NGclear_page)
	NG_DO_PATCH(clear_user_page, NGclear_user_page)
	retl
	 nop
	.size	niagara_patch_pageops,.-niagara_patch_pageops
