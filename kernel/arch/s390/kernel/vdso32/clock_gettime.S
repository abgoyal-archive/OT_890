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
 * Userland implementation of clock_gettime() for 32 bits processes in a
 * s390 kernel for use in the vDSO
 *
 *  Copyright IBM Corp. 2008
 *  Author(s): Martin Schwidefsky (schwidefsky@de.ibm.com)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License (version 2 only)
 * as published by the Free Software Foundation.
 */
#include <asm/vdso.h>
#include <asm/asm-offsets.h>
#include <asm/unistd.h>

	.text
	.align 4
	.globl __kernel_clock_gettime
	.type  __kernel_clock_gettime,@function
__kernel_clock_gettime:
	.cfi_startproc
	basr	%r5,0
0:	al	%r5,21f-0b(%r5)			/* get &_vdso_data */
	chi	%r2,CLOCK_REALTIME
	je	10f
	chi	%r2,CLOCK_MONOTONIC
	jne	19f

	/* CLOCK_MONOTONIC */
	ltr	%r3,%r3
	jz	9f				/* tp == NULL */
1:	l	%r4,__VDSO_UPD_COUNT+4(%r5)	/* load update counter */
	tml	%r4,0x0001			/* pending update ? loop */
	jnz	1b
	stck	24(%r15)			/* Store TOD clock */
	lm	%r0,%r1,24(%r15)
	s	%r0,__VDSO_XTIME_STAMP(%r5)	/* TOD - cycle_last */
	sl	%r1,__VDSO_XTIME_STAMP+4(%r5)
	brc	3,2f
	ahi	%r0,-1
2:	mhi	%r0,1000			/* cyc2ns(clock,cycle_delta) */
	lr	%r2,%r0
	lhi	%r0,1000
	ltr	%r1,%r1
	mr	%r0,%r0
	jnm	3f
	ahi	%r0,1000
3:	alr	%r0,%r2
	srdl	%r0,12
	al	%r0,__VDSO_XTIME_NSEC(%r5)	/*  + xtime */
	al	%r1,__VDSO_XTIME_NSEC+4(%r5)
	brc	12,4f
	ahi	%r0,1
4:	l	%r2,__VDSO_XTIME_SEC+4(%r5)
	al	%r0,__VDSO_WTOM_NSEC(%r5)	/*  + wall_to_monotonic */
	al	%r1,__VDSO_WTOM_NSEC+4(%r5)
	brc	12,5f
	ahi	%r0,1
5:	al	%r2,__VDSO_WTOM_SEC+4(%r5)
	cl	%r4,__VDSO_UPD_COUNT+4(%r5)	/* check update counter */
	jne	1b
	basr	%r5,0
6:	ltr	%r0,%r0
	jnz	7f
	cl	%r1,20f-6b(%r5)
	jl	8f
7:	ahi	%r2,1
	sl	%r1,20f-6b(%r5)
	brc	3,6b
	ahi	%r0,-1
	j	6b
8:	st	%r2,0(%r3)			/* store tp->tv_sec */
	st	%r1,4(%r3)			/* store tp->tv_nsec */
9:	lhi	%r2,0
	br	%r14

	/* CLOCK_REALTIME */
10:	ltr	%r3,%r3				/* tp == NULL */
	jz	18f
11:	l	%r4,__VDSO_UPD_COUNT+4(%r5)	/* load update counter */
	tml	%r4,0x0001			/* pending update ? loop */
	jnz	11b
	stck	24(%r15)			/* Store TOD clock */
	lm	%r0,%r1,24(%r15)
	s	%r0,__VDSO_XTIME_STAMP(%r5)	/* TOD - cycle_last */
	sl	%r1,__VDSO_XTIME_STAMP+4(%r5)
	brc	3,12f
	ahi	%r0,-1
12:	mhi	%r0,1000			/* cyc2ns(clock,cycle_delta) */
	lr	%r2,%r0
	lhi	%r0,1000
	ltr	%r1,%r1
	mr	%r0,%r0
	jnm	13f
	ahi	%r0,1000
13:	alr	%r0,%r2
	srdl	%r0,12
	al	%r0,__VDSO_XTIME_NSEC(%r5)	/*  + xtime */
	al	%r1,__VDSO_XTIME_NSEC+4(%r5)
	brc	12,14f
	ahi	%r0,1
14:	l	%r2,__VDSO_XTIME_SEC+4(%r5)
	cl	%r4,__VDSO_UPD_COUNT+4(%r5)	/* check update counter */
	jne	11b
	basr	%r5,0
15:	ltr	%r0,%r0
	jnz	16f
	cl	%r1,20f-15b(%r5)
	jl	17f
16:	ahi	%r2,1
	sl	%r1,20f-15b(%r5)
	brc	3,15b
	ahi	%r0,-1
	j	15b
17:	st	%r2,0(%r3)			/* store tp->tv_sec */
	st	%r1,4(%r3)			/* store tp->tv_nsec */
18:	lhi	%r2,0
	br	%r14

	/* Fallback to system call */
19:	lhi	%r1,__NR_clock_gettime
	svc	0
	br	%r14

20:	.long	1000000000
21:	.long	_vdso_data - 0b
	.cfi_endproc
	.size	__kernel_clock_gettime,.-__kernel_clock_gettime
