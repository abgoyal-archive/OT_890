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
 * Copyright (C) 2001  Kyle A. Lucke IBM Corporation
 * Copyright (C) 2001 Mike Corrigan & Dave Engebretsen IBM Corporation
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
#include <linux/init.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/param.h>		/* for HZ */
#include <asm/paca.h>
#include <asm/processor.h>
#include <asm/time.h>
#include <asm/lppaca.h>
#include <asm/firmware.h>
#include <asm/iseries/hv_call_xm.h>

#include "processor_vpd.h"
#include "main_store.h"

static int __init iseries_proc_create(void)
{
	struct proc_dir_entry *e;

	if (!firmware_has_feature(FW_FEATURE_ISERIES))
		return 0;

	e = proc_mkdir("iSeries", 0);
	if (!e)
		return 1;

	return 0;
}
core_initcall(iseries_proc_create);

static unsigned long startTitan = 0;
static unsigned long startTb = 0;

static int proc_titantod_show(struct seq_file *m, void *v)
{
	unsigned long tb0, titan_tod;

	tb0 = get_tb();
	titan_tod = HvCallXm_loadTod();

	seq_printf(m, "Titan\n" );
	seq_printf(m, "  time base =          %016lx\n", tb0);
	seq_printf(m, "  titan tod =          %016lx\n", titan_tod);
	seq_printf(m, "  xProcFreq =          %016x\n",
		   xIoHriProcessorVpd[0].xProcFreq);
	seq_printf(m, "  xTimeBaseFreq =      %016x\n",
		   xIoHriProcessorVpd[0].xTimeBaseFreq);
	seq_printf(m, "  tb_ticks_per_jiffy = %lu\n", tb_ticks_per_jiffy);
	seq_printf(m, "  tb_ticks_per_usec  = %lu\n", tb_ticks_per_usec);

	if (!startTitan) {
		startTitan = titan_tod;
		startTb = tb0;
	} else {
		unsigned long titan_usec = (titan_tod - startTitan) >> 12;
		unsigned long tb_ticks = (tb0 - startTb);
		unsigned long titan_jiffies = titan_usec / (1000000/HZ);
		unsigned long titan_jiff_usec = titan_jiffies * (1000000/HZ);
		unsigned long titan_jiff_rem_usec =
			titan_usec - titan_jiff_usec;
		unsigned long tb_jiffies = tb_ticks / tb_ticks_per_jiffy;
		unsigned long tb_jiff_ticks = tb_jiffies * tb_ticks_per_jiffy;
		unsigned long tb_jiff_rem_ticks = tb_ticks - tb_jiff_ticks;
		unsigned long tb_jiff_rem_usec =
			tb_jiff_rem_ticks / tb_ticks_per_usec;
		unsigned long new_tb_ticks_per_jiffy =
			(tb_ticks * (1000000/HZ))/titan_usec;

		seq_printf(m, "  titan elapsed = %lu uSec\n", titan_usec);
		seq_printf(m, "  tb elapsed    = %lu ticks\n", tb_ticks);
		seq_printf(m, "  titan jiffies = %lu.%04lu \n", titan_jiffies,
			   titan_jiff_rem_usec);
		seq_printf(m, "  tb jiffies    = %lu.%04lu\n", tb_jiffies,
			   tb_jiff_rem_usec);
		seq_printf(m, "  new tb_ticks_per_jiffy = %lu\n",
			   new_tb_ticks_per_jiffy);
	}

	return 0;
}

static int proc_titantod_open(struct inode *inode, struct file *file)
{
	return single_open(file, proc_titantod_show, NULL);
}

static const struct file_operations proc_titantod_operations = {
	.open		= proc_titantod_open,
	.read		= seq_read,
	.llseek		= seq_lseek,
	.release	= single_release,
};

static int __init iseries_proc_init(void)
{
	if (!firmware_has_feature(FW_FEATURE_ISERIES))
		return 0;

	proc_create("iSeries/titanTod", S_IFREG|S_IRUGO, NULL,
		    &proc_titantod_operations);
	return 0;
}
__initcall(iseries_proc_init);
