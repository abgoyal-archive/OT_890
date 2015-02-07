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

/* -*- mode: c; c-basic-offset: 8 -*- */

/* Copyright (C) 2001
 *
 * Author: J.E.J.Bottomley@HansenPartnership.com
 *
 * This module provides the machine status monitor thread for the
 * voyager architecture.  This allows us to monitor the machine
 * environment (temp, voltage, fan function) and the front panel and
 * internal UPS.  If a fault is detected, this thread takes corrective
 * action (usually just informing init)
 * */

#include <linux/module.h>
#include <linux/mm.h>
#include <linux/kernel_stat.h>
#include <linux/delay.h>
#include <linux/mc146818rtc.h>
#include <linux/init.h>
#include <linux/bootmem.h>
#include <linux/kmod.h>
#include <linux/completion.h>
#include <linux/sched.h>
#include <linux/kthread.h>
#include <asm/desc.h>
#include <asm/voyager.h>
#include <asm/vic.h>
#include <asm/mtrr.h>
#include <asm/msr.h>

struct task_struct *voyager_thread;
static __u8 set_timeout;

static int execute(const char *string)
{
	int ret;

	char *envp[] = {
		"HOME=/",
		"TERM=linux",
		"PATH=/sbin:/usr/sbin:/bin:/usr/bin",
		NULL,
	};
	char *argv[] = {
		"/bin/bash",
		"-c",
		(char *)string,
		NULL,
	};

	if ((ret =
	     call_usermodehelper(argv[0], argv, envp, UMH_WAIT_PROC)) != 0) {
		printk(KERN_ERR "Voyager failed to run \"%s\": %i\n", string,
		       ret);
	}
	return ret;
}

static void check_from_kernel(void)
{
	if (voyager_status.switch_off) {

		/* FIXME: This should be configurable via proc */
		execute("umask 600; echo 0 > /etc/initrunlvl; kill -HUP 1");
	} else if (voyager_status.power_fail) {
		VDEBUG(("Voyager daemon detected AC power failure\n"));

		/* FIXME: This should be configureable via proc */
		execute("umask 600; echo F > /etc/powerstatus; kill -PWR 1");
		set_timeout = 1;
	}
}

static void check_continuing_condition(void)
{
	if (voyager_status.power_fail) {
		__u8 data;
		voyager_cat_psi(VOYAGER_PSI_SUBREAD,
				VOYAGER_PSI_AC_FAIL_REG, &data);
		if ((data & 0x1f) == 0) {
			/* all power restored */
			printk(KERN_NOTICE
			       "VOYAGER AC power restored, cancelling shutdown\n");
			/* FIXME: should be user configureable */
			execute
			    ("umask 600; echo O > /etc/powerstatus; kill -PWR 1");
			set_timeout = 0;
		}
	}
}

static int thread(void *unused)
{
	printk(KERN_NOTICE "Voyager starting monitor thread\n");

	for (;;) {
		set_current_state(TASK_INTERRUPTIBLE);
		schedule_timeout(set_timeout ? HZ : MAX_SCHEDULE_TIMEOUT);

		VDEBUG(("Voyager Daemon awoken\n"));
		if (voyager_status.request_from_kernel == 0) {
			/* probably awoken from timeout */
			check_continuing_condition();
		} else {
			check_from_kernel();
			voyager_status.request_from_kernel = 0;
		}
	}
}

static int __init voyager_thread_start(void)
{
	voyager_thread = kthread_run(thread, NULL, "kvoyagerd");
	if (IS_ERR(voyager_thread)) {
		printk(KERN_ERR
		       "Voyager: Failed to create system monitor thread.\n");
		return PTR_ERR(voyager_thread);
	}
	return 0;
}

static void __exit voyager_thread_stop(void)
{
	kthread_stop(voyager_thread);
}

module_init(voyager_thread_start);
module_exit(voyager_thread_stop);
