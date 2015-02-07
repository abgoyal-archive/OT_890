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
 |  reg_norm.S                                                               |
 |                                                                           |
 | Copyright (C) 1992,1993,1994,1995,1997                                    |
 |                       W. Metzenthen, 22 Parker St, Ormond, Vic 3163,      |
 |                       Australia.  E-mail billm@suburbia.net               |
 |                                                                           |
 | Normalize the value in a FPU_REG.                                         |
 |                                                                           |
 | Call from C as:                                                           |
 |    int FPU_normalize(FPU_REG *n)                                          |
 |                                                                           |
 |    int FPU_normalize_nuo(FPU_REG *n)                                      |
 |                                                                           |
 |    Return value is the tag of the answer, or-ed with FPU_Exception if     |
 |    one was raised, or -1 on internal error.                               |
 |                                                                           |
 +---------------------------------------------------------------------------*/

#include "fpu_emu.h"


.text
ENTRY(FPU_normalize)
	pushl	%ebp
	movl	%esp,%ebp
	pushl	%ebx

	movl	PARAM1,%ebx

	movl	SIGH(%ebx),%edx
	movl	SIGL(%ebx),%eax

	orl	%edx,%edx	/* ms bits */
	js	L_done		/* Already normalized */
	jnz	L_shift_1	/* Shift left 1 - 31 bits */

	orl	%eax,%eax
	jz	L_zero		/* The contents are zero */

	movl	%eax,%edx
	xorl	%eax,%eax
	subw	$32,EXP(%ebx)	/* This can cause an underflow */

/* We need to shift left by 1 - 31 bits */
L_shift_1:
	bsrl	%edx,%ecx	/* get the required shift in %ecx */
	subl	$31,%ecx
	negl	%ecx
	shld	%cl,%eax,%edx
	shl	%cl,%eax
	subw	%cx,EXP(%ebx)	/* This can cause an underflow */

	movl	%edx,SIGH(%ebx)
	movl	%eax,SIGL(%ebx)

L_done:
	cmpw	EXP_OVER,EXP(%ebx)
	jge	L_overflow

	cmpw	EXP_UNDER,EXP(%ebx)
	jle	L_underflow

L_exit_valid:
	movl	TAG_Valid,%eax

	/* Convert the exponent to 80x87 form. */
	addw	EXTENDED_Ebias,EXP(%ebx)
	andw	$0x7fff,EXP(%ebx)

L_exit:
	popl	%ebx
	leave
	ret


L_zero:
	movw	$0,EXP(%ebx)
	movl	TAG_Zero,%eax
	jmp	L_exit

L_underflow:
	/* Convert the exponent to 80x87 form. */
	addw	EXTENDED_Ebias,EXP(%ebx)
	push	%ebx
	call	arith_underflow
	pop	%ebx
	jmp	L_exit

L_overflow:
	/* Convert the exponent to 80x87 form. */
	addw	EXTENDED_Ebias,EXP(%ebx)
	push	%ebx
	call	arith_overflow
	pop	%ebx
	jmp	L_exit



/* Normalise without reporting underflow or overflow */
ENTRY(FPU_normalize_nuo)
	pushl	%ebp
	movl	%esp,%ebp
	pushl	%ebx

	movl	PARAM1,%ebx

	movl	SIGH(%ebx),%edx
	movl	SIGL(%ebx),%eax

	orl	%edx,%edx	/* ms bits */
	js	L_exit_nuo_valid	/* Already normalized */
	jnz	L_nuo_shift_1	/* Shift left 1 - 31 bits */

	orl	%eax,%eax
	jz	L_exit_nuo_zero		/* The contents are zero */

	movl	%eax,%edx
	xorl	%eax,%eax
	subw	$32,EXP(%ebx)	/* This can cause an underflow */

/* We need to shift left by 1 - 31 bits */
L_nuo_shift_1:
	bsrl	%edx,%ecx	/* get the required shift in %ecx */
	subl	$31,%ecx
	negl	%ecx
	shld	%cl,%eax,%edx
	shl	%cl,%eax
	subw	%cx,EXP(%ebx)	/* This can cause an underflow */

	movl	%edx,SIGH(%ebx)
	movl	%eax,SIGL(%ebx)

L_exit_nuo_valid:
	movl	TAG_Valid,%eax

	popl	%ebx
	leave
	ret

L_exit_nuo_zero:
	movl	TAG_Zero,%eax
	movw	EXP_UNDER,EXP(%ebx)

	popl	%ebx
	leave
	ret
