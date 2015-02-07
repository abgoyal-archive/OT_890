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
|	satanh.sa 3.3 12/19/90
|
|	The entry point satanh computes the inverse
|	hyperbolic tangent of
|	an input argument; satanhd does the same except for denormalized
|	input.
|
|	Input: Double-extended number X in location pointed to
|		by address register a0.
|
|	Output: The value arctanh(X) returned in floating-point register Fp0.
|
|	Accuracy and Monotonicity: The returned result is within 3 ulps in
|		64 significant bit, i.e. within 0.5001 ulp to 53 bits if the
|		result is subsequently rounded to double precision. The
|		result is provably monotonic in double precision.
|
|	Speed: The program satanh takes approximately 270 cycles.
|
|	Algorithm:
|
|	ATANH
|	1. If |X| >= 1, go to 3.
|
|	2. (|X| < 1) Calculate atanh(X) by
|		sgn := sign(X)
|		y := |X|
|		z := 2y/(1-y)
|		atanh(X) := sgn * (1/2) * logp1(z)
|		Exit.
|
|	3. If |X| > 1, go to 5.
|
|	4. (|X| = 1) Generate infinity with an appropriate sign and
|		divide-by-zero by
|		sgn := sign(X)
|		atan(X) := sgn / (+0).
|		Exit.
|
|	5. (|X| > 1) Generate an invalid operation by 0 * infinity.
|		Exit.
|

|		Copyright (C) Motorola, Inc. 1990
|			All Rights Reserved
|
|       For details on the license for this file, please see the
|       file, README, in this same directory.

|satanh	idnt	2,1 | Motorola 040 Floating Point Software Package

	|section	8

	|xref	t_dz
	|xref	t_operr
	|xref	t_frcinx
	|xref	t_extdnrm
	|xref	slognp1

	.global	satanhd
satanhd:
|--ATANH(X) = X FOR DENORMALIZED X

	bra		t_extdnrm

	.global	satanh
satanh:
	movel		(%a0),%d0
	movew		4(%a0),%d0
	andil		#0x7FFFFFFF,%d0
	cmpil		#0x3FFF8000,%d0
	bges		ATANHBIG

|--THIS IS THE USUAL CASE, |X| < 1
|--Y = |X|, Z = 2Y/(1-Y), ATANH(X) = SIGN(X) * (1/2) * LOG1P(Z).

	fabsx		(%a0),%fp0	| ...Y = |X|
	fmovex		%fp0,%fp1
	fnegx		%fp1		| ...-Y
	faddx		%fp0,%fp0		| ...2Y
	fadds		#0x3F800000,%fp1	| ...1-Y
	fdivx		%fp1,%fp0		| ...2Y/(1-Y)
	movel		(%a0),%d0
	andil		#0x80000000,%d0
	oril		#0x3F000000,%d0	| ...SIGN(X)*HALF
	movel		%d0,-(%sp)

	fmovemx	%fp0-%fp0,(%a0)	| ...overwrite input
	movel		%d1,-(%sp)
	clrl		%d1
	bsr		slognp1		| ...LOG1P(Z)
	fmovel		(%sp)+,%fpcr
	fmuls		(%sp)+,%fp0
	bra		t_frcinx

ATANHBIG:
	fabsx		(%a0),%fp0	| ...|X|
	fcmps		#0x3F800000,%fp0
	fbgt		t_operr
	bra		t_dz

	|end
