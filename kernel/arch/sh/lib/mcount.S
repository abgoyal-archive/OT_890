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
 * arch/sh/lib/mcount.S
 *
 *  Copyright (C) 2008  Paul Mundt
 *  Copyright (C) 2008  Matt Fleming
 *
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 */
#include <asm/ftrace.h>

#define MCOUNT_ENTER()		\
	mov.l	r4, @-r15;	\
	mov.l	r5, @-r15;	\
	mov.l	r6, @-r15;	\
	mov.l	r7, @-r15;	\
	sts.l	pr, @-r15;	\
				\
	mov.l	@(20,r15),r4;	\
	sts	pr, r5

#define MCOUNT_LEAVE()		\
	lds.l	@r15+, pr;	\
	mov.l	@r15+, r7;	\
	mov.l	@r15+, r6;	\
	mov.l	@r15+, r5;	\
	rts;			\
	 mov.l	@r15+, r4

	.align 2
	.globl	_mcount
	.type	_mcount,@function
	.globl	mcount
	.type	mcount,@function
_mcount:
mcount:
	MCOUNT_ENTER()

#ifdef CONFIG_DYNAMIC_FTRACE
	.globl	mcount_call
mcount_call:
	mov.l	.Lftrace_stub, r6
#else
	mov.l	.Lftrace_trace_function, r6
	mov.l	ftrace_stub, r7
	cmp/eq	r6, r7
	bt	skip_trace
	mov.l	@r6, r6
#endif

	jsr	@r6
	 nop

skip_trace:
	MCOUNT_LEAVE()

	.align 2
.Lftrace_trace_function:
	.long	ftrace_trace_function

#ifdef CONFIG_DYNAMIC_FTRACE
	.globl ftrace_caller
ftrace_caller:
	MCOUNT_ENTER()

	.globl ftrace_call
ftrace_call:
	mov.l	.Lftrace_stub, r6
	jsr	@r6
	 nop

	MCOUNT_LEAVE()
#endif /* CONFIG_DYNAMIC_FTRACE */

/*
 * NOTE: From here on the locations of the .Lftrace_stub label and
 * ftrace_stub itself are fixed. Adding additional data here will skew
 * the displacement for the memory table and break the block replacement.
 * Place new labels either after the ftrace_stub body, or before
 * ftrace_caller. You have been warned.
 */
	.align 2
.Lftrace_stub:
	.long	ftrace_stub

	.globl	ftrace_stub
ftrace_stub:
	rts
	 nop
