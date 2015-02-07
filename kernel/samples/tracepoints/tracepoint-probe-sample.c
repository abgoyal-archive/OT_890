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
 * tracepoint-probe-sample.c
 *
 * sample tracepoint probes.
 */

#include <linux/module.h>
#include <linux/file.h>
#include <linux/dcache.h>
#include "tp-samples-trace.h"

/*
 * Here the caller only guarantees locking for struct file and struct inode.
 * Locking must therefore be done in the probe to use the dentry.
 */
static void probe_subsys_event(struct inode *inode, struct file *file)
{
	path_get(&file->f_path);
	dget(file->f_path.dentry);
	printk(KERN_INFO "Event is encountered with filename %s\n",
		file->f_path.dentry->d_name.name);
	dput(file->f_path.dentry);
	path_put(&file->f_path);
}

static void probe_subsys_eventb(void)
{
	printk(KERN_INFO "Event B is encountered\n");
}

static int __init tp_sample_trace_init(void)
{
	int ret;

	ret = register_trace_subsys_event(probe_subsys_event);
	WARN_ON(ret);
	ret = register_trace_subsys_eventb(probe_subsys_eventb);
	WARN_ON(ret);

	return 0;
}

module_init(tp_sample_trace_init);

static void __exit tp_sample_trace_exit(void)
{
	unregister_trace_subsys_eventb(probe_subsys_eventb);
	unregister_trace_subsys_event(probe_subsys_event);
	tracepoint_synchronize_unregister();
}

module_exit(tp_sample_trace_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Mathieu Desnoyers");
MODULE_DESCRIPTION("Tracepoint Probes Samples");
