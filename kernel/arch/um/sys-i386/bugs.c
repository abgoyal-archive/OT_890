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
 * Copyright (C) 2002 - 2007 Jeff Dike (jdike@{addtoit,linux.intel}.com)
 * Licensed under the GPL
 */

#include <signal.h>
#include "kern_constants.h"
#include "kern_util.h"
#include "longjmp.h"
#include "task.h"
#include "user.h"
#include "sysdep/ptrace.h"

/* Set during early boot */
static int host_has_cmov = 1;
static jmp_buf cmov_test_return;

static void cmov_sigill_test_handler(int sig)
{
	host_has_cmov = 0;
	longjmp(cmov_test_return, 1);
}

void arch_check_bugs(void)
{
	struct sigaction old, new;

	printk(UM_KERN_INFO "Checking for host processor cmov support...");
	new.sa_handler = cmov_sigill_test_handler;

	/* Make sure that SIGILL is enabled after the handler longjmps back */
	new.sa_flags = SA_NODEFER;
	sigemptyset(&new.sa_mask);
	sigaction(SIGILL, &new, &old);

	if (setjmp(cmov_test_return) == 0) {
		unsigned long foo = 0;
		__asm__ __volatile__("cmovz %0, %1" : "=r" (foo) : "0" (foo));
		printk(UM_KERN_CONT "Yes\n");
	} else
		printk(UM_KERN_CONT "No\n");

	sigaction(SIGILL, &old, &new);
}

void arch_examine_signal(int sig, struct uml_pt_regs *regs)
{
	unsigned char tmp[2];

	/*
	 * This is testing for a cmov (0x0f 0x4x) instruction causing a
	 * SIGILL in init.
	 */
	if ((sig != SIGILL) || (TASK_PID(get_current()) != 1))
		return;

	if (copy_from_user_proc(tmp, (void *) UPT_IP(regs), 2)) {
		printk(UM_KERN_ERR "SIGILL in init, could not read "
		       "instructions!\n");
		return;
	}

	if ((tmp[0] != 0x0f) || ((tmp[1] & 0xf0) != 0x40))
		return;

	if (host_has_cmov == 0)
		printk(UM_KERN_ERR "SIGILL caused by cmov, which this "
		       "processor doesn't implement.  Boot a filesystem "
		       "compiled for older processors");
	else if (host_has_cmov == 1)
		printk(UM_KERN_ERR "SIGILL caused by cmov, which this "
		       "processor claims to implement");
	else
		printk(UM_KERN_ERR "Bad value for host_has_cmov (%d)",
			host_has_cmov);
}
