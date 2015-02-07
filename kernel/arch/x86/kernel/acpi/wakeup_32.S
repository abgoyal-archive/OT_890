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

	.section .text.page_aligned
#include <linux/linkage.h>
#include <asm/segment.h>
#include <asm/page.h>

# Copyright 2003, 2008 Pavel Machek <pavel@suse.cz>, distribute under GPLv2

	.code32
	ALIGN

ENTRY(wakeup_pmode_return)
wakeup_pmode_return:
	movw	$__KERNEL_DS, %ax
	movw	%ax, %ss
	movw	%ax, %ds
	movw	%ax, %es
	movw	%ax, %fs
	movw	%ax, %gs

	# reload the gdt, as we need the full 32 bit address
	lgdt	saved_gdt
	lidt	saved_idt
	lldt	saved_ldt
	ljmp	$(__KERNEL_CS), $1f
1:
	movl	%cr3, %eax
	movl	%eax, %cr3
	wbinvd

	# and restore the stack ... but you need gdt for this to work
	movl	saved_context_esp, %esp

	movl	%cs:saved_magic, %eax
	cmpl	$0x12345678, %eax
	jne	bogus_magic

	# jump to place where we left off
	movl	saved_eip, %eax
	jmp	*%eax

bogus_magic:
	jmp	bogus_magic



save_registers:
	sgdt	saved_gdt
	sidt	saved_idt
	sldt	saved_ldt
	str	saved_tss

	leal	4(%esp), %eax
	movl	%eax, saved_context_esp
	movl	%ebx, saved_context_ebx
	movl	%ebp, saved_context_ebp
	movl	%esi, saved_context_esi
	movl	%edi, saved_context_edi
	pushfl
	popl	saved_context_eflags

	movl	$ret_point, saved_eip
	ret


restore_registers:
	movl	saved_context_ebp, %ebp
	movl	saved_context_ebx, %ebx
	movl	saved_context_esi, %esi
	movl	saved_context_edi, %edi
	pushl	saved_context_eflags
	popfl
	ret

ENTRY(do_suspend_lowlevel)
	call	save_processor_state
	call	save_registers
	pushl	$3
	call	acpi_enter_sleep_state
	addl	$4, %esp

#	In case of S3 failure, we'll emerge here.  Jump
# 	to ret_point to recover
	jmp	ret_point
	.p2align 4,,7
ret_point:
	call	restore_registers
	call	restore_processor_state
	ret

.data
ALIGN
ENTRY(saved_magic)	.long	0
ENTRY(saved_eip)	.long	0

# saved registers
saved_gdt:	.long	0,0
saved_idt:	.long	0,0
saved_ldt:	.long	0
saved_tss:	.long	0

