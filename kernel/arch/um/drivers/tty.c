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

#include <errno.h>
#include <fcntl.h>
#include <termios.h>
#include "chan_user.h"
#include "kern_constants.h"
#include "os.h"
#include "um_malloc.h"
#include "user.h"

struct tty_chan {
	char *dev;
	int raw;
	struct termios tt;
};

static void *tty_chan_init(char *str, int device, const struct chan_opts *opts)
{
	struct tty_chan *data;

	if (*str != ':') {
		printk(UM_KERN_ERR "tty_init : channel type 'tty' must specify "
		       "a device\n");
		return NULL;
	}
	str++;

	data = uml_kmalloc(sizeof(*data), UM_GFP_KERNEL);
	if (data == NULL)
		return NULL;
	*data = ((struct tty_chan) { .dev 	= str,
				     .raw 	= opts->raw });

	return data;
}

static int tty_open(int input, int output, int primary, void *d,
		    char **dev_out)
{
	struct tty_chan *data = d;
	int fd, err, mode = 0;

	if (input && output)
		mode = O_RDWR;
	else if (input)
		mode = O_RDONLY;
	else if (output)
		mode = O_WRONLY;

	fd = open(data->dev, mode);
	if (fd < 0)
		return -errno;

	if (data->raw) {
		CATCH_EINTR(err = tcgetattr(fd, &data->tt));
		if (err)
			return err;

		err = raw(fd);
		if (err)
			return err;
	}

	*dev_out = data->dev;
	return fd;
}

const struct chan_ops tty_ops = {
	.type		= "tty",
	.init		= tty_chan_init,
	.open		= tty_open,
	.close		= generic_close,
	.read		= generic_read,
	.write		= generic_write,
	.console_write	= generic_console_write,
	.window_size	= generic_window_size,
	.free		= generic_free,
	.winch		= 0,
};
