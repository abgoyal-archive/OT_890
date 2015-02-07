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
 * Licensed under the GPL.
 */

#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/wait.h>
#include "kern_constants.h"
#include "net_user.h"
#include "os.h"
#include "slirp.h"
#include "user.h"

static int slirp_user_init(void *data, void *dev)
{
	struct slirp_data *pri = data;

	pri->dev = dev;
	return 0;
}

struct slirp_pre_exec_data {
	int stdin;
	int stdout;
};

static void slirp_pre_exec(void *arg)
{
	struct slirp_pre_exec_data *data = arg;

	if (data->stdin != -1)
		dup2(data->stdin, 0);
	if (data->stdout != -1)
		dup2(data->stdout, 1);
}

static int slirp_tramp(char **argv, int fd)
{
	struct slirp_pre_exec_data pe_data;
	int pid;

	pe_data.stdin = fd;
	pe_data.stdout = fd;
	pid = run_helper(slirp_pre_exec, &pe_data, argv);

	return pid;
}

static int slirp_open(void *data)
{
	struct slirp_data *pri = data;
	int fds[2], pid, err;

	err = os_pipe(fds, 1, 1);
	if (err)
		return err;

	err = slirp_tramp(pri->argw.argv, fds[1]);
	if (err < 0) {
		printk(UM_KERN_ERR "slirp_tramp failed - errno = %d\n", -err);
		goto out;
	}
	pid = err;

	pri->slave = fds[1];
	pri->slip.pos = 0;
	pri->slip.esc = 0;
	pri->pid = err;

	return fds[0];
out:
	close(fds[0]);
	close(fds[1]);
	return err;
}

static void slirp_close(int fd, void *data)
{
	struct slirp_data *pri = data;
	int err;

	close(fd);
	close(pri->slave);

	pri->slave = -1;

	if (pri->pid<1) {
		printk(UM_KERN_ERR "slirp_close: no child process to shut "
		       "down\n");
		return;
	}

#if 0
	if (kill(pri->pid, SIGHUP)<0) {
		printk(UM_KERN_ERR "slirp_close: sending hangup to %d failed "
		       "(%d)\n", pri->pid, errno);
	}
#endif
	err = helper_wait(pri->pid);
	if (err < 0)
		return;

	pri->pid = -1;
}

int slirp_user_read(int fd, void *buf, int len, struct slirp_data *pri)
{
	return slip_proto_read(fd, buf, len, &pri->slip);
}

int slirp_user_write(int fd, void *buf, int len, struct slirp_data *pri)
{
	return slip_proto_write(fd, buf, len, &pri->slip);
}

const struct net_user_info slirp_user_info = {
	.init		= slirp_user_init,
	.open		= slirp_open,
	.close	 	= slirp_close,
	.remove	 	= NULL,
	.add_address	= NULL,
	.delete_address = NULL,
	.mtu		= BUF_SIZE,
	.max_packet	= BUF_SIZE,
};
