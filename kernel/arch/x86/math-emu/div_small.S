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

	.file	"div_small.S"
/*---------------------------------------------------------------------------+
 |  div_small.S                                                              |
 |                                                                           |
 | Divide a 64 bit integer by a 32 bit integer & return remainder.           |
 |                                                                           |
 | Copyright (C) 1992,1995                                                   |
 |                       W. Metzenthen, 22 Parker St, Ormond, Vic 3163,      |
 |                       Australia.  E-mail billm@jacobi.maths.monash.edu.au |
 |                                                                           |
 |                                                                           |
 +---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------+
 |    unsigned long FPU_div_small(unsigned long long *x, unsigned long y)    |
 +---------------------------------------------------------------------------*/

#include "fpu_emu.h"

.text
ENTRY(FPU_div_small)
	pushl	%ebp
	movl	%esp,%ebp

	pushl	%esi

	movl	PARAM1,%esi	/* pointer to num */
	movl	PARAM2,%ecx	/* The denominator */

	movl	4(%esi),%eax	/* Get the current num msw */
	xorl	%edx,%edx
	divl	%ecx

	movl	%eax,4(%esi)

	movl	(%esi),%eax	/* Get the num lsw */
	divl	%ecx

	movl	%eax,(%esi)

	movl	%edx,%eax	/* Return the remainder in eax */

	popl	%esi

	leave
	ret

