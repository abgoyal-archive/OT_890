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
 * RTC subsystem, proc interface
 *
 * Copyright (C) 2005-06 Tower Technologies
 * Author: Alessandro Zummo <a.zummo@towertech.it>
 *
 * based on arch/arm/common/rtctime.c
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
*/

#include <linux/module.h>
#include <linux/rtc.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>

#include "rtc-core.h"


static int rtc_proc_show(struct seq_file *seq, void *offset)
{
	int err;
	struct rtc_device *rtc = seq->private;
	const struct rtc_class_ops *ops = rtc->ops;
	struct rtc_wkalrm alrm;
	struct rtc_time tm;

	err = rtc_read_time(rtc, &tm);
	if (err == 0) {
		seq_printf(seq,
			"rtc_time\t: %02d:%02d:%02d\n"
			"rtc_date\t: %04d-%02d-%02d\n",
			tm.tm_hour, tm.tm_min, tm.tm_sec,
			tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday);
	}

	err = rtc_read_alarm(rtc, &alrm);
	if (err == 0) {
		seq_printf(seq, "alrm_time\t: ");
		if ((unsigned int)alrm.time.tm_hour <= 24)
			seq_printf(seq, "%02d:", alrm.time.tm_hour);
		else
			seq_printf(seq, "**:");
		if ((unsigned int)alrm.time.tm_min <= 59)
			seq_printf(seq, "%02d:", alrm.time.tm_min);
		else
			seq_printf(seq, "**:");
		if ((unsigned int)alrm.time.tm_sec <= 59)
			seq_printf(seq, "%02d\n", alrm.time.tm_sec);
		else
			seq_printf(seq, "**\n");

		seq_printf(seq, "alrm_date\t: ");
		if ((unsigned int)alrm.time.tm_year <= 200)
			seq_printf(seq, "%04d-", alrm.time.tm_year + 1900);
		else
			seq_printf(seq, "****-");
		if ((unsigned int)alrm.time.tm_mon <= 11)
			seq_printf(seq, "%02d-", alrm.time.tm_mon + 1);
		else
			seq_printf(seq, "**-");
		if (alrm.time.tm_mday && (unsigned int)alrm.time.tm_mday <= 31)
			seq_printf(seq, "%02d\n", alrm.time.tm_mday);
		else
			seq_printf(seq, "**\n");
		seq_printf(seq, "alarm_IRQ\t: %s\n",
				alrm.enabled ? "yes" : "no");
		seq_printf(seq, "alrm_pending\t: %s\n",
				alrm.pending ? "yes" : "no");
	}

	seq_printf(seq, "24hr\t\t: yes\n");

	if (ops->proc)
		ops->proc(rtc->dev.parent, seq);

	return 0;
}

static int rtc_proc_open(struct inode *inode, struct file *file)
{
	struct rtc_device *rtc = PDE(inode)->data;

	if (!try_module_get(THIS_MODULE))
		return -ENODEV;

	return single_open(file, rtc_proc_show, rtc);
}

static int rtc_proc_release(struct inode *inode, struct file *file)
{
	int res = single_release(inode, file);
	module_put(THIS_MODULE);
	return res;
}

static const struct file_operations rtc_proc_fops = {
	.open		= rtc_proc_open,
	.read		= seq_read,
	.llseek		= seq_lseek,
	.release	= rtc_proc_release,
};

void rtc_proc_add_device(struct rtc_device *rtc)
{
	if (rtc->id == 0) {
		struct proc_dir_entry *ent;

		ent = proc_create_data("driver/rtc", 0, NULL,
				       &rtc_proc_fops, rtc);
		if (ent)
			ent->owner = rtc->owner;
	}
}

void rtc_proc_del_device(struct rtc_device *rtc)
{
	if (rtc->id == 0)
		remove_proc_entry("driver/rtc", NULL);
}
