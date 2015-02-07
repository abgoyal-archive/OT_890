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
 * Copyright IBM Corp. 2008
 *
 *   Author(s): Heiko Carstens <heiko.carstens@de.ibm.com>,
 *
 */

#include <asm/asm-offsets.h>

#ifndef CONFIG_64BIT
.globl _mcount
_mcount:
	stm	%r0,%r5,8(%r15)
	st	%r14,56(%r15)
	lr	%r1,%r15
	ahi	%r15,-96
	l	%r3,100(%r15)
	la	%r2,0(%r14)
	st	%r1,__SF_BACKCHAIN(%r15)
	la	%r3,0(%r3)
	bras	%r14,0f
	.long	ftrace_trace_function
0:	l	%r14,0(%r14)
	l	%r14,0(%r14)
	basr	%r14,%r14
	ahi	%r15,96
	lm	%r0,%r5,8(%r15)
	l	%r14,56(%r15)
	br	%r14

.globl ftrace_stub
ftrace_stub:
	br	%r14

#else /* CONFIG_64BIT */

.globl _mcount
_mcount:
	stmg	%r0,%r5,16(%r15)
	stg	%r14,112(%r15)
	lgr	%r1,%r15
	aghi	%r15,-160
	stg	%r1,__SF_BACKCHAIN(%r15)
	lgr	%r2,%r14
	lg	%r3,168(%r15)
	larl	%r14,ftrace_trace_function
	lg	%r14,0(%r14)
	basr	%r14,%r14
	aghi	%r15,160
	lmg	%r0,%r5,16(%r15)
	lg	%r14,112(%r15)
	br	%r14

.globl ftrace_stub
ftrace_stub:
	br	%r14

#endif /* CONFIG_64BIT */
