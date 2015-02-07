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

#include <linux/slab.h>
#include <linux/completion.h>
#include <linux/irqreturn.h>
#include <asm/irq.h>
#include "irq_kern.h"
#include "os.h"

struct xterm_wait {
	struct completion ready;
	int fd;
	int pid;
	int new_fd;
};

static irqreturn_t xterm_interrupt(int irq, void *data)
{
	struct xterm_wait *xterm = data;
	int fd;

	fd = os_rcv_fd(xterm->fd, &xterm->pid);
	if (fd == -EAGAIN)
		return IRQ_NONE;

	xterm->new_fd = fd;
	complete(&xterm->ready);

	return IRQ_HANDLED;
}

int xterm_fd(int socket, int *pid_out)
{
	struct xterm_wait *data;
	int err, ret;

	data = kmalloc(sizeof(*data), GFP_KERNEL);
	if (data == NULL) {
		printk(KERN_ERR "xterm_fd : failed to allocate xterm_wait\n");
		return -ENOMEM;
	}

	/* This is a locked semaphore... */
	*data = ((struct xterm_wait) { .fd 		= socket,
				       .pid 		= -1,
				       .new_fd	 	= -1 });
	init_completion(&data->ready);

	err = um_request_irq(XTERM_IRQ, socket, IRQ_READ, xterm_interrupt,
			     IRQF_DISABLED | IRQF_SHARED | IRQF_SAMPLE_RANDOM,
			     "xterm", data);
	if (err) {
		printk(KERN_ERR "xterm_fd : failed to get IRQ for xterm, "
		       "err = %d\n",  err);
		ret = err;
		goto out;
	}

	/* ... so here we wait for an xterm interrupt.
	 *
	 * XXX Note, if the xterm doesn't work for some reason (eg. DISPLAY
	 * isn't set) this will hang... */
	wait_for_completion(&data->ready);

	free_irq(XTERM_IRQ, data);

	ret = data->new_fd;
	*pid_out = data->pid;
 out:
	kfree(data);

	return ret;
}
