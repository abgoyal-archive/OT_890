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
 * linux/fs/nfs/sysctl.c
 *
 * Sysctl interface to NFS parameters
 */
#include <linux/types.h>
#include <linux/linkage.h>
#include <linux/ctype.h>
#include <linux/fs.h>
#include <linux/sysctl.h>
#include <linux/module.h>
#include <linux/nfs4.h>
#include <linux/nfs_idmap.h>
#include <linux/nfs_fs.h>

#include "callback.h"

static const int nfs_set_port_min = 0;
static const int nfs_set_port_max = 65535;
static struct ctl_table_header *nfs_callback_sysctl_table;

static ctl_table nfs_cb_sysctls[] = {
#ifdef CONFIG_NFS_V4
	{
		.ctl_name = CTL_UNNUMBERED,
		.procname = "nfs_callback_tcpport",
		.data = &nfs_callback_set_tcpport,
		.maxlen = sizeof(int),
		.mode = 0644,
		.proc_handler = &proc_dointvec_minmax,
		.extra1 = (int *)&nfs_set_port_min,
		.extra2 = (int *)&nfs_set_port_max,
	},
	{
		.ctl_name = CTL_UNNUMBERED,
		.procname = "idmap_cache_timeout",
		.data = &nfs_idmap_cache_timeout,
		.maxlen = sizeof(int),
		.mode = 0644,
		.proc_handler = &proc_dointvec_jiffies,
		.strategy = &sysctl_jiffies,
	},
#endif
	{
		.ctl_name	= CTL_UNNUMBERED,
		.procname	= "nfs_mountpoint_timeout",
		.data		= &nfs_mountpoint_expiry_timeout,
		.maxlen		= sizeof(nfs_mountpoint_expiry_timeout),
		.mode		= 0644,
		.proc_handler	= &proc_dointvec_jiffies,
		.strategy	= &sysctl_jiffies,
	},
	{
		.ctl_name	= CTL_UNNUMBERED,
		.procname	= "nfs_congestion_kb",
		.data		= &nfs_congestion_kb,
		.maxlen		= sizeof(nfs_congestion_kb),
		.mode		= 0644,
		.proc_handler	= &proc_dointvec,
	},
	{ .ctl_name = 0 }
};

static ctl_table nfs_cb_sysctl_dir[] = {
	{
		.ctl_name = CTL_UNNUMBERED,
		.procname = "nfs",
		.mode = 0555,
		.child = nfs_cb_sysctls,
	},
	{ .ctl_name = 0 }
};

static ctl_table nfs_cb_sysctl_root[] = {
	{
		.ctl_name = CTL_FS,
		.procname = "fs",
		.mode = 0555,
		.child = nfs_cb_sysctl_dir,
	},
	{ .ctl_name = 0 }
};

int nfs_register_sysctl(void)
{
	nfs_callback_sysctl_table = register_sysctl_table(nfs_cb_sysctl_root);
	if (nfs_callback_sysctl_table == NULL)
		return -ENOMEM;
	return 0;
}

void nfs_unregister_sysctl(void)
{
	unregister_sysctl_table(nfs_callback_sysctl_table);
	nfs_callback_sysctl_table = NULL;
}
