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
 * Copyright (C) 2005-2006 Atmel Corporation
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

/*
 * Stubs for syscalls that require access to pt_regs or that take more
 * than five parameters.
 */

#define ARG6	r3

	.text
	.global __sys_rt_sigsuspend
	.type	__sys_rt_sigsuspend,@function
__sys_rt_sigsuspend:
	mov	r10, sp
	rjmp	sys_rt_sigsuspend

	.global	__sys_sigaltstack
	.type	__sys_sigaltstack,@function
__sys_sigaltstack:
	mov	r10, sp
	rjmp	sys_sigaltstack

	.global	__sys_rt_sigreturn
	.type	__sys_rt_sigreturn,@function
__sys_rt_sigreturn:
	mov	r12, sp
	rjmp	sys_rt_sigreturn

	.global	__sys_fork
	.type	__sys_fork,@function
__sys_fork:
	mov	r12, sp
	rjmp	sys_fork

	.global	__sys_clone
	.type	__sys_clone,@function
__sys_clone:
	mov	r8, sp
	rjmp	sys_clone

	.global	__sys_vfork
	.type	__sys_vfork,@function
__sys_vfork:
	mov	r12, sp
	rjmp	sys_vfork

	.global	__sys_execve
	.type	__sys_execve,@function
__sys_execve:
	mov	r9, sp
	rjmp	sys_execve

	.global	__sys_mmap2
	.type	__sys_mmap2,@function
__sys_mmap2:
	pushm	lr
	st.w	--sp, ARG6
	rcall	sys_mmap2
	sub	sp, -4
	popm	pc

	.global	__sys_sendto
	.type	__sys_sendto,@function
__sys_sendto:
	pushm	lr
	st.w	--sp, ARG6
	rcall	sys_sendto
	sub	sp, -4
	popm	pc

	.global	__sys_recvfrom
	.type	__sys_recvfrom,@function
__sys_recvfrom:
	pushm	lr
	st.w	--sp, ARG6
	rcall	sys_recvfrom
	sub	sp, -4
	popm	pc

	.global	__sys_pselect6
	.type	__sys_pselect6,@function
__sys_pselect6:
	pushm	lr
	st.w	--sp, ARG6
	rcall	sys_pselect6
	sub	sp, -4
	popm	pc

	.global	__sys_splice
	.type	__sys_splice,@function
__sys_splice:
	pushm	lr
	st.w	--sp, ARG6
	rcall	sys_splice
	sub	sp, -4
	popm	pc

	.global	__sys_epoll_pwait
	.type	__sys_epoll_pwait,@function
__sys_epoll_pwait:
	pushm	lr
	st.w	--sp, ARG6
	rcall	sys_epoll_pwait
	sub	sp, -4
	popm	pc

	.global __sys_sync_file_range
	.type	__sys_sync_file_range,@function
__sys_sync_file_range:
	pushm	lr
	st.w	--sp, ARG6
	rcall	sys_sync_file_range
	sub	sp, -4
	popm	pc
