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
 * Copyright (C) 2007 Jeff Dike (jdike@{addtoit,linux.intel}.com)
 * Licensed under the GPL
 */

#include <signal.h>
#include <sched.h>
#include <asm/unistd.h>
#include <sys/time.h>
#include "as-layout.h"
#include "kern_constants.h"
#include "ptrace_user.h"
#include "stub-data.h"
#include "sysdep/stub.h"

/*
 * This is in a separate file because it needs to be compiled with any
 * extraneous gcc flags (-pg, -fprofile-arcs, -ftest-coverage) disabled
 *
 * Use UM_KERN_PAGE_SIZE instead of PAGE_SIZE because that calls getpagesize
 * on some systems.
 */

void __attribute__ ((__section__ (".__syscall_stub")))
stub_clone_handler(void)
{
	struct stub_data *data = (struct stub_data *) STUB_DATA;
	long err;

	err = stub_syscall2(__NR_clone, CLONE_PARENT | CLONE_FILES | SIGCHLD,
			    STUB_DATA + UM_KERN_PAGE_SIZE / 2 - sizeof(void *));
	if (err != 0)
		goto out;

	err = stub_syscall4(__NR_ptrace, PTRACE_TRACEME, 0, 0, 0);
	if (err)
		goto out;

	err = stub_syscall3(__NR_setitimer, ITIMER_VIRTUAL,
			    (long) &data->timer, 0);
	if (err)
		goto out;

	remap_stack(data->fd, data->offset);
	goto done;

 out:
	/*
	 * save current result.
	 * Parent: pid;
	 * child: retcode of mmap already saved and it jumps around this
	 * assignment
	 */
	data->err = err;
 done:
	trap_myself();
}
