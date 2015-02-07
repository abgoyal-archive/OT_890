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
 *  Copyright (C) 2007
 *
 *  Author: Eric Biederman <ebiederm@xmision.com>
 *
 *  This program is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License as
 *  published by the Free Software Foundation, version 2 of the
 *  License.
 */

#include <linux/module.h>
#include <linux/uts.h>
#include <linux/utsname.h>
#include <linux/sysctl.h>

static void *get_uts(ctl_table *table, int write)
{
	char *which = table->data;
	struct uts_namespace *uts_ns;

	uts_ns = current->nsproxy->uts_ns;
	which = (which - (char *)&init_uts_ns) + (char *)uts_ns;

	if (!write)
		down_read(&uts_sem);
	else
		down_write(&uts_sem);
	return which;
}

static void put_uts(ctl_table *table, int write, void *which)
{
	if (!write)
		up_read(&uts_sem);
	else
		up_write(&uts_sem);
}

#ifdef CONFIG_PROC_FS
/*
 *	Special case of dostring for the UTS structure. This has locks
 *	to observe. Should this be in kernel/sys.c ????
 */
static int proc_do_uts_string(ctl_table *table, int write, struct file *filp,
		  void __user *buffer, size_t *lenp, loff_t *ppos)
{
	struct ctl_table uts_table;
	int r;
	memcpy(&uts_table, table, sizeof(uts_table));
	uts_table.data = get_uts(table, write);
	r = proc_dostring(&uts_table,write,filp,buffer,lenp, ppos);
	put_uts(table, write, uts_table.data);
	return r;
}
#else
#define proc_do_uts_string NULL
#endif


#ifdef CONFIG_SYSCTL_SYSCALL
/* The generic string strategy routine: */
static int sysctl_uts_string(ctl_table *table,
		  void __user *oldval, size_t __user *oldlenp,
		  void __user *newval, size_t newlen)
{
	struct ctl_table uts_table;
	int r, write;
	write = newval && newlen;
	memcpy(&uts_table, table, sizeof(uts_table));
	uts_table.data = get_uts(table, write);
	r = sysctl_string(&uts_table, oldval, oldlenp, newval, newlen);
	put_uts(table, write, uts_table.data);
	return r;
}
#else
#define sysctl_uts_string NULL
#endif

static struct ctl_table uts_kern_table[] = {
	{
		.ctl_name	= KERN_OSTYPE,
		.procname	= "ostype",
		.data		= init_uts_ns.name.sysname,
		.maxlen		= sizeof(init_uts_ns.name.sysname),
		.mode		= 0444,
		.proc_handler	= proc_do_uts_string,
		.strategy	= sysctl_uts_string,
	},
	{
		.ctl_name	= KERN_OSRELEASE,
		.procname	= "osrelease",
		.data		= init_uts_ns.name.release,
		.maxlen		= sizeof(init_uts_ns.name.release),
		.mode		= 0444,
		.proc_handler	= proc_do_uts_string,
		.strategy	= sysctl_uts_string,
	},
	{
		.ctl_name	= KERN_VERSION,
		.procname	= "version",
		.data		= init_uts_ns.name.version,
		.maxlen		= sizeof(init_uts_ns.name.version),
		.mode		= 0444,
		.proc_handler	= proc_do_uts_string,
		.strategy	= sysctl_uts_string,
	},
	{
		.ctl_name	= KERN_NODENAME,
		.procname	= "hostname",
		.data		= init_uts_ns.name.nodename,
		.maxlen		= sizeof(init_uts_ns.name.nodename),
		.mode		= 0644,
		.proc_handler	= proc_do_uts_string,
		.strategy	= sysctl_uts_string,
	},
	{
		.ctl_name	= KERN_DOMAINNAME,
		.procname	= "domainname",
		.data		= init_uts_ns.name.domainname,
		.maxlen		= sizeof(init_uts_ns.name.domainname),
		.mode		= 0644,
		.proc_handler	= proc_do_uts_string,
		.strategy	= sysctl_uts_string,
	},
	{}
};

static struct ctl_table uts_root_table[] = {
	{
		.ctl_name	= CTL_KERN,
		.procname	= "kernel",
		.mode		= 0555,
		.child		= uts_kern_table,
	},
	{}
};

static int __init utsname_sysctl_init(void)
{
	register_sysctl_table(uts_root_table);
	return 0;
}

__initcall(utsname_sysctl_init);
