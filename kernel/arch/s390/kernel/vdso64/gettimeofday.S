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
 * Userland implementation of gettimeofday() for 64 bits processes in a
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
	.globl __kernel_gettimeofday
	.type  __kernel_gettimeofday,@function
__kernel_gettimeofday:
	.cfi_startproc
	larl	%r5,_vdso_data
0:	ltgr	%r3,%r3				/* check if tz is NULL */
	je	1f
	mvc	0(8,%r3),__VDSO_TIMEZONE(%r5)
1:	ltgr	%r2,%r2				/* check if tv is NULL */
	je	4f
	lg	%r4,__VDSO_UPD_COUNT(%r5)	/* load update counter */
	tmll	%r4,0x0001			/* pending update ? loop */
	jnz	0b
	stck	48(%r15)			/* Store TOD clock */
	lg	%r1,48(%r15)
	sg	%r1,__VDSO_XTIME_STAMP(%r5)	/* TOD - cycle_last */
	mghi	%r1,1000
	srlg	%r1,%r1,12			/* cyc2ns(clock,cycle_delta) */
	alg	%r1,__VDSO_XTIME_NSEC(%r5)	/*  + xtime.tv_nsec */
	lg	%r0,__VDSO_XTIME_SEC(%r5)	/* xtime.tv_sec */
	clg	%r4,__VDSO_UPD_COUNT(%r5)	/* check update counter */
	jne	0b
	larl	%r5,5f
2:	clg	%r1,0(%r5)
	jl	3f
	slg	%r1,0(%r5)
	aghi	%r0,1
	j	2b
3:	stg	%r0,0(%r2)			/* store tv->tv_sec */
	slgr	%r0,%r0				/* tv_nsec -> tv_usec */
	ml	%r0,8(%r5)
	srlg	%r0,%r0,6
	stg	%r0,8(%r2)			/* store tv->tv_usec */
4:	lghi	%r2,0
	br	%r14
5:	.quad	1000000000
	.long	274877907
	.cfi_endproc
	.size	__kernel_gettimeofday,.-__kernel_gettimeofday
