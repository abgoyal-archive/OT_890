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
 * Copyright (C) 2000, 2001 Jeff Dike (jdike@karaya.com)
 * Licensed under the GPL
 */

#ifndef __CHAN_USER_H__
#define __CHAN_USER_H__

#include "init.h"

struct chan_opts {
	void (*const announce)(char *dev_name, int dev);
	char *xterm_title;
	const int raw;
};

enum chan_init_pri { INIT_STATIC, INIT_ALL, INIT_ONE };

struct chan_ops {
	char *type;
	void *(*init)(char *, int, const struct chan_opts *);
	int (*open)(int, int, int, void *, char **);
	void (*close)(int, void *);
	int (*read)(int, char *, void *);
	int (*write)(int, const char *, int, void *);
	int (*console_write)(int, const char *, int);
	int (*window_size)(int, void *, unsigned short *, unsigned short *);
	void (*free)(void *);
	int winch;
};

extern const struct chan_ops fd_ops, null_ops, port_ops, pts_ops, pty_ops,
	tty_ops, xterm_ops;

extern void generic_close(int fd, void *unused);
extern int generic_read(int fd, char *c_out, void *unused);
extern int generic_write(int fd, const char *buf, int n, void *unused);
extern int generic_console_write(int fd, const char *buf, int n);
extern int generic_window_size(int fd, void *unused, unsigned short *rows_out,
			       unsigned short *cols_out);
extern void generic_free(void *data);

struct tty_struct;
extern void register_winch(int fd,  struct tty_struct *tty);
extern void register_winch_irq(int fd, int tty_fd, int pid,
			       struct tty_struct *tty, unsigned long stack);

#define __channel_help(fn, prefix) \
__uml_help(fn, prefix "[0-9]*=<channel description>\n" \
"    Attach a console or serial line to a host channel.  See\n" \
"    http://user-mode-linux.sourceforge.net/old/input.html for a complete\n" \
"    description of this switch.\n\n" \
);

#endif
