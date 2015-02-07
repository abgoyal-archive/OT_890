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
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 *
 * Copyright (C) 2004, 2005 Ralf Baechle
 * Copyright (C) 2005 MIPS Technologies, Inc.
 */
#include <linux/errno.h>
#include <linux/init.h>
#include <linux/oprofile.h>
#include <linux/smp.h>
#include <asm/cpu-info.h>

#include "op_impl.h"

extern struct op_mips_model op_model_mipsxx_ops __attribute__((weak));
extern struct op_mips_model op_model_rm9000_ops __attribute__((weak));

static struct op_mips_model *model;

static struct op_counter_config ctr[20];

static int op_mips_setup(void)
{
	/* Pre-compute the values to stuff in the hardware registers.  */
	model->reg_setup(ctr);

	/* Configure the registers on all cpus.  */
	on_each_cpu(model->cpu_setup, NULL, 1);

        return 0;
}

static int op_mips_create_files(struct super_block *sb, struct dentry *root)
{
	int i;

	for (i = 0; i < model->num_counters; ++i) {
		struct dentry *dir;
		char buf[4];

		snprintf(buf, sizeof buf, "%d", i);
		dir = oprofilefs_mkdir(sb, root, buf);

		oprofilefs_create_ulong(sb, dir, "enabled", &ctr[i].enabled);
		oprofilefs_create_ulong(sb, dir, "event", &ctr[i].event);
		oprofilefs_create_ulong(sb, dir, "count", &ctr[i].count);
		oprofilefs_create_ulong(sb, dir, "kernel", &ctr[i].kernel);
		oprofilefs_create_ulong(sb, dir, "user", &ctr[i].user);
		oprofilefs_create_ulong(sb, dir, "exl", &ctr[i].exl);
		/* Dummy.  */
		oprofilefs_create_ulong(sb, dir, "unit_mask", &ctr[i].unit_mask);
	}

	return 0;
}

static int op_mips_start(void)
{
	on_each_cpu(model->cpu_start, NULL, 1);

	return 0;
}

static void op_mips_stop(void)
{
	/* Disable performance monitoring for all counters.  */
	on_each_cpu(model->cpu_stop, NULL, 1);
}

int __init oprofile_arch_init(struct oprofile_operations *ops)
{
	struct op_mips_model *lmodel = NULL;
	int res;

	switch (current_cpu_type()) {
	case CPU_5KC:
	case CPU_20KC:
	case CPU_24K:
	case CPU_25KF:
	case CPU_34K:
	case CPU_1004K:
	case CPU_74K:
	case CPU_SB1:
	case CPU_SB1A:
	case CPU_R10000:
	case CPU_R12000:
	case CPU_R14000:
		lmodel = &op_model_mipsxx_ops;
		break;

	case CPU_RM9000:
		lmodel = &op_model_rm9000_ops;
		break;
	};

	if (!lmodel)
		return -ENODEV;

	res = lmodel->init();
	if (res)
		return res;

	model = lmodel;

	ops->create_files	= op_mips_create_files;
	ops->setup		= op_mips_setup;
	//ops->shutdown         = op_mips_shutdown;
	ops->start		= op_mips_start;
	ops->stop		= op_mips_stop;
	ops->cpu_type		= lmodel->cpu_type;

	printk(KERN_INFO "oprofile: using %s performance monitoring.\n",
	       lmodel->cpu_type);

	return 0;
}

void oprofile_arch_exit(void)
{
	if (model)
		model->exit();
}
