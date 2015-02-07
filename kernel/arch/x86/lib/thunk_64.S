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
 * Save registers before calling assembly functions. This avoids
 * disturbance of register allocation in some inline assembly constructs.
 * Copyright 2001,2002 by Andi Kleen, SuSE Labs.
 * Added trace_hardirqs callers - Copyright 2007 Steven Rostedt, Red Hat, Inc.
 * Subject to the GNU public license, v.2. No warranty of any kind.
 */

	#include <linux/linkage.h>
	#include <asm/dwarf2.h>
	#include <asm/calling.h>			
	#include <asm/rwlock.h>
		
	/* rdi:	arg1 ... normal C conventions. rax is saved/restored. */ 	
	.macro thunk name,func
	.globl \name
\name:	
	CFI_STARTPROC
	SAVE_ARGS
	call \func
	jmp  restore
	CFI_ENDPROC
	.endm

	/* rdi:	arg1 ... normal C conventions. rax is passed from C. */ 	
	.macro thunk_retrax name,func
	.globl \name
\name:	
	CFI_STARTPROC
	SAVE_ARGS
	call \func
	jmp  restore_norax
	CFI_ENDPROC
	.endm
	

	.section .sched.text, "ax"
#ifdef CONFIG_RWSEM_XCHGADD_ALGORITHM
	thunk rwsem_down_read_failed_thunk,rwsem_down_read_failed
	thunk rwsem_down_write_failed_thunk,rwsem_down_write_failed
	thunk rwsem_wake_thunk,rwsem_wake
	thunk rwsem_downgrade_thunk,rwsem_downgrade_wake
#endif	
	
#ifdef CONFIG_TRACE_IRQFLAGS
	/* put return address in rdi (arg1) */
	.macro thunk_ra name,func
	.globl \name
\name:
	CFI_STARTPROC
	SAVE_ARGS
	/* SAVE_ARGS pushs 9 elements */
	/* the next element would be the rip */
	movq 9*8(%rsp), %rdi
	call \func
	jmp  restore
	CFI_ENDPROC
	.endm

	thunk_ra trace_hardirqs_on_thunk,trace_hardirqs_on_caller
	thunk_ra trace_hardirqs_off_thunk,trace_hardirqs_off_caller
#endif

#ifdef CONFIG_DEBUG_LOCK_ALLOC
	thunk lockdep_sys_exit_thunk,lockdep_sys_exit
#endif
	
	/* SAVE_ARGS below is used only for the .cfi directives it contains. */
	CFI_STARTPROC
	SAVE_ARGS
restore:
	RESTORE_ARGS
	ret	
	CFI_ENDPROC
	
	CFI_STARTPROC
	SAVE_ARGS
restore_norax:	
	RESTORE_ARGS 1
	ret
	CFI_ENDPROC
