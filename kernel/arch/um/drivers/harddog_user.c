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

#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include "os.h"
#include "user.h"

struct dog_data {
	int stdin;
	int stdout;
	int close_me[2];
};

static void pre_exec(void *d)
{
	struct dog_data *data = d;

	dup2(data->stdin, 0);
	dup2(data->stdout, 1);
	dup2(data->stdout, 2);
	close(data->stdin);
	close(data->stdout);
	close(data->close_me[0]);
	close(data->close_me[1]);
}

int start_watchdog(int *in_fd_ret, int *out_fd_ret, char *sock)
{
	struct dog_data data;
	int in_fds[2], out_fds[2], pid, n, err;
	char pid_buf[sizeof("nnnnn\0")], c;
	char *pid_args[] = { "/usr/bin/uml_watchdog", "-pid", pid_buf, NULL };
	char *mconsole_args[] = { "/usr/bin/uml_watchdog", "-mconsole", NULL,
				  NULL };
	char **args = NULL;

	err = os_pipe(in_fds, 1, 0);
	if (err < 0) {
		printk("harddog_open - os_pipe failed, err = %d\n", -err);
		goto out;
	}

	err = os_pipe(out_fds, 1, 0);
	if (err < 0) {
		printk("harddog_open - os_pipe failed, err = %d\n", -err);
		goto out_close_in;
	}

	data.stdin = out_fds[0];
	data.stdout = in_fds[1];
	data.close_me[0] = out_fds[1];
	data.close_me[1] = in_fds[0];

	if (sock != NULL) {
		mconsole_args[2] = sock;
		args = mconsole_args;
	}
	else {
		/* XXX The os_getpid() is not SMP correct */
		sprintf(pid_buf, "%d", os_getpid());
		args = pid_args;
	}

	pid = run_helper(pre_exec, &data, args);

	close(out_fds[0]);
	close(in_fds[1]);

	if (pid < 0) {
		err = -pid;
		printk("harddog_open - run_helper failed, errno = %d\n", -err);
		goto out_close_out;
	}

	n = read(in_fds[0], &c, sizeof(c));
	if (n == 0) {
		printk("harddog_open - EOF on watchdog pipe\n");
		helper_wait(pid);
		err = -EIO;
		goto out_close_out;
	}
	else if (n < 0) {
		printk("harddog_open - read of watchdog pipe failed, "
		       "err = %d\n", errno);
		helper_wait(pid);
		err = n;
		goto out_close_out;
	}
	*in_fd_ret = in_fds[0];
	*out_fd_ret = out_fds[1];
	return 0;

 out_close_in:
	close(in_fds[0]);
	close(in_fds[1]);
 out_close_out:
	close(out_fds[0]);
	close(out_fds[1]);
 out:
	return err;
}

void stop_watchdog(int in_fd, int out_fd)
{
	close(in_fd);
	close(out_fd);
}

int ping_watchdog(int fd)
{
	int n;
	char c = '\n';

	n = write(fd, &c, sizeof(c));
	if (n != sizeof(c)) {
		printk("ping_watchdog - write failed, ret = %d, err = %d\n",
		       n, errno);
		if (n < 0)
			return n;
		return -EIO;
	}
	return 1;

}
