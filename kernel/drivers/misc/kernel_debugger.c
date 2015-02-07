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

/* drivers/android/kernel_debugger.c
 *
 * Guts of the kernel debugger.
 * Needs something to actually push commands to it.
 *
 * Copyright (C) 2007-2008 Google, Inc.
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */

#include <linux/ctype.h>
#include <linux/device.h>
#include <linux/sched.h>
#include <linux/spinlock.h>
#include <linux/sysrq.h>
#include <linux/kernel_debugger.h>

#define dprintf(fmt...) (ctxt->printf(ctxt->cookie, fmt))

static void do_ps(struct kdbg_ctxt *ctxt)
{
	struct task_struct *g, *p;
	unsigned state;
	static const char stat_nam[] = "RSDTtZX";

	dprintf("pid   ppid  prio task            pc\n");
	read_lock(&tasklist_lock);
	do_each_thread(g, p) {
		state = p->state ? __ffs(p->state) + 1 : 0;
		dprintf("%5d %5d %4d ", p->pid, p->parent->pid, p->prio);
		dprintf("%-13.13s %c", p->comm,
			state >= sizeof(stat_nam) ? '?' : stat_nam[state]);
		if (state == TASK_RUNNING)
			dprintf(" running\n");
		else
			dprintf(" %08lx\n", thread_saved_pc(p));
	} while_each_thread(g, p);
	read_unlock(&tasklist_lock);
}

int log_buf_copy(char *dest, int idx, int len);
extern int do_syslog(int type, char __user *bug, int count);
static void do_sysrq(struct kdbg_ctxt *ctxt, char rq)
{
	char buf[128];
	int ret;
	int idx = 0;
	do_syslog(5 /* clear */, NULL, 0);
	__handle_sysrq(rq, NULL, 0);
	while (1) {
		ret = log_buf_copy(buf, idx, sizeof(buf) - 1);
		if (ret <= 0)
			break;
		buf[ret] = 0;
		dprintf("%s", buf);
		idx += ret;
	}
}

int kernel_debugger(struct kdbg_ctxt *ctxt, char *cmd)
{
	if (!strcmp(cmd, "ps"))
		do_ps(ctxt);
	if (!strcmp(cmd, "sysrq"))
		do_sysrq(ctxt, 'h');
	if (!strncmp(cmd, "sysrq ", 6))
		do_sysrq(ctxt, cmd[6]);

	return 0;
}

