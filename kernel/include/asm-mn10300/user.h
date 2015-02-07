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

/* MN10300 User process data
 *
 * Copyright (C) 2007 Matsushita Electric Industrial Co., Ltd.
 * Copyright (C) 2007 Red Hat, Inc. All Rights Reserved.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public Licence
 * as published by the Free Software Foundation; either version
 * 2 of the Licence, or (at your option) any later version.
 */
#ifndef _ASM_USER_H
#define _ASM_USER_H

#include <asm/page.h>
#include <linux/ptrace.h>

#ifndef __ASSEMBLY__
/*
 * When the kernel dumps core, it starts by dumping the user struct - this will
 * be used by gdb to figure out where the data and stack segments are within
 * the file, and what virtual addresses to use.
 */
struct user {
	/* We start with the registers, to mimic the way that "memory" is
	 * returned from the ptrace(3,...) function.
	 */
	struct pt_regs regs;		/* Where the registers are actually stored */

	/* The rest of this junk is to help gdb figure out what goes where */
	unsigned long int u_tsize;	/* Text segment size (pages). */
	unsigned long int u_dsize;	/* Data segment size (pages). */
	unsigned long int u_ssize;	/* Stack segment size (pages). */
	unsigned long start_code;	/* Starting virtual address of text. */
	unsigned long start_stack;	/* Starting virtual address of stack area.
					   This is actually the bottom of the stack,
					   the top of the stack is always found in the
					   esp register.  */
	long int signal;		/* Signal that caused the core dump. */
	int reserved;			/* No longer used */
	struct user_pt_regs *u_ar0;	/* Used by gdb to help find the values for */

	/* the registers */
	unsigned long magic;		/* To uniquely identify a core file */
	char u_comm[32];		/* User command that was responsible */
};
#endif

#define NBPG PAGE_SIZE
#define UPAGES 1
#define HOST_TEXT_START_ADDR	+(u.start_code)
#define HOST_STACK_END_ADDR	+(u.start_stack + u.u_ssize * NBPG)

#endif /* _ASM_USER_H */
