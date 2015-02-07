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
 * i386 semaphore implementation.
 *
 * (C) Copyright 1999 Linus Torvalds
 *
 * Portions Copyright 1999 Red Hat, Inc.
 *
 *	This program is free software; you can redistribute it and/or
 *	modify it under the terms of the GNU General Public License
 *	as published by the Free Software Foundation; either version
 *	2 of the License, or (at your option) any later version.
 *
 * rw semaphores implemented November 1999 by Benjamin LaHaise <bcrl@kvack.org>
 */

#include <linux/linkage.h>
#include <asm/rwlock.h>
#include <asm/alternative-asm.h>
#include <asm/frame.h>
#include <asm/dwarf2.h>

/*
 * The semaphore operations have a special calling sequence that
 * allow us to do a simpler in-line version of them. These routines
 * need to convert that sequence back into the C sequence when
 * there is contention on the semaphore.
 *
 * %eax contains the semaphore pointer on entry. Save the C-clobbered
 * registers (%eax, %edx and %ecx) except %eax whish is either a return
 * value or just clobbered..
 */
	.section .sched.text, "ax"

/*
 * rw spinlock fallbacks
 */
#ifdef CONFIG_SMP
ENTRY(__write_lock_failed)
	CFI_STARTPROC simple
	FRAME
2: 	LOCK_PREFIX
	addl	$ RW_LOCK_BIAS,(%eax)
1:	rep; nop
	cmpl	$ RW_LOCK_BIAS,(%eax)
	jne	1b
	LOCK_PREFIX
	subl	$ RW_LOCK_BIAS,(%eax)
	jnz	2b
	ENDFRAME
	ret
	CFI_ENDPROC
	ENDPROC(__write_lock_failed)

ENTRY(__read_lock_failed)
	CFI_STARTPROC
	FRAME
2: 	LOCK_PREFIX
	incl	(%eax)
1:	rep; nop
	cmpl	$1,(%eax)
	js	1b
	LOCK_PREFIX
	decl	(%eax)
	js	2b
	ENDFRAME
	ret
	CFI_ENDPROC
	ENDPROC(__read_lock_failed)

#endif

#ifdef CONFIG_RWSEM_XCHGADD_ALGORITHM

/* Fix up special calling conventions */
ENTRY(call_rwsem_down_read_failed)
	CFI_STARTPROC
	push %ecx
	CFI_ADJUST_CFA_OFFSET 4
	CFI_REL_OFFSET ecx,0
	push %edx
	CFI_ADJUST_CFA_OFFSET 4
	CFI_REL_OFFSET edx,0
	call rwsem_down_read_failed
	pop %edx
	CFI_ADJUST_CFA_OFFSET -4
	pop %ecx
	CFI_ADJUST_CFA_OFFSET -4
	ret
	CFI_ENDPROC
	ENDPROC(call_rwsem_down_read_failed)

ENTRY(call_rwsem_down_write_failed)
	CFI_STARTPROC
	push %ecx
	CFI_ADJUST_CFA_OFFSET 4
	CFI_REL_OFFSET ecx,0
	calll rwsem_down_write_failed
	pop %ecx
	CFI_ADJUST_CFA_OFFSET -4
	ret
	CFI_ENDPROC
	ENDPROC(call_rwsem_down_write_failed)

ENTRY(call_rwsem_wake)
	CFI_STARTPROC
	decw %dx    /* do nothing if still outstanding active readers */
	jnz 1f
	push %ecx
	CFI_ADJUST_CFA_OFFSET 4
	CFI_REL_OFFSET ecx,0
	call rwsem_wake
	pop %ecx
	CFI_ADJUST_CFA_OFFSET -4
1:	ret
	CFI_ENDPROC
	ENDPROC(call_rwsem_wake)

/* Fix up special calling conventions */
ENTRY(call_rwsem_downgrade_wake)
	CFI_STARTPROC
	push %ecx
	CFI_ADJUST_CFA_OFFSET 4
	CFI_REL_OFFSET ecx,0
	push %edx
	CFI_ADJUST_CFA_OFFSET 4
	CFI_REL_OFFSET edx,0
	call rwsem_downgrade_wake
	pop %edx
	CFI_ADJUST_CFA_OFFSET -4
	pop %ecx
	CFI_ADJUST_CFA_OFFSET -4
	ret
	CFI_ENDPROC
	ENDPROC(call_rwsem_downgrade_wake)

#endif
