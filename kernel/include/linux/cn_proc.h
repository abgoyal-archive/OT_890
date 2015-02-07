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
 * cn_proc.h - process events connector
 *
 * Copyright (C) Matt Helsley, IBM Corp. 2005
 * Based on cn_fork.h by Nguyen Anh Quynh and Guillaume Thouvenin
 * Copyright (C) 2005 Nguyen Anh Quynh <aquynh@gmail.com>
 * Copyright (C) 2005 Guillaume Thouvenin <guillaume.thouvenin@bull.net>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of version 2.1 of the GNU Lesser General Public License
 * as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it would be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 */

#ifndef CN_PROC_H
#define CN_PROC_H

#include <linux/types.h>

/*
 * Userspace sends this enum to register with the kernel that it is listening
 * for events on the connector.
 */
enum proc_cn_mcast_op {
	PROC_CN_MCAST_LISTEN = 1,
	PROC_CN_MCAST_IGNORE = 2
};

/*
 * From the user's point of view, the process
 * ID is the thread group ID and thread ID is the internal
 * kernel "pid". So, fields are assigned as follow:
 *
 *  In user space     -  In  kernel space
 *
 * parent process ID  =  parent->tgid
 * parent thread  ID  =  parent->pid
 * child  process ID  =  child->tgid
 * child  thread  ID  =  child->pid
 */

struct proc_event {
	enum what {
		/* Use successive bits so the enums can be used to record
		 * sets of events as well
		 */
		PROC_EVENT_NONE = 0x00000000,
		PROC_EVENT_FORK = 0x00000001,
		PROC_EVENT_EXEC = 0x00000002,
		PROC_EVENT_UID  = 0x00000004,
		PROC_EVENT_GID  = 0x00000040,
		/* "next" should be 0x00000400 */
		/* "last" is the last process event: exit */
		PROC_EVENT_EXIT = 0x80000000
	} what;
	__u32 cpu;
	__u64 __attribute__((aligned(8))) timestamp_ns;
		/* Number of nano seconds since system boot */
	union { /* must be last field of proc_event struct */
		struct {
			__u32 err;
		} ack;

		struct fork_proc_event {
			pid_t parent_pid;
			pid_t parent_tgid;
			pid_t child_pid;
			pid_t child_tgid;
		} fork;

		struct exec_proc_event {
			pid_t process_pid;
			pid_t process_tgid;
		} exec;

		struct id_proc_event {
			pid_t process_pid;
			pid_t process_tgid;
			union {
				__u32 ruid; /* task uid */
				__u32 rgid; /* task gid */
			} r;
			union {
				__u32 euid;
				__u32 egid;
			} e;
		} id;

		struct exit_proc_event {
			pid_t process_pid;
			pid_t process_tgid;
			__u32 exit_code, exit_signal;
		} exit;
	} event_data;
};

#ifdef __KERNEL__
#ifdef CONFIG_PROC_EVENTS
void proc_fork_connector(struct task_struct *task);
void proc_exec_connector(struct task_struct *task);
void proc_id_connector(struct task_struct *task, int which_id);
void proc_exit_connector(struct task_struct *task);
#else
static inline void proc_fork_connector(struct task_struct *task)
{}

static inline void proc_exec_connector(struct task_struct *task)
{}

static inline void proc_id_connector(struct task_struct *task,
				     int which_id)
{}

static inline void proc_exit_connector(struct task_struct *task)
{}
#endif	/* CONFIG_PROC_EVENTS */
#endif	/* __KERNEL__ */
#endif	/* CN_PROC_H */
