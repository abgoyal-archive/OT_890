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
 * Userland implementation of clock_gettime() for 64 bits processes in a
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
	larl	%r5,_vdso_data
	cghi	%r2,CLOCK_REALTIME
	je	4f
	cghi	%r2,-2		/* CLOCK_THREAD_CPUTIME_ID for this thread */
	je	9f
	cghi	%r2,CLOCK_MONOTONIC
	jne	12f

	/* CLOCK_MONOTONIC */
	ltgr	%r3,%r3
	jz	3f				/* tp == NULL */
0:	lg	%r4,__VDSO_UPD_COUNT(%r5)	/* load update counter */
	tmll	%r4,0x0001			/* pending update ? loop */
	jnz	0b
	stck	48(%r15)			/* Store TOD clock */
	lg	%r1,48(%r15)
	sg	%r1,__VDSO_XTIME_STAMP(%r5)	/* TOD - cycle_last */
	mghi	%r1,1000
	srlg	%r1,%r1,12			/* cyc2ns(clock,cycle_delta) */
	alg	%r1,__VDSO_XTIME_NSEC(%r5)	/*  + xtime */
	lg	%r0,__VDSO_XTIME_SEC(%r5)
	alg	%r1,__VDSO_WTOM_NSEC(%r5)	/*  + wall_to_monotonic */
	alg	%r0,__VDSO_WTOM_SEC(%r5)
	clg	%r4,__VDSO_UPD_COUNT(%r5)	/* check update counter */
	jne	0b
	larl	%r5,13f
1:	clg	%r1,0(%r5)
	jl	2f
	slg	%r1,0(%r5)
	aghi	%r0,1
	j	1b
2:	stg	%r0,0(%r3)			/* store tp->tv_sec */
	stg	%r1,8(%r3)			/* store tp->tv_nsec */
3:	lghi	%r2,0
	br	%r14

	/* CLOCK_REALTIME */
4:	ltr	%r3,%r3				/* tp == NULL */
	jz	8f
5:	lg	%r4,__VDSO_UPD_COUNT(%r5)	/* load update counter */
	tmll	%r4,0x0001			/* pending update ? loop */
	jnz	5b
	stck	48(%r15)			/* Store TOD clock */
	lg	%r1,48(%r15)
	sg	%r1,__VDSO_XTIME_STAMP(%r5)	/* TOD - cycle_last */
	mghi	%r1,1000
	srlg	%r1,%r1,12			/* cyc2ns(clock,cycle_delta) */
	alg	%r1,__VDSO_XTIME_NSEC(%r5)	/*  + xtime */
	lg	%r0,__VDSO_XTIME_SEC(%r5)
	clg	%r4,__VDSO_UPD_COUNT(%r5)	/* check update counter */
	jne	5b
	larl	%r5,13f
6:	clg	%r1,0(%r5)
	jl	7f
	slg	%r1,0(%r5)
	aghi	%r0,1
	j	6b
7:	stg	%r0,0(%r3)			/* store tp->tv_sec */
	stg	%r1,8(%r3)			/* store tp->tv_nsec */
8:	lghi	%r2,0
	br	%r14

	/* CLOCK_THREAD_CPUTIME_ID for this thread */
9:	icm	%r0,15,__VDSO_ECTG_OK(%r5)
	jz	12f
	ear	%r2,%a4
	llilh	%r4,0x0100
	sar	%a4,%r4
	lghi	%r4,0
	sacf	512				/* Magic ectg instruction */
	.insn	ssf,0xc80100000000,__VDSO_ECTG_BASE(4),__VDSO_ECTG_USER(4),4
	sacf	0
	sar	%a4,%r2
	algr	%r1,%r0				/* r1 = cputime as TOD value */
	mghi	%r1,1000			/* convert to nanoseconds */
	srlg	%r1,%r1,12			/* r1 = cputime in nanosec */
	lgr	%r4,%r1
	larl	%r5,13f
	srlg	%r1,%r1,9			/* divide by 1000000000 */
	mlg	%r0,8(%r5)
	srlg	%r0,%r0,11			/* r0 = tv_sec */
	stg	%r0,0(%r3)
	msg	%r0,0(%r5)			/* calculate tv_nsec */
	slgr	%r4,%r0				/* r4 = tv_nsec */
	stg	%r4,8(%r3)
	lghi	%r2,0
	br	%r14

	/* Fallback to system call */
12:	lghi	%r1,__NR_clock_gettime
	svc	0
	br	%r14

13:	.quad	1000000000
14:	.quad	19342813113834067
	.cfi_endproc
	.size	__kernel_clock_gettime,.-__kernel_clock_gettime
