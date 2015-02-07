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

/* ----------------------------------------------------------------------- *
 *
 *   Copyright (C) 1991, 1992 Linus Torvalds
 *   Copyright 2007 rPath, Inc. - All Rights Reserved
 *
 *   This file is part of the Linux kernel, and is made available under
 *   the terms of the GNU General Public License version 2.
 *
 * ----------------------------------------------------------------------- */

/*
 * The actual transition into protected mode
 */

#include <asm/boot.h>
#include <asm/processor-flags.h>
#include <asm/segment.h>

	.text

	.globl	protected_mode_jump
	.type	protected_mode_jump, @function

	.code16

/*
 * void protected_mode_jump(u32 entrypoint, u32 bootparams);
 */
protected_mode_jump:
	movl	%edx, %esi		# Pointer to boot_params table

	xorl	%ebx, %ebx
	movw	%cs, %bx
	shll	$4, %ebx
	addl	%ebx, 2f
	jmp	1f			# Short jump to serialize on 386/486
1:

	movw	$__BOOT_DS, %cx
	movw	$__BOOT_TSS, %di

	movl	%cr0, %edx
	orb	$X86_CR0_PE, %dl	# Protected mode
	movl	%edx, %cr0

	# Transition to 32-bit mode
	.byte	0x66, 0xea		# ljmpl opcode
2:	.long	in_pm32			# offset
	.word	__BOOT_CS		# segment

	.size	protected_mode_jump, .-protected_mode_jump

	.code32
	.type	in_pm32, @function
in_pm32:
	# Set up data segments for flat 32-bit mode
	movl	%ecx, %ds
	movl	%ecx, %es
	movl	%ecx, %fs
	movl	%ecx, %gs
	movl	%ecx, %ss
	# The 32-bit code sets up its own stack, but this way we do have
	# a valid stack if some debugging hack wants to use it.
	addl	%ebx, %esp

	# Set up TR to make Intel VT happy
	ltr	%di

	# Clear registers to allow for future extensions to the
	# 32-bit boot protocol
	xorl	%ecx, %ecx
	xorl	%edx, %edx
	xorl	%ebx, %ebx
	xorl	%ebp, %ebp
	xorl	%edi, %edi

	# Set up LDTR to make Intel VT happy
	lldt	%cx

	jmpl	*%eax			# Jump to the 32-bit entrypoint

	.size	in_pm32, .-in_pm32
