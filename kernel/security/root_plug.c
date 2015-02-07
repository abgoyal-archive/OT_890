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
 * Root Plug sample LSM module
 *
 * Originally written for a Linux Journal.
 *
 * Copyright (C) 2002 Greg Kroah-Hartman <greg@kroah.com>
 *
 * Prevents any programs running with egid == 0 if a specific USB device
 * is not present in the system.  Yes, it can be gotten around, but is a
 * nice starting point for people to play with, and learn the LSM
 * interface.
 *
 * If you want to turn this into something with a semblance of security,
 * you need to hook the task_* functions also.
 *
 * See http://www.linuxjournal.com/article.php?sid=6279 for more information
 * about this code.
 *
 *	This program is free software; you can redistribute it and/or
 *	modify it under the terms of the GNU General Public License as
 *	published by the Free Software Foundation, version 2 of the
 *	License.
 */

#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/security.h>
#include <linux/usb.h>
#include <linux/moduleparam.h>

/* default is a generic type of usb to serial converter */
static int vendor_id = 0x0557;
static int product_id = 0x2008;

module_param(vendor_id, uint, 0400);
module_param(product_id, uint, 0400);

/* should we print out debug messages */
static int debug = 0;

module_param(debug, bool, 0600);

#define MY_NAME "root_plug"

#define root_dbg(fmt, arg...)					\
	do {							\
		if (debug)					\
			printk(KERN_DEBUG "%s: %s: " fmt ,	\
				MY_NAME , __func__ , 	\
				## arg);			\
	} while (0)

static int rootplug_bprm_check_security (struct linux_binprm *bprm)
{
	struct usb_device *dev;

	root_dbg("file %s, e_uid = %d, e_gid = %d\n",
		 bprm->filename, bprm->cred->euid, bprm->cred->egid);

	if (bprm->cred->egid == 0) {
		dev = usb_find_device(vendor_id, product_id);
		if (!dev) {
			root_dbg("e_gid = 0, and device not found, "
				 "task not allowed to run...\n");
			return -EPERM;
		}
		usb_put_dev(dev);
	}

	return 0;
}

static struct security_operations rootplug_security_ops = {
	/* Use the capability functions for some of the hooks */
	.ptrace_may_access =		cap_ptrace_may_access,
	.ptrace_traceme =		cap_ptrace_traceme,
	.capget =			cap_capget,
	.capset =			cap_capset,
	.capable =			cap_capable,

	.bprm_set_creds =		cap_bprm_set_creds,

	.task_fix_setuid =		cap_task_fix_setuid,
	.task_prctl =			cap_task_prctl,

	.bprm_check_security =		rootplug_bprm_check_security,
};

static int __init rootplug_init (void)
{
	/* register ourselves with the security framework */
	if (register_security (&rootplug_security_ops)) {
		printk (KERN_INFO 
			"Failure registering Root Plug module with the kernel\n");
			return -EINVAL;
	}
	printk (KERN_INFO "Root Plug module initialized, "
		"vendor_id = %4.4x, product id = %4.4x\n", vendor_id, product_id);
	return 0;
}

security_initcall (rootplug_init);
