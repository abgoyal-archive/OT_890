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
 * Function calling ABI conversion from Linux to EFI for x86_64
 *
 * Copyright (C) 2007 Intel Corp
 *	Bibo Mao <bibo.mao@intel.com>
 *	Huang Ying <ying.huang@intel.com>
 */

#include <linux/linkage.h>

#define SAVE_XMM			\
	mov %rsp, %rax;			\
	subq $0x70, %rsp;		\
	and $~0xf, %rsp;		\
	mov %rax, (%rsp);		\
	mov %cr0, %rax;			\
	clts;				\
	mov %rax, 0x8(%rsp);		\
	movaps %xmm0, 0x60(%rsp);	\
	movaps %xmm1, 0x50(%rsp);	\
	movaps %xmm2, 0x40(%rsp);	\
	movaps %xmm3, 0x30(%rsp);	\
	movaps %xmm4, 0x20(%rsp);	\
	movaps %xmm5, 0x10(%rsp)

#define RESTORE_XMM			\
	movaps 0x60(%rsp), %xmm0;	\
	movaps 0x50(%rsp), %xmm1;	\
	movaps 0x40(%rsp), %xmm2;	\
	movaps 0x30(%rsp), %xmm3;	\
	movaps 0x20(%rsp), %xmm4;	\
	movaps 0x10(%rsp), %xmm5;	\
	mov 0x8(%rsp), %rsi;		\
	mov %rsi, %cr0;			\
	mov (%rsp), %rsp

ENTRY(efi_call0)
	SAVE_XMM
	subq $32, %rsp
	call *%rdi
	addq $32, %rsp
	RESTORE_XMM
	ret

ENTRY(efi_call1)
	SAVE_XMM
	subq $32, %rsp
	mov  %rsi, %rcx
	call *%rdi
	addq $32, %rsp
	RESTORE_XMM
	ret

ENTRY(efi_call2)
	SAVE_XMM
	subq $32, %rsp
	mov  %rsi, %rcx
	call *%rdi
	addq $32, %rsp
	RESTORE_XMM
	ret

ENTRY(efi_call3)
	SAVE_XMM
	subq $32, %rsp
	mov  %rcx, %r8
	mov  %rsi, %rcx
	call *%rdi
	addq $32, %rsp
	RESTORE_XMM
	ret

ENTRY(efi_call4)
	SAVE_XMM
	subq $32, %rsp
	mov %r8, %r9
	mov %rcx, %r8
	mov %rsi, %rcx
	call *%rdi
	addq $32, %rsp
	RESTORE_XMM
	ret

ENTRY(efi_call5)
	SAVE_XMM
	subq $48, %rsp
	mov %r9, 32(%rsp)
	mov %r8, %r9
	mov %rcx, %r8
	mov %rsi, %rcx
	call *%rdi
	addq $48, %rsp
	RESTORE_XMM
	ret

ENTRY(efi_call6)
	SAVE_XMM
	mov (%rsp), %rax
	mov 8(%rax), %rax
	subq $48, %rsp
	mov %r9, 32(%rsp)
	mov %rax, 40(%rsp)
	mov %r8, %r9
	mov %rcx, %r8
	mov %rsi, %rcx
	call *%rdi
	addq $48, %rsp
	RESTORE_XMM
	ret
