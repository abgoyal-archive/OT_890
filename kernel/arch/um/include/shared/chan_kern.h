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

#ifndef __CHAN_KERN_H__
#define __CHAN_KERN_H__

#include "linux/tty.h"
#include "linux/list.h"
#include "linux/console.h"
#include "chan_user.h"
#include "line.h"

struct chan {
	struct list_head list;
	struct list_head free_list;
	struct line *line;
	char *dev;
	unsigned int primary:1;
	unsigned int input:1;
	unsigned int output:1;
	unsigned int opened:1;
	unsigned int enabled:1;
	int fd;
	const struct chan_ops *ops;
	void *data;
};

extern void chan_interrupt(struct list_head *chans, struct delayed_work *task,
			   struct tty_struct *tty, int irq);
extern int parse_chan_pair(char *str, struct line *line, int device,
			   const struct chan_opts *opts, char **error_out);
extern int write_chan(struct list_head *chans, const char *buf, int len,
			     int write_irq);
extern int console_write_chan(struct list_head *chans, const char *buf, 
			      int len);
extern int console_open_chan(struct line *line, struct console *co);
extern void deactivate_chan(struct list_head *chans, int irq);
extern void reactivate_chan(struct list_head *chans, int irq);
extern void chan_enable_winch(struct list_head *chans, struct tty_struct *tty);
extern int enable_chan(struct line *line);
extern void close_chan(struct list_head *chans, int delay_free_irq);
extern int chan_window_size(struct list_head *chans, 
			     unsigned short *rows_out, 
			     unsigned short *cols_out);
extern int chan_config_string(struct list_head *chans, char *str, int size,
			      char **error_out);

#endif
