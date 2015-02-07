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

/* cgroupstats.h - exporting per-cgroup statistics
 *
 * Copyright IBM Corporation, 2007
 * Author Balbir Singh <balbir@linux.vnet.ibm.com>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of version 2.1 of the GNU Lesser General Public License
 * as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it would be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 */

#ifndef _LINUX_CGROUPSTATS_H
#define _LINUX_CGROUPSTATS_H

#include <linux/types.h>
#include <linux/taskstats.h>

/*
 * Data shared between user space and kernel space on a per cgroup
 * basis. This data is shared using taskstats.
 *
 * Most of these states are derived by looking at the task->state value
 * For the nr_io_wait state, a flag in the delay accounting structure
 * indicates that the task is waiting on IO
 *
 * Each member is aligned to a 8 byte boundary.
 */
struct cgroupstats {
	__u64	nr_sleeping;		/* Number of tasks sleeping */
	__u64	nr_running;		/* Number of tasks running */
	__u64	nr_stopped;		/* Number of tasks in stopped state */
	__u64	nr_uninterruptible;	/* Number of tasks in uninterruptible */
					/* state */
	__u64	nr_io_wait;		/* Number of tasks waiting on IO */
};

/*
 * Commands sent from userspace
 * Not versioned. New commands should only be inserted at the enum's end
 * prior to __CGROUPSTATS_CMD_MAX
 */

enum {
	CGROUPSTATS_CMD_UNSPEC = __TASKSTATS_CMD_MAX,	/* Reserved */
	CGROUPSTATS_CMD_GET,		/* user->kernel request/get-response */
	CGROUPSTATS_CMD_NEW,		/* kernel->user event */
	__CGROUPSTATS_CMD_MAX,
};

#define CGROUPSTATS_CMD_MAX (__CGROUPSTATS_CMD_MAX - 1)

enum {
	CGROUPSTATS_TYPE_UNSPEC = 0,	/* Reserved */
	CGROUPSTATS_TYPE_CGROUP_STATS,	/* contains name + stats */
	__CGROUPSTATS_TYPE_MAX,
};

#define CGROUPSTATS_TYPE_MAX (__CGROUPSTATS_TYPE_MAX - 1)

enum {
	CGROUPSTATS_CMD_ATTR_UNSPEC = 0,
	CGROUPSTATS_CMD_ATTR_FD,
	__CGROUPSTATS_CMD_ATTR_MAX,
};

#define CGROUPSTATS_CMD_ATTR_MAX (__CGROUPSTATS_CMD_ATTR_MAX - 1)

#endif /* _LINUX_CGROUPSTATS_H */
