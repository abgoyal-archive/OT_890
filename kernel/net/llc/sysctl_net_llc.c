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
 * sysctl_net_llc.c: sysctl interface to LLC net subsystem.
 *
 * Arnaldo Carvalho de Melo <acme@conectiva.com.br>
 */

#include <linux/mm.h>
#include <linux/init.h>
#include <linux/sysctl.h>
#include <net/llc.h>

#ifndef CONFIG_SYSCTL
#error This file should not be compiled without CONFIG_SYSCTL defined
#endif

static struct ctl_table llc2_timeout_table[] = {
	{
		.ctl_name	= NET_LLC2_ACK_TIMEOUT,
		.procname	= "ack",
		.data		= &sysctl_llc2_ack_timeout,
		.maxlen		= sizeof(long),
		.mode		= 0644,
		.proc_handler   = proc_dointvec_jiffies,
		.strategy       = sysctl_jiffies,
	},
	{
		.ctl_name	= NET_LLC2_BUSY_TIMEOUT,
		.procname	= "busy",
		.data		= &sysctl_llc2_busy_timeout,
		.maxlen		= sizeof(long),
		.mode		= 0644,
		.proc_handler   = proc_dointvec_jiffies,
		.strategy       = sysctl_jiffies,
	},
	{
		.ctl_name	= NET_LLC2_P_TIMEOUT,
		.procname	= "p",
		.data		= &sysctl_llc2_p_timeout,
		.maxlen		= sizeof(long),
		.mode		= 0644,
		.proc_handler   = proc_dointvec_jiffies,
		.strategy       = sysctl_jiffies,
	},
	{
		.ctl_name	= NET_LLC2_REJ_TIMEOUT,
		.procname	= "rej",
		.data		= &sysctl_llc2_rej_timeout,
		.maxlen		= sizeof(long),
		.mode		= 0644,
		.proc_handler   = proc_dointvec_jiffies,
		.strategy       = sysctl_jiffies,
	},
	{ 0 },
};

static struct ctl_table llc_station_table[] = {
	{
		.ctl_name	= NET_LLC_STATION_ACK_TIMEOUT,
		.procname	= "ack_timeout",
		.data		= &sysctl_llc_station_ack_timeout,
		.maxlen		= sizeof(long),
		.mode		= 0644,
		.proc_handler   = proc_dointvec_jiffies,
		.strategy       = sysctl_jiffies,
	},
	{ 0 },
};

static struct ctl_table llc2_dir_timeout_table[] = {
	{
		.ctl_name	= NET_LLC2,
		.procname	= "timeout",
		.mode		= 0555,
		.child		= llc2_timeout_table,
	},
	{ 0 },
};

static struct ctl_table llc_table[] = {
	{
		.ctl_name	= NET_LLC2,
		.procname	= "llc2",
		.mode		= 0555,
		.child		= llc2_dir_timeout_table,
	},
	{
		.ctl_name       = NET_LLC_STATION,
		.procname       = "station",
		.mode           = 0555,
		.child          = llc_station_table,
	},
	{ 0 },
};

static struct ctl_path llc_path[] = {
	{ .procname = "net", .ctl_name = CTL_NET, },
	{ .procname = "llc", .ctl_name = NET_LLC, },
	{ }
};

static struct ctl_table_header *llc_table_header;

int __init llc_sysctl_init(void)
{
	llc_table_header = register_sysctl_paths(llc_path, llc_table);

	return llc_table_header ? 0 : -ENOMEM;
}

void llc_sysctl_exit(void)
{
	if (llc_table_header) {
		unregister_sysctl_table(llc_table_header);
		llc_table_header = NULL;
	}
}
