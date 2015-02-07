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
 * Sysctl operations for Coda filesystem
 * Original version: (C) 1996 P. Braam and M. Callahan
 * Rewritten for Linux 2.1. (C) 1997 Carnegie Mellon University
 * 
 * Carnegie Mellon encourages users to contribute improvements to
 * the Coda project. Contact Peter Braam (coda@cs.cmu.edu).
 */

#include <linux/sysctl.h>

#include "coda_int.h"

#ifdef CONFIG_SYSCTL
static struct ctl_table_header *fs_table_header;
#endif

static ctl_table coda_table[] = {
	{
		.ctl_name	= CTL_UNNUMBERED,
		.procname	= "timeout",
		.data		= &coda_timeout,
		.maxlen		= sizeof(int),
		.mode		= 0644,
		.proc_handler	= &proc_dointvec
	},
	{
		.ctl_name	= CTL_UNNUMBERED,
		.procname	= "hard",
		.data		= &coda_hard,
		.maxlen		= sizeof(int),
		.mode		= 0644,
		.proc_handler	= &proc_dointvec
	},
	{
		.ctl_name	= CTL_UNNUMBERED,
		.procname	= "fake_statfs",
		.data		= &coda_fake_statfs,
		.maxlen		= sizeof(int),
		.mode		= 0600,
		.proc_handler	= &proc_dointvec
	},
	{}
};

#ifdef CONFIG_SYSCTL
static ctl_table fs_table[] = {
	{
		.ctl_name	= CTL_UNNUMBERED,
		.procname	= "coda",
		.mode		= 0555,
		.child		= coda_table
	},
	{}
};
#endif

void coda_sysctl_init(void)
{
#ifdef CONFIG_SYSCTL
	if ( !fs_table_header )
		fs_table_header = register_sysctl_table(fs_table);
#endif
}

void coda_sysctl_clean(void)
{
#ifdef CONFIG_SYSCTL
	if ( fs_table_header ) {
		unregister_sysctl_table(fs_table_header);
		fs_table_header = NULL;
	}
#endif
}
