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
 * sysctl.c - Code for sysctl handling in NTFS Linux kernel driver. Part of
 *	      the Linux-NTFS project. Adapted from the old NTFS driver,
 *	      Copyright (C) 1997 Martin von Löwis, Régis Duchesne
 *
 * Copyright (c) 2002-2005 Anton Altaparmakov
 *
 * This program/include file is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as published
 * by the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program/include file is distributed in the hope that it will be
 * useful, but WITHOUT ANY WARRANTY; without even the implied warranty
 * of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program (in the main directory of the Linux-NTFS
 * distribution in the file COPYING); if not, write to the Free Software
 * Foundation,Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#ifdef DEBUG

#include <linux/module.h>

#ifdef CONFIG_SYSCTL

#include <linux/proc_fs.h>
#include <linux/sysctl.h>

#include "sysctl.h"
#include "debug.h"

/* Definition of the ntfs sysctl. */
static ctl_table ntfs_sysctls[] = {
	{
		.ctl_name	= CTL_UNNUMBERED,	/* Binary and text IDs. */
		.procname	= "ntfs-debug",
		.data		= &debug_msgs,		/* Data pointer and size. */
		.maxlen		= sizeof(debug_msgs),
		.mode		= 0644,			/* Mode, proc handler. */
		.proc_handler	= &proc_dointvec
	},
	{}
};

/* Define the parent directory /proc/sys/fs. */
static ctl_table sysctls_root[] = {
	{
		.ctl_name	= CTL_FS,
		.procname	= "fs",
		.mode		= 0555,
		.child		= ntfs_sysctls
	},
	{}
};

/* Storage for the sysctls header. */
static struct ctl_table_header *sysctls_root_table = NULL;

/**
 * ntfs_sysctl - add or remove the debug sysctl
 * @add:	add (1) or remove (0) the sysctl
 *
 * Add or remove the debug sysctl. Return 0 on success or -errno on error.
 */
int ntfs_sysctl(int add)
{
	if (add) {
		BUG_ON(sysctls_root_table);
		sysctls_root_table = register_sysctl_table(sysctls_root);
		if (!sysctls_root_table)
			return -ENOMEM;
	} else {
		BUG_ON(!sysctls_root_table);
		unregister_sysctl_table(sysctls_root_table);
		sysctls_root_table = NULL;
	}
	return 0;
}

#endif /* CONFIG_SYSCTL */
#endif /* DEBUG */
