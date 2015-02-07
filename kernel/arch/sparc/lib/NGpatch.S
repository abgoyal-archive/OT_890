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

/* NGpatch.S: Patch Ultra-I routines with Niagara variant.
 *
 * Copyright (C) 2006 David S. Miller <davem@davemloft.net>
 */

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

	.globl	niagara_patch_copyops
	.type	niagara_patch_copyops,#function
niagara_patch_copyops:
	NG_DO_PATCH(memcpy, NGmemcpy)
	NG_DO_PATCH(___copy_from_user, NGcopy_from_user)
	NG_DO_PATCH(___copy_to_user, NGcopy_to_user)
	retl
	 nop
	.size	niagara_patch_copyops,.-niagara_patch_copyops
