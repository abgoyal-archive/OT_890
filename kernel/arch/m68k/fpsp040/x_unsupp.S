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
|	x_unsupp.sa 3.3 7/1/91
|
|	fpsp_unsupp --- FPSP handler for unsupported data type exception
|
| Trap vector #55	(See table 8-1 Mc68030 User's manual).
| Invoked when the user program encounters a data format (packed) that
| hardware does not support or a data type (denormalized numbers or un-
| normalized numbers).
| Normalizes denorms and unnorms, unpacks packed numbers then stores
| them back into the machine to let the 040 finish the operation.
|
| Unsupp calls two routines:
|	1. get_op -  gets the operand(s)
|	2. res_func - restore the function back into the 040 or
|			if fmove.p fpm,<ea> then pack source (fpm)
|			and store in users memory <ea>.
|
|  Input: Long fsave stack frame
|
|

|		Copyright (C) Motorola, Inc. 1990
|			All Rights Reserved
|
|       For details on the license for this file, please see the
|       file, README, in this same directory.

X_UNSUPP:	|idnt    2,1 | Motorola 040 Floating Point Software Package

	|section	8

#include "fpsp.h"

	|xref	get_op
	|xref	res_func
	|xref	gen_except
	|xref	fpsp_fmt_error

	.global	fpsp_unsupp
fpsp_unsupp:
|
	link		%a6,#-LOCAL_SIZE
	fsave		-(%a7)
	moveml		%d0-%d1/%a0-%a1,USER_DA(%a6)
	fmovemx	%fp0-%fp3,USER_FP0(%a6)
	fmoveml	%fpcr/%fpsr/%fpiar,USER_FPCR(%a6)


	moveb		(%a7),VER_TMP(%a6) |save version number
	moveb		(%a7),%d0		|test for valid version num
	andib		#0xf0,%d0		|test for $4x
	cmpib		#VER_4,%d0	|must be $4x or exit
	bnel		fpsp_fmt_error

	fmovel		#0,%FPSR		|clear all user status bits
	fmovel		#0,%FPCR		|clear all user control bits
|
|	The following lines are used to ensure that the FPSR
|	exception byte and condition codes are clear before proceeding,
|	except in the case of fmove, which leaves the cc's intact.
|
unsupp_con:
	movel		USER_FPSR(%a6),%d1
	btst		#5,CMDREG1B(%a6)	|looking for fmove out
	bne		fmove_con
	andl		#0xFF00FF,%d1	|clear all but aexcs and qbyte
	bras		end_fix
fmove_con:
	andl		#0x0FFF40FF,%d1	|clear all but cc's, snan bit, aexcs, and qbyte
end_fix:
	movel		%d1,USER_FPSR(%a6)

	st		UFLG_TMP(%a6)	|set flag for unsupp data

	bsrl		get_op		|everything okay, go get operand(s)
	bsrl		res_func	|fix up stack frame so can restore it
	clrl		-(%a7)
	moveb		VER_TMP(%a6),(%a7) |move idle fmt word to top of stack
	bral		gen_except
|
	|end
