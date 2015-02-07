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
 * sysctl_net_atalk.c: sysctl interface to net AppleTalk subsystem.
 *
 * Begun April 1, 1996, Mike Shaver.
 * Added /proc/sys/net/atalk directory entry (empty =) ). [MS]
 * Dynamic registration, added aarp entries. (5/30/97 Chris Horn)
 */

#include <linux/sysctl.h>
#include <net/sock.h>
#include <linux/atalk.h>

static struct ctl_table atalk_table[] = {
	{
		.ctl_name	= NET_ATALK_AARP_EXPIRY_TIME,
		.procname	= "aarp-expiry-time",
		.data		= &sysctl_aarp_expiry_time,
		.maxlen		= sizeof(int),
		.mode		= 0644,
		.proc_handler	= proc_dointvec_jiffies,
		.strategy	= sysctl_jiffies,
	},
	{
		.ctl_name	= NET_ATALK_AARP_TICK_TIME,
		.procname	= "aarp-tick-time",
		.data		= &sysctl_aarp_tick_time,
		.maxlen		= sizeof(int),
		.mode		= 0644,
		.proc_handler	= proc_dointvec_jiffies,
		.strategy	= sysctl_jiffies,
	},
	{
		.ctl_name	= NET_ATALK_AARP_RETRANSMIT_LIMIT,
		.procname	= "aarp-retransmit-limit",
		.data		= &sysctl_aarp_retransmit_limit,
		.maxlen		= sizeof(int),
		.mode		= 0644,
		.proc_handler	= proc_dointvec,
	},
	{
		.ctl_name	= NET_ATALK_AARP_RESOLVE_TIME,
		.procname	= "aarp-resolve-time",
		.data		= &sysctl_aarp_resolve_time,
		.maxlen		= sizeof(int),
		.mode		= 0644,
		.proc_handler	= proc_dointvec_jiffies,
		.strategy	= sysctl_jiffies,
	},
	{ 0 },
};

static struct ctl_path atalk_path[] = {
	{ .procname = "net", .ctl_name = CTL_NET, },
	{ .procname = "appletalk", .ctl_name = NET_ATALK, },
	{ }
};

static struct ctl_table_header *atalk_table_header;

void atalk_register_sysctl(void)
{
	atalk_table_header = register_sysctl_paths(atalk_path, atalk_table);
}

void atalk_unregister_sysctl(void)
{
	unregister_sysctl_table(atalk_table_header);
}
