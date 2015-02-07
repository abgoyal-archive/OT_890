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
 * linux/net/sunrpc/sysctl.c
 *
 * Sysctl interface to sunrpc module.
 *
 * I would prefer to register the sunrpc table below sys/net, but that's
 * impossible at the moment.
 */

#include <linux/types.h>
#include <linux/linkage.h>
#include <linux/ctype.h>
#include <linux/fs.h>
#include <linux/sysctl.h>
#include <linux/module.h>

#include <asm/uaccess.h>
#include <linux/sunrpc/types.h>
#include <linux/sunrpc/sched.h>
#include <linux/sunrpc/stats.h>
#include <linux/sunrpc/svc_xprt.h>

/*
 * Declare the debug flags here
 */
unsigned int	rpc_debug;
EXPORT_SYMBOL_GPL(rpc_debug);

unsigned int	nfs_debug;
EXPORT_SYMBOL_GPL(nfs_debug);

unsigned int	nfsd_debug;
EXPORT_SYMBOL_GPL(nfsd_debug);

unsigned int	nlm_debug;
EXPORT_SYMBOL_GPL(nlm_debug);

#ifdef RPC_DEBUG

static struct ctl_table_header *sunrpc_table_header;
static ctl_table		sunrpc_table[];

void
rpc_register_sysctl(void)
{
	if (!sunrpc_table_header)
		sunrpc_table_header = register_sysctl_table(sunrpc_table);
}

void
rpc_unregister_sysctl(void)
{
	if (sunrpc_table_header) {
		unregister_sysctl_table(sunrpc_table_header);
		sunrpc_table_header = NULL;
	}
}

static int proc_do_xprt(ctl_table *table, int write, struct file *file,
			void __user *buffer, size_t *lenp, loff_t *ppos)
{
	char tmpbuf[256];
	size_t len;

	if ((*ppos && !write) || !*lenp) {
		*lenp = 0;
		return 0;
	}
	len = svc_print_xprts(tmpbuf, sizeof(tmpbuf));
	return simple_read_from_buffer(buffer, *lenp, ppos, tmpbuf, len);
}

static int
proc_dodebug(ctl_table *table, int write, struct file *file,
				void __user *buffer, size_t *lenp, loff_t *ppos)
{
	char		tmpbuf[20], c, *s;
	char __user *p;
	unsigned int	value;
	size_t		left, len;

	if ((*ppos && !write) || !*lenp) {
		*lenp = 0;
		return 0;
	}

	left = *lenp;

	if (write) {
		if (!access_ok(VERIFY_READ, buffer, left))
			return -EFAULT;
		p = buffer;
		while (left && __get_user(c, p) >= 0 && isspace(c))
			left--, p++;
		if (!left)
			goto done;

		if (left > sizeof(tmpbuf) - 1)
			return -EINVAL;
		if (copy_from_user(tmpbuf, p, left))
			return -EFAULT;
		tmpbuf[left] = '\0';

		for (s = tmpbuf, value = 0; '0' <= *s && *s <= '9'; s++, left--)
			value = 10 * value + (*s - '0');
		if (*s && !isspace(*s))
			return -EINVAL;
		while (left && isspace(*s))
			left--, s++;
		*(unsigned int *) table->data = value;
		/* Display the RPC tasks on writing to rpc_debug */
		if (strcmp(table->procname, "rpc_debug") == 0)
			rpc_show_tasks();
	} else {
		if (!access_ok(VERIFY_WRITE, buffer, left))
			return -EFAULT;
		len = sprintf(tmpbuf, "%d", *(unsigned int *) table->data);
		if (len > left)
			len = left;
		if (__copy_to_user(buffer, tmpbuf, len))
			return -EFAULT;
		if ((left -= len) > 0) {
			if (put_user('\n', (char __user *)buffer + len))
				return -EFAULT;
			left--;
		}
	}

done:
	*lenp -= left;
	*ppos += *lenp;
	return 0;
}


static ctl_table debug_table[] = {
	{
		.procname	= "rpc_debug",
		.data		= &rpc_debug,
		.maxlen		= sizeof(int),
		.mode		= 0644,
		.proc_handler	= &proc_dodebug
	},
	{
		.procname	= "nfs_debug",
		.data		= &nfs_debug,
		.maxlen		= sizeof(int),
		.mode		= 0644,
		.proc_handler	= &proc_dodebug
	},
	{
		.procname	= "nfsd_debug",
		.data		= &nfsd_debug,
		.maxlen		= sizeof(int),
		.mode		= 0644,
		.proc_handler	= &proc_dodebug
	},
	{
		.procname	= "nlm_debug",
		.data		= &nlm_debug,
		.maxlen		= sizeof(int),
		.mode		= 0644,
		.proc_handler	= &proc_dodebug
	},
	{
		.procname	= "transports",
		.maxlen		= 256,
		.mode		= 0444,
		.proc_handler	= &proc_do_xprt,
	},
	{ .ctl_name = 0 }
};

static ctl_table sunrpc_table[] = {
	{
		.ctl_name	= CTL_SUNRPC,
		.procname	= "sunrpc",
		.mode		= 0555,
		.child		= debug_table
	},
	{ .ctl_name = 0 }
};

#endif
