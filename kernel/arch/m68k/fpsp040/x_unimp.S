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

|
|	x_unimp.sa 3.3 7/1/91
|
|	fpsp_unimp --- FPSP handler for unimplemented instruction
|	exception.
|
| Invoked when the user program encounters a floating-point
| op-code that hardware does not support.  Trap vector# 11
| (See table 8-1 MC68030 User's Manual).
|
|
| Note: An fsave for an unimplemented inst. will create a short
| fsave stack.
|
|  Input: 1. Six word stack frame for unimplemented inst, four word
|            for illegal
|            (See table 8-7 MC68030 User's Manual).
|         2. Unimp (short) fsave state frame created here by fsave
|            instruction.
|
|
|		Copyright (C) Motorola, Inc. 1990
|			All Rights Reserved
|
|       For details on the license for this file, please see the
|       file, README, in this same directory.

X_UNIMP:	|idnt    2,1 | Motorola 040 Floating Point Software Package

	|section	8

#include "fpsp.h"

	|xref	get_op
	|xref	do_func
	|xref	sto_res
	|xref	gen_except
	|xref	fpsp_fmt_error

	.global	fpsp_unimp
	.global	uni_2
fpsp_unimp:
	link		%a6,#-LOCAL_SIZE
	fsave		-(%a7)
uni_2:
	moveml		%d0-%d1/%a0-%a1,USER_DA(%a6)
	fmovemx	%fp0-%fp3,USER_FP0(%a6)
	fmoveml	%fpcr/%fpsr/%fpiar,USER_FPCR(%a6)
	moveb		(%a7),%d0		|test for valid version num
	andib		#0xf0,%d0		|test for $4x
	cmpib		#VER_4,%d0	|must be $4x or exit
	bnel		fpsp_fmt_error
|
|	Temporary D25B Fix
|	The following lines are used to ensure that the FPSR
|	exception byte and condition codes are clear before proceeding
|
	movel		USER_FPSR(%a6),%d0
	andl		#0xFF00FF,%d0	|clear all but accrued exceptions
	movel		%d0,USER_FPSR(%a6)
	fmovel		#0,%FPSR |clear all user bits
	fmovel		#0,%FPCR	|clear all user exceptions for FPSP

	clrb		UFLG_TMP(%a6)	|clr flag for unsupp data

	bsrl		get_op		|go get operand(s)
	clrb		STORE_FLG(%a6)
	bsrl		do_func		|do the function
	fsave		-(%a7)		|capture possible exc state
	tstb		STORE_FLG(%a6)
	bnes		no_store	|if STORE_FLG is set, no store
	bsrl		sto_res		|store the result in user space
no_store:
	bral		gen_except	|post any exceptions and return

	|end
