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

/* kernel_thread.S: kernel thread creation
 *
 * Copyright (C) 2003 Red Hat, Inc. All Rights Reserved.
 * Written by David Howells (dhowells@redhat.com)
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version
 * 2 of the License, or (at your option) any later version.
 */

#include <linux/linkage.h>
#include <asm/unistd.h>

#define CLONE_VM	0x00000100	/* set if VM shared between processes */
#define	KERN_ERR	"<3>"

	.section .rodata
kernel_thread_emsg:
	.asciz	KERN_ERR "failed to create kernel thread: error=%d\n"

	.text
	.balign		4

###############################################################################
#
# Create a kernel thread
#
# int kernel_thread(int (*fn)(void *), void * arg, unsigned long flags)
#
###############################################################################
	.globl		kernel_thread
	.type		kernel_thread,@function
kernel_thread:
	or.p		gr8,gr0,gr4
	or		gr9,gr0,gr5

	# start by forking the current process, but with shared VM
	setlos.p	#__NR_clone,gr7		; syscall number
	ori		gr10,#CLONE_VM,gr8	; first syscall arg	[clone_flags]
	sethi.p		#0xe4e4,gr9		; second syscall arg	[newsp]
	setlo		#0xe4e4,gr9
	setlos.p	#0,gr10			; third syscall arg	[parent_tidptr]
	setlos		#0,gr11			; fourth syscall arg	[child_tidptr]
	tira		gr0,#0
	setlos.p	#4095,gr7
	andcc		gr8,gr8,gr0,icc0
	addcc.p		gr8,gr7,gr0,icc1
	bnelr		icc0,#2
	bc		icc1,#0,kernel_thread_error

	# now invoke the work function
	or		gr5,gr0,gr8
	calll		@(gr4,gr0)

	# and finally exit the thread
	setlos		#__NR_exit,gr7		; syscall number
	tira		gr0,#0

kernel_thread_error:
	subi		sp,#8,sp
	movsg		lr,gr4
	sti		gr8,@(sp,#0)
	sti.p		gr4,@(sp,#4)

	or		gr8,gr0,gr9
	sethi.p		%hi(kernel_thread_emsg),gr8
	setlo		%lo(kernel_thread_emsg),gr8

	call		printk

	ldi		@(sp,#4),gr4
	ldi		@(sp,#0),gr8
	subi		sp,#8,sp
	jmpl		@(gr4,gr0)

	.size		kernel_thread,.-kernel_thread
