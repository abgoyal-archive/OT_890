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
 * @file init.c
 *
 * @remark Copyright 2002 OProfile authors
 * @remark Read the file COPYING
 *
 * @author John Levon <levon@movementarian.org>
 */

#include <linux/kernel.h>
#include <linux/oprofile.h>
#include <linux/errno.h>
#include <linux/init.h>
 
#ifdef CONFIG_SPARC64
#include <linux/notifier.h>
#include <linux/rcupdate.h>
#include <linux/kdebug.h>
#include <asm/nmi.h>

static int profile_timer_exceptions_notify(struct notifier_block *self,
					   unsigned long val, void *data)
{
	struct die_args *args = (struct die_args *)data;
	int ret = NOTIFY_DONE;

	switch (val) {
	case DIE_NMI:
		oprofile_add_sample(args->regs, 0);
		ret = NOTIFY_STOP;
		break;
	default:
		break;
	}
	return ret;
}

static struct notifier_block profile_timer_exceptions_nb = {
	.notifier_call	= profile_timer_exceptions_notify,
};

static int timer_start(void)
{
	if (register_die_notifier(&profile_timer_exceptions_nb))
		return 1;
	nmi_adjust_hz(HZ);
	return 0;
}


static void timer_stop(void)
{
	nmi_adjust_hz(1);
	unregister_die_notifier(&profile_timer_exceptions_nb);
	synchronize_sched();  /* Allow already-started NMIs to complete. */
}

static int op_nmi_timer_init(struct oprofile_operations *ops)
{
	if (!nmi_usable)
		return -ENODEV;

	ops->start = timer_start;
	ops->stop = timer_stop;
	ops->cpu_type = "timer";
	printk(KERN_INFO "oprofile: Using perfctr NMI timer interrupt.\n");
	return 0;
}
#endif

int __init oprofile_arch_init(struct oprofile_operations *ops)
{
	int ret = -ENODEV;

#ifdef CONFIG_SPARC64
	ret = op_nmi_timer_init(ops);
	if (!ret)
		return ret;
#endif

	return ret;
}

void oprofile_arch_exit(void)
{
}
