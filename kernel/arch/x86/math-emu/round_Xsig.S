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

/*---------------------------------------------------------------------------+
 |  round_Xsig.S                                                             |
 |                                                                           |
 | Copyright (C) 1992,1993,1994,1995                                         |
 |                       W. Metzenthen, 22 Parker St, Ormond, Vic 3163,      |
 |                       Australia.  E-mail billm@jacobi.maths.monash.edu.au |
 |                                                                           |
 | Normalize and round a 12 byte quantity.                                   |
 | Call from C as:                                                           |
 |   int round_Xsig(Xsig *n)                                                 |
 |                                                                           |
 | Normalize a 12 byte quantity.                                             |
 | Call from C as:                                                           |
 |   int norm_Xsig(Xsig *n)                                                  |
 |                                                                           |
 | Each function returns the size of the shift (nr of bits).                 |
 |                                                                           |
 +---------------------------------------------------------------------------*/
	.file	"round_Xsig.S"

#include "fpu_emu.h"


.text
ENTRY(round_Xsig)
	pushl	%ebp
	movl	%esp,%ebp
	pushl	%ebx		/* Reserve some space */
	pushl	%ebx
	pushl	%esi

	movl	PARAM1,%esi

	movl	8(%esi),%edx
	movl	4(%esi),%ebx
	movl	(%esi),%eax

	movl	$0,-4(%ebp)

	orl	%edx,%edx	/* ms bits */
	js	L_round		/* Already normalized */
	jnz	L_shift_1	/* Shift left 1 - 31 bits */

	movl	%ebx,%edx
	movl	%eax,%ebx
	xorl	%eax,%eax
	movl	$-32,-4(%ebp)

/* We need to shift left by 1 - 31 bits */
L_shift_1:
	bsrl	%edx,%ecx	/* get the required shift in %ecx */
	subl	$31,%ecx
	negl	%ecx
	subl	%ecx,-4(%ebp)
	shld	%cl,%ebx,%edx
	shld	%cl,%eax,%ebx
	shl	%cl,%eax

L_round:
	testl	$0x80000000,%eax
	jz	L_exit

	addl	$1,%ebx
	adcl	$0,%edx
	jnz	L_exit

	movl	$0x80000000,%edx
	incl	-4(%ebp)

L_exit:
	movl	%edx,8(%esi)
	movl	%ebx,4(%esi)
	movl	%eax,(%esi)

	movl	-4(%ebp),%eax

	popl	%esi
	popl	%ebx
	leave
	ret




ENTRY(norm_Xsig)
	pushl	%ebp
	movl	%esp,%ebp
	pushl	%ebx		/* Reserve some space */
	pushl	%ebx
	pushl	%esi

	movl	PARAM1,%esi

	movl	8(%esi),%edx
	movl	4(%esi),%ebx
	movl	(%esi),%eax

	movl	$0,-4(%ebp)

	orl	%edx,%edx	/* ms bits */
	js	L_n_exit		/* Already normalized */
	jnz	L_n_shift_1	/* Shift left 1 - 31 bits */

	movl	%ebx,%edx
	movl	%eax,%ebx
	xorl	%eax,%eax
	movl	$-32,-4(%ebp)

	orl	%edx,%edx	/* ms bits */
	js	L_n_exit	/* Normalized now */
	jnz	L_n_shift_1	/* Shift left 1 - 31 bits */

	movl	%ebx,%edx
	movl	%eax,%ebx
	xorl	%eax,%eax
	addl	$-32,-4(%ebp)
	jmp	L_n_exit	/* Might not be normalized,
	                           but shift no more. */

/* We need to shift left by 1 - 31 bits */
L_n_shift_1:
	bsrl	%edx,%ecx	/* get the required shift in %ecx */
	subl	$31,%ecx
	negl	%ecx
	subl	%ecx,-4(%ebp)
	shld	%cl,%ebx,%edx
	shld	%cl,%eax,%ebx
	shl	%cl,%eax

L_n_exit:
	movl	%edx,8(%esi)
	movl	%ebx,4(%esi)
	movl	%eax,(%esi)

	movl	-4(%ebp),%eax

	popl	%esi
	popl	%ebx
	leave
	ret

