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
 * debugfs ops for process ASIDs
 *
 *  Copyright (C) 2000, 2001  Paolo Alberelli
 *  Copyright (C) 2003 - 2008  Paul Mundt
 *  Copyright (C) 2003, 2004  Richard Curnow
 *
 * Provides a debugfs file that lists out the ASIDs currently associated
 * with the processes.
 *
 * In the SH-5 case, if the DM.PC register is examined through the debug
 * link, this shows ASID + PC. To make use of this, the PID->ASID
 * relationship needs to be known. This is primarily for debugging.
 *
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 */
#include <linux/init.h>
#include <linux/module.h>
#include <linux/debugfs.h>
#include <linux/seq_file.h>
#include <linux/spinlock.h>
#include <asm/processor.h>
#include <asm/mmu_context.h>

static int asids_seq_show(struct seq_file *file, void *iter)
{
	struct task_struct *p;

	read_lock(&tasklist_lock);

	for_each_process(p) {
		int pid = p->pid;

		if (unlikely(!pid))
			continue;

		if (p->mm)
			seq_printf(file, "%5d : %02lx\n", pid,
				   cpu_asid(smp_processor_id(), p->mm));
		else
			seq_printf(file, "%5d : (none)\n", pid);
	}

	read_unlock(&tasklist_lock);

	return 0;
}

static int asids_debugfs_open(struct inode *inode, struct file *file)
{
	return single_open(file, asids_seq_show, inode->i_private);
}

static const struct file_operations asids_debugfs_fops = {
	.owner		= THIS_MODULE,
	.open		= asids_debugfs_open,
	.read		= seq_read,
	.llseek		= seq_lseek,
	.release	= single_release,
};

static int __init asids_debugfs_init(void)
{
	struct dentry *asids_dentry;

	asids_dentry = debugfs_create_file("asids", S_IRUSR, sh_debugfs_root,
					   NULL, &asids_debugfs_fops);
	if (!asids_dentry)
		return -ENOMEM;
	if (IS_ERR(asids_dentry))
		return PTR_ERR(asids_dentry);

	return 0;
}
module_init(asids_debugfs_init);

MODULE_LICENSE("GPL v2");
