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

/* -*- linux-c -*-
 * sysctl_net_x25.c: sysctl interface to net X.25 subsystem.
 *
 * Begun April 1, 1996, Mike Shaver.
 * Added /proc/sys/net/x25 directory entry (empty =) ). [MS]
 */

#include <linux/sysctl.h>
#include <linux/skbuff.h>
#include <linux/socket.h>
#include <linux/netdevice.h>
#include <linux/init.h>
#include <net/x25.h>

static int min_timer[] = {   1 * HZ };
static int max_timer[] = { 300 * HZ };

static struct ctl_table_header *x25_table_header;

static struct ctl_table x25_table[] = {
	{
		.ctl_name =	NET_X25_RESTART_REQUEST_TIMEOUT,
		.procname =	"restart_request_timeout",
		.data =		&sysctl_x25_restart_request_timeout,
		.maxlen =	sizeof(int),
		.mode =		0644,
		.proc_handler =	proc_dointvec_minmax,
		.strategy =	sysctl_intvec,
		.extra1 =	&min_timer,
		.extra2 =	&max_timer,
	},
	{
		.ctl_name =	NET_X25_CALL_REQUEST_TIMEOUT,
		.procname =	"call_request_timeout",
		.data =		&sysctl_x25_call_request_timeout,
		.maxlen =	sizeof(int),
		.mode =		0644,
		.proc_handler =	proc_dointvec_minmax,
		.strategy =	sysctl_intvec,
		.extra1 =	&min_timer,
		.extra2 =	&max_timer,
	},
	{
		.ctl_name =	NET_X25_RESET_REQUEST_TIMEOUT,
		.procname =	"reset_request_timeout",
		.data =		&sysctl_x25_reset_request_timeout,
		.maxlen =	sizeof(int),
		.mode =		0644,
		.proc_handler =	proc_dointvec_minmax,
		.strategy =	sysctl_intvec,
		.extra1 =	&min_timer,
		.extra2 =	&max_timer,
	},
	{
		.ctl_name =	NET_X25_CLEAR_REQUEST_TIMEOUT,
		.procname =	"clear_request_timeout",
		.data =		&sysctl_x25_clear_request_timeout,
		.maxlen =	sizeof(int),
		.mode =		0644,
		.proc_handler =	proc_dointvec_minmax,
		.strategy =	sysctl_intvec,
		.extra1 =	&min_timer,
		.extra2 =	&max_timer,
	},
	{
		.ctl_name =	NET_X25_ACK_HOLD_BACK_TIMEOUT,
		.procname =	"acknowledgement_hold_back_timeout",
		.data =		&sysctl_x25_ack_holdback_timeout,
		.maxlen =	sizeof(int),
		.mode =		0644,
		.proc_handler =	proc_dointvec_minmax,
		.strategy =	sysctl_intvec,
		.extra1 =	&min_timer,
		.extra2 =	&max_timer,
	},
	{
		.ctl_name =	NET_X25_FORWARD,
		.procname =	"x25_forward",
		.data = 	&sysctl_x25_forward,
		.maxlen = 	sizeof(int),
		.mode = 	0644,
		.proc_handler = proc_dointvec,
	},
	{ 0, },
};

static struct ctl_path x25_path[] = {
	{ .procname = "net", .ctl_name = CTL_NET, },
	{ .procname = "x25", .ctl_name = NET_X25, },
	{ }
};

void __init x25_register_sysctl(void)
{
	x25_table_header = register_sysctl_paths(x25_path, x25_table);
}

void x25_unregister_sysctl(void)
{
	unregister_sysctl_table(x25_table_header);
}
