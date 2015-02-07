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
 * Copyright (C) 2001 - 2007 Jeff Dike (jdike@{linux.intel,addtoit}.com)
 * Licensed under the GPL
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <termios.h>
#include "chan_user.h"
#include "kern_constants.h"
#include "os.h"
#include "um_malloc.h"
#include "user.h"

struct fd_chan {
	int fd;
	int raw;
	struct termios tt;
	char str[sizeof("1234567890\0")];
};

static void *fd_init(char *str, int device, const struct chan_opts *opts)
{
	struct fd_chan *data;
	char *end;
	int n;

	if (*str != ':') {
		printk(UM_KERN_ERR "fd_init : channel type 'fd' must specify a "
		       "file descriptor\n");
		return NULL;
	}
	str++;
	n = strtoul(str, &end, 0);
	if ((*end != '\0') || (end == str)) {
		printk(UM_KERN_ERR "fd_init : couldn't parse file descriptor "
		       "'%s'\n", str);
		return NULL;
	}

	data = uml_kmalloc(sizeof(*data), UM_GFP_KERNEL);
	if (data == NULL)
		return NULL;

	*data = ((struct fd_chan) { .fd  	= n,
				    .raw  	= opts->raw });
	return data;
}

static int fd_open(int input, int output, int primary, void *d, char **dev_out)
{
	struct fd_chan *data = d;
	int err;

	if (data->raw && isatty(data->fd)) {
		CATCH_EINTR(err = tcgetattr(data->fd, &data->tt));
		if (err)
			return err;

		err = raw(data->fd);
		if (err)
			return err;
	}
	sprintf(data->str, "%d", data->fd);
	*dev_out = data->str;
	return data->fd;
}

static void fd_close(int fd, void *d)
{
	struct fd_chan *data = d;
	int err;

	if (!data->raw || !isatty(fd))
		return;

	CATCH_EINTR(err = tcsetattr(fd, TCSAFLUSH, &data->tt));
	if (err)
		printk(UM_KERN_ERR "Failed to restore terminal state - "
		       "errno = %d\n", -err);
	data->raw = 0;
}

const struct chan_ops fd_ops = {
	.type		= "fd",
	.init		= fd_init,
	.open		= fd_open,
	.close		= fd_close,
	.read		= generic_read,
	.write		= generic_write,
	.console_write	= generic_console_write,
	.window_size	= generic_window_size,
	.free		= generic_free,
	.winch		= 1,
};
