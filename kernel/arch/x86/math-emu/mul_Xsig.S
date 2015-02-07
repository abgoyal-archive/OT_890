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
 |  mul_Xsig.S                                                               |
 |                                                                           |
 | Multiply a 12 byte fixed point number by another fixed point number.      |
 |                                                                           |
 | Copyright (C) 1992,1994,1995                                              |
 |                       W. Metzenthen, 22 Parker St, Ormond, Vic 3163,      |
 |                       Australia.  E-mail billm@jacobi.maths.monash.edu.au |
 |                                                                           |
 | Call from C as:                                                           |
 |   void mul32_Xsig(Xsig *x, unsigned b)                                    |
 |                                                                           |
 |   void mul64_Xsig(Xsig *x, unsigned long long *b)                         |
 |                                                                           |
 |   void mul_Xsig_Xsig(Xsig *x, unsigned *b)                                |
 |                                                                           |
 | The result is neither rounded nor normalized, and the ls bit or so may    |
 | be wrong.                                                                 |
 |                                                                           |
 +---------------------------------------------------------------------------*/
	.file	"mul_Xsig.S"


#include "fpu_emu.h"

.text
ENTRY(mul32_Xsig)
	pushl %ebp
	movl %esp,%ebp
	subl $16,%esp
	pushl %esi

	movl PARAM1,%esi
	movl PARAM2,%ecx

	xor %eax,%eax
	movl %eax,-4(%ebp)
	movl %eax,-8(%ebp)

	movl (%esi),%eax        /* lsl of Xsig */
	mull %ecx		/* msl of b */
	movl %edx,-12(%ebp)

	movl 4(%esi),%eax	/* midl of Xsig */
	mull %ecx		/* msl of b */
	addl %eax,-12(%ebp)
	adcl %edx,-8(%ebp)
	adcl $0,-4(%ebp)

	movl 8(%esi),%eax	/* msl of Xsig */
	mull %ecx		/* msl of b */
	addl %eax,-8(%ebp)
	adcl %edx,-4(%ebp)

	movl -12(%ebp),%eax
	movl %eax,(%esi)
	movl -8(%ebp),%eax
	movl %eax,4(%esi)
	movl -4(%ebp),%eax
	movl %eax,8(%esi)

	popl %esi
	leave
	ret


ENTRY(mul64_Xsig)
	pushl %ebp
	movl %esp,%ebp
	subl $16,%esp
	pushl %esi

	movl PARAM1,%esi
	movl PARAM2,%ecx

	xor %eax,%eax
	movl %eax,-4(%ebp)
	movl %eax,-8(%ebp)

	movl (%esi),%eax        /* lsl of Xsig */
	mull 4(%ecx)		/* msl of b */
	movl %edx,-12(%ebp)

	movl 4(%esi),%eax	/* midl of Xsig */
	mull (%ecx)		/* lsl of b */
	addl %edx,-12(%ebp)
	adcl $0,-8(%ebp)
	adcl $0,-4(%ebp)

	movl 4(%esi),%eax	/* midl of Xsig */
	mull 4(%ecx)		/* msl of b */
	addl %eax,-12(%ebp)
	adcl %edx,-8(%ebp)
	adcl $0,-4(%ebp)

	movl 8(%esi),%eax	/* msl of Xsig */
	mull (%ecx)		/* lsl of b */
	addl %eax,-12(%ebp)
	adcl %edx,-8(%ebp)
	adcl $0,-4(%ebp)

	movl 8(%esi),%eax	/* msl of Xsig */
	mull 4(%ecx)		/* msl of b */
	addl %eax,-8(%ebp)
	adcl %edx,-4(%ebp)

	movl -12(%ebp),%eax
	movl %eax,(%esi)
	movl -8(%ebp),%eax
	movl %eax,4(%esi)
	movl -4(%ebp),%eax
	movl %eax,8(%esi)

	popl %esi
	leave
	ret



ENTRY(mul_Xsig_Xsig)
	pushl %ebp
	movl %esp,%ebp
	subl $16,%esp
	pushl %esi

	movl PARAM1,%esi
	movl PARAM2,%ecx

	xor %eax,%eax
	movl %eax,-4(%ebp)
	movl %eax,-8(%ebp)

	movl (%esi),%eax        /* lsl of Xsig */
	mull 8(%ecx)		/* msl of b */
	movl %edx,-12(%ebp)

	movl 4(%esi),%eax	/* midl of Xsig */
	mull 4(%ecx)		/* midl of b */
	addl %edx,-12(%ebp)
	adcl $0,-8(%ebp)
	adcl $0,-4(%ebp)

	movl 8(%esi),%eax	/* msl of Xsig */
	mull (%ecx)		/* lsl of b */
	addl %edx,-12(%ebp)
	adcl $0,-8(%ebp)
	adcl $0,-4(%ebp)

	movl 4(%esi),%eax	/* midl of Xsig */
	mull 8(%ecx)		/* msl of b */
	addl %eax,-12(%ebp)
	adcl %edx,-8(%ebp)
	adcl $0,-4(%ebp)

	movl 8(%esi),%eax	/* msl of Xsig */
	mull 4(%ecx)		/* midl of b */
	addl %eax,-12(%ebp)
	adcl %edx,-8(%ebp)
	adcl $0,-4(%ebp)

	movl 8(%esi),%eax	/* msl of Xsig */
	mull 8(%ecx)		/* msl of b */
	addl %eax,-8(%ebp)
	adcl %edx,-4(%ebp)

	movl -12(%ebp),%edx
	movl %edx,(%esi)
	movl -8(%ebp),%edx
	movl %edx,4(%esi)
	movl -4(%ebp),%edx
	movl %edx,8(%esi)

	popl %esi
	leave
	ret

