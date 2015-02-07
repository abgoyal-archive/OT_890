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
 * Copyright (C) 2000 - 2007 Jeff Dike (jdike@{addtoit,linux.intel}.com)
 * Licensed under the GPL
 */

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <signal.h>
#include <string.h>
#include <termios.h>
#include <wait.h>
#include <sys/mman.h>
#include <sys/utsname.h>
#include "kern_constants.h"
#include "os.h"
#include "user.h"

void stack_protections(unsigned long address)
{
	if (mprotect((void *) address, UM_THREAD_SIZE,
		    PROT_READ | PROT_WRITE | PROT_EXEC) < 0)
		panic("protecting stack failed, errno = %d", errno);
}

int raw(int fd)
{
	struct termios tt;
	int err;

	CATCH_EINTR(err = tcgetattr(fd, &tt));
	if (err < 0)
		return -errno;

	cfmakeraw(&tt);

	CATCH_EINTR(err = tcsetattr(fd, TCSADRAIN, &tt));
	if (err < 0)
		return -errno;

	/*
	 * XXX tcsetattr could have applied only some changes
	 * (and cfmakeraw() is a set of changes)
	 */
	return 0;
}

void setup_machinename(char *machine_out)
{
	struct utsname host;

	uname(&host);
#ifdef UML_CONFIG_UML_X86
# ifndef UML_CONFIG_64BIT
	if (!strcmp(host.machine, "x86_64")) {
		strcpy(machine_out, "i686");
		return;
	}
# else
	if (!strcmp(host.machine, "i686")) {
		strcpy(machine_out, "x86_64");
		return;
	}
# endif
#endif
	strcpy(machine_out, host.machine);
}

void setup_hostinfo(char *buf, int len)
{
	struct utsname host;

	uname(&host);
	snprintf(buf, len, "%s %s %s %s %s", host.sysname, host.nodename,
		 host.release, host.version, host.machine);
}

void os_dump_core(void)
{
	int pid;

	signal(SIGSEGV, SIG_DFL);

	/*
	 * We are about to SIGTERM this entire process group to ensure that
	 * nothing is around to run after the kernel exits.  The
	 * kernel wants to abort, not die through SIGTERM, so we
	 * ignore it here.
	 */

	signal(SIGTERM, SIG_IGN);
	kill(0, SIGTERM);
	/*
	 * Most of the other processes associated with this UML are
	 * likely sTopped, so give them a SIGCONT so they see the
	 * SIGTERM.
	 */
	kill(0, SIGCONT);

	/*
	 * Now, having sent signals to everyone but us, make sure they
	 * die by ptrace.  Processes can survive what's been done to
	 * them so far - the mechanism I understand is receiving a
	 * SIGSEGV and segfaulting immediately upon return.  There is
	 * always a SIGSEGV pending, and (I'm guessing) signals are
	 * processed in numeric order so the SIGTERM (signal 15 vs
	 * SIGSEGV being signal 11) is never handled.
	 *
	 * Run a waitpid loop until we get some kind of error.
	 * Hopefully, it's ECHILD, but there's not a lot we can do if
	 * it's something else.  Tell os_kill_ptraced_process not to
	 * wait for the child to report its death because there's
	 * nothing reasonable to do if that fails.
	 */

	while ((pid = waitpid(-1, NULL, WNOHANG | __WALL)) > 0)
		os_kill_ptraced_process(pid, 0);

	abort();
}
