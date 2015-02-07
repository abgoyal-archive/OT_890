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
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * Copyright (C) 1996 Mike Shaver (shaver@zeroknowledge.com)
 */
#include <linux/mm.h>
#include <linux/sysctl.h>
#include <linux/init.h>
#include <net/ax25.h>
#include <net/rose.h>

static int min_timer[]  = {1 * HZ};
static int max_timer[]  = {300 * HZ};
static int min_idle[]   = {0 * HZ};
static int max_idle[]   = {65535 * HZ};
static int min_route[1],       max_route[] = {1};
static int min_ftimer[] = {60 * HZ};
static int max_ftimer[] = {600 * HZ};
static int min_maxvcs[] = {1}, max_maxvcs[] = {254};
static int min_window[] = {1}, max_window[] = {7};

static struct ctl_table_header *rose_table_header;

static ctl_table rose_table[] = {
	{
		.ctl_name	= NET_ROSE_RESTART_REQUEST_TIMEOUT,
		.procname	= "restart_request_timeout",
		.data		= &sysctl_rose_restart_request_timeout,
		.maxlen		= sizeof(int),
		.mode		= 0644,
		.proc_handler	= proc_dointvec_minmax,
		.strategy	= sysctl_intvec,
		.extra1		= &min_timer,
		.extra2		= &max_timer
	},
	{
		.ctl_name	= NET_ROSE_CALL_REQUEST_TIMEOUT,
		.procname	= "call_request_timeout",
		.data		= &sysctl_rose_call_request_timeout,
		.maxlen		= sizeof(int),
		.mode		= 0644,
		.proc_handler	= proc_dointvec_minmax,
		.strategy	= sysctl_intvec,
		.extra1		= &min_timer,
		.extra2		= &max_timer
	},
	{
		.ctl_name	= NET_ROSE_RESET_REQUEST_TIMEOUT,
		.procname	= "reset_request_timeout",
		.data		= &sysctl_rose_reset_request_timeout,
		.maxlen		= sizeof(int),
		.mode		= 0644,
		.proc_handler	= proc_dointvec_minmax,
		.strategy	= sysctl_intvec,
		.extra1		= &min_timer,
		.extra2		= &max_timer
	},
	{
		.ctl_name	= NET_ROSE_CLEAR_REQUEST_TIMEOUT,
		.procname	= "clear_request_timeout",
		.data		= &sysctl_rose_clear_request_timeout,
		.maxlen		= sizeof(int),
		.mode		= 0644,
		.proc_handler	= proc_dointvec_minmax,
		.strategy	= sysctl_intvec,
		.extra1		= &min_timer,
		.extra2		= &max_timer
	},
	{
		.ctl_name	= NET_ROSE_NO_ACTIVITY_TIMEOUT,
		.procname	= "no_activity_timeout",
		.data		= &sysctl_rose_no_activity_timeout,
		.maxlen		= sizeof(int),
		.mode		= 0644,
		.proc_handler	= proc_dointvec_minmax,
		.strategy	= sysctl_intvec,
		.extra1		= &min_idle,
		.extra2		= &max_idle
	},
	{
		.ctl_name	= NET_ROSE_ACK_HOLD_BACK_TIMEOUT,
		.procname	= "acknowledge_hold_back_timeout",
		.data		= &sysctl_rose_ack_hold_back_timeout,
		.maxlen		= sizeof(int),
		.mode		= 0644,
		.proc_handler	= proc_dointvec_minmax,
		.strategy	= sysctl_intvec,
		.extra1		= &min_timer,
		.extra2		= &max_timer
	},
	{
		.ctl_name	= NET_ROSE_ROUTING_CONTROL,
		.procname	= "routing_control",
		.data		= &sysctl_rose_routing_control,
		.maxlen		= sizeof(int),
		.mode		= 0644,
		.proc_handler	= proc_dointvec_minmax,
		.strategy	= sysctl_intvec,
		.extra1		= &min_route,
		.extra2		= &max_route
	},
	{
		.ctl_name	= NET_ROSE_LINK_FAIL_TIMEOUT,
		.procname	= "link_fail_timeout",
		.data		= &sysctl_rose_link_fail_timeout,
		.maxlen		= sizeof(int),
		.mode		= 0644,
		.proc_handler	= proc_dointvec_minmax,
		.strategy	= sysctl_intvec,
		.extra1		= &min_ftimer,
		.extra2		= &max_ftimer
	},
	{
		.ctl_name	= NET_ROSE_MAX_VCS,
		.procname	= "maximum_virtual_circuits",
		.data		= &sysctl_rose_maximum_vcs,
		.maxlen		= sizeof(int),
		.mode		= 0644,
		.proc_handler	= proc_dointvec_minmax,
		.strategy	= sysctl_intvec,
		.extra1		= &min_maxvcs,
		.extra2		= &max_maxvcs
	},
	{
		.ctl_name	= NET_ROSE_WINDOW_SIZE,
		.procname	= "window_size",
		.data		= &sysctl_rose_window_size,
		.maxlen		= sizeof(int),
		.mode		= 0644,
		.proc_handler	= proc_dointvec_minmax,
		.strategy	= sysctl_intvec,
		.extra1		= &min_window,
		.extra2		= &max_window
	},
	{ .ctl_name = 0 }
};

static struct ctl_path rose_path[] = {
	{ .procname = "net", .ctl_name = CTL_NET, },
	{ .procname = "rose", .ctl_name = NET_ROSE, },
	{ }
};

void __init rose_register_sysctl(void)
{
	rose_table_header = register_sysctl_paths(rose_path, rose_table);
}

void rose_unregister_sysctl(void)
{
	unregister_sysctl_table(rose_table_header);
}
