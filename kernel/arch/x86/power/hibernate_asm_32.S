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
 * This may not use any stack, nor any variable that is not "NoSave":
 *
 * Its rewriting one kernel image with another. What is stack in "old"
 * image could very well be data page in "new" image, and overwriting
 * your own stack under you is bad idea.
 */

#include <linux/linkage.h>
#include <asm/segment.h>
#include <asm/page.h>
#include <asm/asm-offsets.h>
#include <asm/processor-flags.h>

.text

ENTRY(swsusp_arch_suspend)
	movl %esp, saved_context_esp
	movl %ebx, saved_context_ebx
	movl %ebp, saved_context_ebp
	movl %esi, saved_context_esi
	movl %edi, saved_context_edi
	pushfl
	popl saved_context_eflags

	call swsusp_save
	ret

ENTRY(restore_image)
	movl	resume_pg_dir, %eax
	subl	$__PAGE_OFFSET, %eax
	movl	%eax, %cr3

	movl	restore_pblist, %edx
	.p2align 4,,7

copy_loop:
	testl	%edx, %edx
	jz	done

	movl	pbe_address(%edx), %esi
	movl	pbe_orig_address(%edx), %edi

	movl	$1024, %ecx
	rep
	movsl

	movl	pbe_next(%edx), %edx
	jmp	copy_loop
	.p2align 4,,7

done:
	/* go back to the original page tables */
	movl	$swapper_pg_dir, %eax
	subl	$__PAGE_OFFSET, %eax
	movl	%eax, %cr3
	/* Flush TLB, including "global" things (vmalloc) */
	movl	mmu_cr4_features, %ecx
	jecxz	1f	# cr4 Pentium and higher, skip if zero
	movl	%ecx, %edx
	andl	$~(X86_CR4_PGE), %edx
	movl	%edx, %cr4;  # turn off PGE
1:
	movl	%cr3, %eax;  # flush TLB
	movl	%eax, %cr3
	jecxz	1f	# cr4 Pentium and higher, skip if zero
	movl	%ecx, %cr4;  # turn PGE back on
1:

	movl saved_context_esp, %esp
	movl saved_context_ebp, %ebp
	movl saved_context_ebx, %ebx
	movl saved_context_esi, %esi
	movl saved_context_edi, %edi

	pushl saved_context_eflags
	popfl

	xorl	%eax, %eax

	ret
