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
 * Copyright (C) 2006 Mike Kravetz IBM Corporation
 *
 * Hypervisor Call Instrumentation
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
 */

#include <linux/kernel.h>
#include <linux/percpu.h>
#include <linux/debugfs.h>
#include <linux/seq_file.h>
#include <linux/cpumask.h>
#include <asm/hvcall.h>
#include <asm/firmware.h>
#include <asm/cputable.h>

DEFINE_PER_CPU(struct hcall_stats[HCALL_STAT_ARRAY_SIZE], hcall_stats);

/*
 * Routines for displaying the statistics in debugfs
 */
static void *hc_start(struct seq_file *m, loff_t *pos)
{
	if ((int)*pos < (HCALL_STAT_ARRAY_SIZE-1))
		return (void *)(unsigned long)(*pos + 1);

	return NULL;
}

static void *hc_next(struct seq_file *m, void *p, loff_t * pos)
{
	++*pos;

	return hc_start(m, pos);
}

static void hc_stop(struct seq_file *m, void *p)
{
}

static int hc_show(struct seq_file *m, void *p)
{
	unsigned long h_num = (unsigned long)p;
	struct hcall_stats *hs = (struct hcall_stats *)m->private;

	if (hs[h_num].num_calls) {
		if (cpu_has_feature(CPU_FTR_PURR))
			seq_printf(m, "%lu %lu %lu %lu\n", h_num<<2,
				   hs[h_num].num_calls,
				   hs[h_num].tb_total,
				   hs[h_num].purr_total);
		else
			seq_printf(m, "%lu %lu %lu\n", h_num<<2,
				   hs[h_num].num_calls,
				   hs[h_num].tb_total);
	}

	return 0;
}

static struct seq_operations hcall_inst_seq_ops = {
        .start = hc_start,
        .next  = hc_next,
        .stop  = hc_stop,
        .show  = hc_show
};

static int hcall_inst_seq_open(struct inode *inode, struct file *file)
{
	int rc;
	struct seq_file *seq;

	rc = seq_open(file, &hcall_inst_seq_ops);
	seq = file->private_data;
	seq->private = file->f_path.dentry->d_inode->i_private;

	return rc;
}

static const struct file_operations hcall_inst_seq_fops = {
	.open = hcall_inst_seq_open,
	.read = seq_read,
	.llseek = seq_lseek,
	.release = seq_release,
};

#define	HCALL_ROOT_DIR		"hcall_inst"
#define CPU_NAME_BUF_SIZE	32

static int __init hcall_inst_init(void)
{
	struct dentry *hcall_root;
	struct dentry *hcall_file;
	char cpu_name_buf[CPU_NAME_BUF_SIZE];
	int cpu;

	if (!firmware_has_feature(FW_FEATURE_LPAR))
		return 0;

	hcall_root = debugfs_create_dir(HCALL_ROOT_DIR, NULL);
	if (!hcall_root)
		return -ENOMEM;

	for_each_possible_cpu(cpu) {
		snprintf(cpu_name_buf, CPU_NAME_BUF_SIZE, "cpu%d", cpu);
		hcall_file = debugfs_create_file(cpu_name_buf, S_IRUGO,
						 hcall_root,
						 per_cpu(hcall_stats, cpu),
						 &hcall_inst_seq_fops);
		if (!hcall_file)
			return -ENOMEM;
	}

	return 0;
}
__initcall(hcall_inst_init);
