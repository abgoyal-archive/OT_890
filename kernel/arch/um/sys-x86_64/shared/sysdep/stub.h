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
 * Copyright (C) 2004 Jeff Dike (jdike@addtoit.com)
 * Licensed under the GPL
 */

#ifndef __SYSDEP_STUB_H
#define __SYSDEP_STUB_H

#include <sys/mman.h>
#include <asm/unistd.h>
#include <sysdep/ptrace_user.h>
#include "as-layout.h"
#include "stub-data.h"
#include "kern_constants.h"

extern void stub_segv_handler(int sig);
extern void stub_clone_handler(void);

#define STUB_SYSCALL_RET PT_INDEX(RAX)
#define STUB_MMAP_NR __NR_mmap
#define MMAP_OFFSET(o) (o)

#define __syscall_clobber "r11","rcx","memory"
#define __syscall "syscall"

static inline long stub_syscall0(long syscall)
{
	long ret;

	__asm__ volatile (__syscall
		: "=a" (ret)
		: "0" (syscall) : __syscall_clobber );

	return ret;
}

static inline long stub_syscall2(long syscall, long arg1, long arg2)
{
	long ret;

	__asm__ volatile (__syscall
		: "=a" (ret)
		: "0" (syscall), "D" (arg1), "S" (arg2) : __syscall_clobber );

	return ret;
}

static inline long stub_syscall3(long syscall, long arg1, long arg2, long arg3)
{
	long ret;

	__asm__ volatile (__syscall
		: "=a" (ret)
		: "0" (syscall), "D" (arg1), "S" (arg2), "d" (arg3)
		: __syscall_clobber );

	return ret;
}

static inline long stub_syscall4(long syscall, long arg1, long arg2, long arg3,
				 long arg4)
{
	long ret;

	__asm__ volatile ("movq %5,%%r10 ; " __syscall
		: "=a" (ret)
		: "0" (syscall), "D" (arg1), "S" (arg2), "d" (arg3),
		  "g" (arg4)
		: __syscall_clobber, "r10" );

	return ret;
}

static inline long stub_syscall5(long syscall, long arg1, long arg2, long arg3,
				 long arg4, long arg5)
{
	long ret;

	__asm__ volatile ("movq %5,%%r10 ; movq %6,%%r8 ; " __syscall
		: "=a" (ret)
		: "0" (syscall), "D" (arg1), "S" (arg2), "d" (arg3),
		  "g" (arg4), "g" (arg5)
		: __syscall_clobber, "r10", "r8" );

	return ret;
}

static inline void trap_myself(void)
{
	__asm("int3");
}

static inline void remap_stack(long fd, unsigned long offset)
{
	__asm__ volatile ("movq %4,%%r10 ; movq %5,%%r8 ; "
			  "movq %6, %%r9; " __syscall "; movq %7, %%rbx ; "
			  "movq %%rax, (%%rbx)":
			  : "a" (STUB_MMAP_NR), "D" (STUB_DATA),
			    "S" (UM_KERN_PAGE_SIZE),
			    "d" (PROT_READ | PROT_WRITE),
                            "g" (MAP_FIXED | MAP_SHARED), "g" (fd),
			    "g" (offset),
			    "i" (&((struct stub_data *) STUB_DATA)->err)
			  : __syscall_clobber, "r10", "r8", "r9" );
}

#endif
