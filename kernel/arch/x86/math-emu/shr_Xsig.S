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

	.file	"shr_Xsig.S"
/*---------------------------------------------------------------------------+
 |  shr_Xsig.S                                                               |
 |                                                                           |
 | 12 byte right shift function                                              |
 |                                                                           |
 | Copyright (C) 1992,1994,1995                                              |
 |                       W. Metzenthen, 22 Parker St, Ormond, Vic 3163,      |
 |                       Australia.  E-mail billm@jacobi.maths.monash.edu.au |
 |                                                                           |
 | Call from C as:                                                           |
 |   void shr_Xsig(Xsig *arg, unsigned nr)                                   |
 |                                                                           |
 |   Extended shift right function.                                          |
 |   Fastest for small shifts.                                               |
 |   Shifts the 12 byte quantity pointed to by the first arg (arg)           |
 |   right by the number of bits specified by the second arg (nr).           |
 |                                                                           |
 +---------------------------------------------------------------------------*/

#include "fpu_emu.h"

.text
ENTRY(shr_Xsig)
	push	%ebp
	movl	%esp,%ebp
	pushl	%esi
	movl	PARAM2,%ecx
	movl	PARAM1,%esi
	cmpl	$32,%ecx	/* shrd only works for 0..31 bits */
	jnc	L_more_than_31

/* less than 32 bits */
	pushl	%ebx
	movl	(%esi),%eax	/* lsl */
	movl	4(%esi),%ebx	/* midl */
	movl	8(%esi),%edx	/* msl */
	shrd	%cl,%ebx,%eax
	shrd	%cl,%edx,%ebx
	shr	%cl,%edx
	movl	%eax,(%esi)
	movl	%ebx,4(%esi)
	movl	%edx,8(%esi)
	popl	%ebx
	popl	%esi
	leave
	ret

L_more_than_31:
	cmpl	$64,%ecx
	jnc	L_more_than_63

	subb	$32,%cl
	movl	4(%esi),%eax	/* midl */
	movl	8(%esi),%edx	/* msl */
	shrd	%cl,%edx,%eax
	shr	%cl,%edx
	movl	%eax,(%esi)
	movl	%edx,4(%esi)
	movl	$0,8(%esi)
	popl	%esi
	leave
	ret

L_more_than_63:
	cmpl	$96,%ecx
	jnc	L_more_than_95

	subb	$64,%cl
	movl	8(%esi),%eax	/* msl */
	shr	%cl,%eax
	xorl	%edx,%edx
	movl	%eax,(%esi)
	movl	%edx,4(%esi)
	movl	%edx,8(%esi)
	popl	%esi
	leave
	ret

L_more_than_95:
	xorl	%eax,%eax
	movl	%eax,(%esi)
	movl	%eax,4(%esi)
	movl	%eax,8(%esi)
	popl	%esi
	leave
	ret
