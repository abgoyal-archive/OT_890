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

#include <stdlib.h>
#include <errno.h>
#include <poll.h>
#include <signal.h>
#include <string.h>
#include "irq_user.h"
#include "kern_constants.h"
#include "os.h"
#include "process.h"
#include "um_malloc.h"
#include "user.h"

/*
 * Locked by irq_lock in arch/um/kernel/irq.c.  Changed by os_create_pollfd
 * and os_free_irq_by_cb, which are called under irq_lock.
 */
static struct pollfd *pollfds = NULL;
static int pollfds_num = 0;
static int pollfds_size = 0;

int os_waiting_for_events(struct irq_fd *active_fds)
{
	struct irq_fd *irq_fd;
	int i, n, err;

	n = poll(pollfds, pollfds_num, 0);
	if (n < 0) {
		err = -errno;
		if (errno != EINTR)
			printk(UM_KERN_ERR "os_waiting_for_events:"
			       " poll returned %d, errno = %d\n", n, errno);
		return err;
	}

	if (n == 0)
		return 0;

	irq_fd = active_fds;

	for (i = 0; i < pollfds_num; i++) {
		if (pollfds[i].revents != 0) {
			irq_fd->current_events = pollfds[i].revents;
			pollfds[i].fd = -1;
		}
		irq_fd = irq_fd->next;
	}
	return n;
}

int os_create_pollfd(int fd, int events, void *tmp_pfd, int size_tmpfds)
{
	if (pollfds_num == pollfds_size) {
		if (size_tmpfds <= pollfds_size * sizeof(pollfds[0])) {
			/* return min size needed for new pollfds area */
			return (pollfds_size + 1) * sizeof(pollfds[0]);
		}

		if (pollfds != NULL) {
			memcpy(tmp_pfd, pollfds,
			       sizeof(pollfds[0]) * pollfds_size);
			/* remove old pollfds */
			kfree(pollfds);
		}
		pollfds = tmp_pfd;
		pollfds_size++;
	} else
		kfree(tmp_pfd);	/* remove not used tmp_pfd */

	pollfds[pollfds_num] = ((struct pollfd) { .fd		= fd,
						  .events	= events,
						  .revents	= 0 });
	pollfds_num++;

	return 0;
}

void os_free_irq_by_cb(int (*test)(struct irq_fd *, void *), void *arg,
		struct irq_fd *active_fds, struct irq_fd ***last_irq_ptr2)
{
	struct irq_fd **prev;
	int i = 0;

	prev = &active_fds;
	while (*prev != NULL) {
		if ((*test)(*prev, arg)) {
			struct irq_fd *old_fd = *prev;
			if ((pollfds[i].fd != -1) &&
			    (pollfds[i].fd != (*prev)->fd)) {
				printk(UM_KERN_ERR "os_free_irq_by_cb - "
				       "mismatch between active_fds and "
				       "pollfds, fd %d vs %d\n",
				       (*prev)->fd, pollfds[i].fd);
				goto out;
			}

			pollfds_num--;

			/*
			 * This moves the *whole* array after pollfds[i]
			 * (though it doesn't spot as such)!
			 */
			memmove(&pollfds[i], &pollfds[i + 1],
			       (pollfds_num - i) * sizeof(pollfds[0]));
			if (*last_irq_ptr2 == &old_fd->next)
				*last_irq_ptr2 = prev;

			*prev = (*prev)->next;
			if (old_fd->type == IRQ_WRITE)
				ignore_sigio_fd(old_fd->fd);
			kfree(old_fd);
			continue;
		}
		prev = &(*prev)->next;
		i++;
	}
 out:
	return;
}

int os_get_pollfd(int i)
{
	return pollfds[i].fd;
}

void os_set_pollfd(int i, int fd)
{
	pollfds[i].fd = fd;
}

void os_set_ioignore(void)
{
	signal(SIGIO, SIG_IGN);
}
