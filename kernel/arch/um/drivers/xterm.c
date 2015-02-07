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
 * Copyright (C) 2001 - 2007 Jeff Dike (jdike@{addtoit,linux.intel}.com)
 * Licensed under the GPL
 */

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <termios.h>
#include "chan_user.h"
#include "kern_constants.h"
#include "os.h"
#include "um_malloc.h"
#include "user.h"
#include "xterm.h"

struct xterm_chan {
	int pid;
	int helper_pid;
	char *title;
	int device;
	int raw;
	struct termios tt;
};

static void *xterm_init(char *str, int device, const struct chan_opts *opts)
{
	struct xterm_chan *data;

	data = uml_kmalloc(sizeof(*data), UM_GFP_KERNEL);
	if (data == NULL)
		return NULL;
	*data = ((struct xterm_chan) { .pid 		= -1,
				       .helper_pid 	= -1,
				       .device 		= device,
				       .title 		= opts->xterm_title,
				       .raw  		= opts->raw } );
	return data;
}

/* Only changed by xterm_setup, which is a setup */
static char *terminal_emulator = "xterm";
static char *title_switch = "-T";
static char *exec_switch = "-e";

static int __init xterm_setup(char *line, int *add)
{
	*add = 0;
	terminal_emulator = line;

	line = strchr(line, ',');
	if (line == NULL)
		return 0;

	*line++ = '\0';
	if (*line)
		title_switch = line;

	line = strchr(line, ',');
	if (line == NULL)
		return 0;

	*line++ = '\0';
	if (*line)
		exec_switch = line;

	return 0;
}

__uml_setup("xterm=", xterm_setup,
"xterm=<terminal emulator>,<title switch>,<exec switch>\n"
"    Specifies an alternate terminal emulator to use for the debugger,\n"
"    consoles, and serial lines when they are attached to the xterm channel.\n"
"    The values are the terminal emulator binary, the switch it uses to set\n"
"    its title, and the switch it uses to execute a subprocess,\n"
"    respectively.  The title switch must have the form '<switch> title',\n"
"    not '<switch>=title'.  Similarly, the exec switch must have the form\n"
"    '<switch> command arg1 arg2 ...'.\n"
"    The default values are 'xterm=xterm,-T,-e'.  Values for gnome-terminal\n"
"    are 'xterm=gnome-terminal,-t,-x'.\n\n"
);

static int xterm_open(int input, int output, int primary, void *d,
		      char **dev_out)
{
	struct xterm_chan *data = d;
	int pid, fd, new, err;
	char title[256], file[] = "/tmp/xterm-pipeXXXXXX";
	char *argv[] = { terminal_emulator, title_switch, title, exec_switch,
			 "/usr/lib/uml/port-helper", "-uml-socket",
			 file, NULL };

	if (access(argv[4], X_OK) < 0)
		argv[4] = "port-helper";

	/*
	 * Check that DISPLAY is set, this doesn't guarantee the xterm
	 * will work but w/o it we can be pretty sure it won't.
	 */
	if (getenv("DISPLAY") == NULL) {
		printk(UM_KERN_ERR "xterm_open: $DISPLAY not set.\n");
		return -ENODEV;
	}

	/*
	 * This business of getting a descriptor to a temp file,
	 * deleting the file and closing the descriptor is just to get
	 * a known-unused name for the Unix socket that we really
	 * want.
	 */
	fd = mkstemp(file);
	if (fd < 0) {
		err = -errno;
		printk(UM_KERN_ERR "xterm_open : mkstemp failed, errno = %d\n",
		       errno);
		return err;
	}

	if (unlink(file)) {
		err = -errno;
		printk(UM_KERN_ERR "xterm_open : unlink failed, errno = %d\n",
		       errno);
		return err;
	}
	close(fd);

	fd = os_create_unix_socket(file, sizeof(file), 1);
	if (fd < 0) {
		printk(UM_KERN_ERR "xterm_open : create_unix_socket failed, "
		       "errno = %d\n", -fd);
		return fd;
	}

	sprintf(title, data->title, data->device);
	pid = run_helper(NULL, NULL, argv);
	if (pid < 0) {
		err = pid;
		printk(UM_KERN_ERR "xterm_open : run_helper failed, "
		       "errno = %d\n", -err);
		goto out_close1;
	}

	err = os_set_fd_block(fd, 0);
	if (err < 0) {
		printk(UM_KERN_ERR "xterm_open : failed to set descriptor "
		       "non-blocking, err = %d\n", -err);
		goto out_kill;
	}

	new = xterm_fd(fd, &data->helper_pid);
	if (new < 0) {
		err = new;
		printk(UM_KERN_ERR "xterm_open : os_rcv_fd failed, err = %d\n",
		       -err);
		goto out_kill;
	}

	err = os_set_fd_block(new, 0);
	if (err) {
		printk(UM_KERN_ERR "xterm_open : failed to set xterm "
		       "descriptor non-blocking, err = %d\n", -err);
		goto out_close2;
	}

	CATCH_EINTR(err = tcgetattr(new, &data->tt));
	if (err) {
		new = err;
		goto out_close2;
	}

	if (data->raw) {
		err = raw(new);
		if (err) {
			new = err;
			goto out_close2;
		}
	}

	unlink(file);
	data->pid = pid;
	*dev_out = NULL;

	return new;

 out_close2:
	close(new);
 out_kill:
	os_kill_process(pid, 1);
 out_close1:
	close(fd);

	return err;
}

static void xterm_close(int fd, void *d)
{
	struct xterm_chan *data = d;

	if (data->pid != -1)
		os_kill_process(data->pid, 1);
	data->pid = -1;

	if (data->helper_pid != -1)
		os_kill_process(data->helper_pid, 0);
	data->helper_pid = -1;

	os_close_file(fd);
}

const struct chan_ops xterm_ops = {
	.type		= "xterm",
	.init		= xterm_init,
	.open		= xterm_open,
	.close		= xterm_close,
	.read		= generic_read,
	.write		= generic_write,
	.console_write	= generic_console_write,
	.window_size	= generic_window_size,
	.free		= generic_free,
	.winch		= 1,
};
