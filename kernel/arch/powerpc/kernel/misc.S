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

/*
 * This file contains miscellaneous low-level functions.
 *    Copyright (C) 1995-1996 Gary Thomas (gdt@linuxppc.org)
 *
 * Largely rewritten by Cort Dougan (cort@cs.nmt.edu)
 * and Paul Mackerras.
 *
 * Adapted for iSeries by Mike Corrigan (mikejc@us.ibm.com)
 * PPC64 updates by Dave Engebretsen (engebret@us.ibm.com)
 *
 * setjmp/longjmp code by Paul Mackerras.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version
 * 2 of the License, or (at your option) any later version.
 */
#include <asm/ppc_asm.h>
#include <asm/unistd.h>
#include <asm/asm-compat.h>
#include <asm/asm-offsets.h>

	.text

/*
 * Returns (address we are running at) - (address we were linked at)
 * for use before the text and data are mapped to KERNELBASE.
 */

_GLOBAL(reloc_offset)
	mflr	r0
	bl	1f
1:	mflr	r3
	PPC_LL	r4,(2f-1b)(r3)
	subf	r3,r4,r3
	mtlr	r0
	blr

	.align	3
2:	PPC_LONG 1b

/*
 * add_reloc_offset(x) returns x + reloc_offset().
 */
_GLOBAL(add_reloc_offset)
	mflr	r0
	bl	1f
1:	mflr	r5
	PPC_LL	r4,(2f-1b)(r5)
	subf	r5,r4,r5
	add	r3,r3,r5
	mtlr	r0
	blr

	.align	3
2:	PPC_LONG 1b

_GLOBAL(kernel_execve)
	li	r0,__NR_execve
	sc
	bnslr
	neg	r3,r3
	blr

_GLOBAL(setjmp)
	mflr	r0
	PPC_STL	r0,0(r3)
	PPC_STL	r1,SZL(r3)
	PPC_STL	r2,2*SZL(r3)
	mfcr	r0
	PPC_STL	r0,3*SZL(r3)
	PPC_STL	r13,4*SZL(r3)
	PPC_STL	r14,5*SZL(r3)
	PPC_STL	r15,6*SZL(r3)
	PPC_STL	r16,7*SZL(r3)
	PPC_STL	r17,8*SZL(r3)
	PPC_STL	r18,9*SZL(r3)
	PPC_STL	r19,10*SZL(r3)
	PPC_STL	r20,11*SZL(r3)
	PPC_STL	r21,12*SZL(r3)
	PPC_STL	r22,13*SZL(r3)
	PPC_STL	r23,14*SZL(r3)
	PPC_STL	r24,15*SZL(r3)
	PPC_STL	r25,16*SZL(r3)
	PPC_STL	r26,17*SZL(r3)
	PPC_STL	r27,18*SZL(r3)
	PPC_STL	r28,19*SZL(r3)
	PPC_STL	r29,20*SZL(r3)
	PPC_STL	r30,21*SZL(r3)
	PPC_STL	r31,22*SZL(r3)
	li	r3,0
	blr

_GLOBAL(longjmp)
	PPC_LCMPI r4,0
	bne	1f
	li	r4,1
1:	PPC_LL	r13,4*SZL(r3)
	PPC_LL	r14,5*SZL(r3)
	PPC_LL	r15,6*SZL(r3)
	PPC_LL	r16,7*SZL(r3)
	PPC_LL	r17,8*SZL(r3)
	PPC_LL	r18,9*SZL(r3)
	PPC_LL	r19,10*SZL(r3)
	PPC_LL	r20,11*SZL(r3)
	PPC_LL	r21,12*SZL(r3)
	PPC_LL	r22,13*SZL(r3)
	PPC_LL	r23,14*SZL(r3)
	PPC_LL	r24,15*SZL(r3)
	PPC_LL	r25,16*SZL(r3)
	PPC_LL	r26,17*SZL(r3)
	PPC_LL	r27,18*SZL(r3)
	PPC_LL	r28,19*SZL(r3)
	PPC_LL	r29,20*SZL(r3)
	PPC_LL	r30,21*SZL(r3)
	PPC_LL	r31,22*SZL(r3)
	PPC_LL	r0,3*SZL(r3)
	mtcrf	0x38,r0
	PPC_LL	r0,0(r3)
	PPC_LL	r1,SZL(r3)
	PPC_LL	r2,2*SZL(r3)
	mtlr	r0
	mr	r3,r4
	blr

_GLOBAL(__setup_cpu_power7)
_GLOBAL(__restore_cpu_power7)
	/* place holder */
	blr
