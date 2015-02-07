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

/**
 * @file oprofile_stats.c
 *
 * @remark Copyright 2002 OProfile authors
 * @remark Read the file COPYING
 *
 * @author John Levon
 */

#include <linux/oprofile.h>
#include <linux/smp.h>
#include <linux/cpumask.h>
#include <linux/threads.h>

#include "oprofile_stats.h"
#include "cpu_buffer.h"

struct oprofile_stat_struct oprofile_stats;

void oprofile_reset_stats(void)
{
	struct oprofile_cpu_buffer *cpu_buf;
	int i;

	for_each_possible_cpu(i) {
		cpu_buf = &per_cpu(cpu_buffer, i);
		cpu_buf->sample_received = 0;
		cpu_buf->sample_lost_overflow = 0;
		cpu_buf->backtrace_aborted = 0;
		cpu_buf->sample_invalid_eip = 0;
	}

	atomic_set(&oprofile_stats.sample_lost_no_mm, 0);
	atomic_set(&oprofile_stats.sample_lost_no_mapping, 0);
	atomic_set(&oprofile_stats.event_lost_overflow, 0);
}


void oprofile_create_stats_files(struct super_block *sb, struct dentry *root)
{
	struct oprofile_cpu_buffer *cpu_buf;
	struct dentry *cpudir;
	struct dentry *dir;
	char buf[10];
	int i;

	dir = oprofilefs_mkdir(sb, root, "stats");
	if (!dir)
		return;

	for_each_possible_cpu(i) {
		cpu_buf = &per_cpu(cpu_buffer, i);
		snprintf(buf, 10, "cpu%d", i);
		cpudir = oprofilefs_mkdir(sb, dir, buf);

		/* Strictly speaking access to these ulongs is racy,
		 * but we can't simply lock them, and they are
		 * informational only.
		 */
		oprofilefs_create_ro_ulong(sb, cpudir, "sample_received",
			&cpu_buf->sample_received);
		oprofilefs_create_ro_ulong(sb, cpudir, "sample_lost_overflow",
			&cpu_buf->sample_lost_overflow);
		oprofilefs_create_ro_ulong(sb, cpudir, "backtrace_aborted",
			&cpu_buf->backtrace_aborted);
		oprofilefs_create_ro_ulong(sb, cpudir, "sample_invalid_eip",
			&cpu_buf->sample_invalid_eip);
	}

	oprofilefs_create_ro_atomic(sb, dir, "sample_lost_no_mm",
		&oprofile_stats.sample_lost_no_mm);
	oprofilefs_create_ro_atomic(sb, dir, "sample_lost_no_mapping",
		&oprofile_stats.sample_lost_no_mapping);
	oprofilefs_create_ro_atomic(sb, dir, "event_lost_overflow",
		&oprofile_stats.event_lost_overflow);
	oprofilefs_create_ro_atomic(sb, dir, "bt_lost_no_mapping",
		&oprofile_stats.bt_lost_no_mapping);
}
