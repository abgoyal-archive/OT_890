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

/******************************************************************************
*******************************************************************************
**
**  Copyright (C) Sistina Software, Inc.  1997-2003  All rights reserved.
**  Copyright (C) 2004-2007 Red Hat, Inc.  All rights reserved.
**
**  This copyrighted material is made available to anyone wishing to use,
**  modify, copy, or redistribute it subject to the terms and conditions
**  of the GNU General Public License v.2.
**
*******************************************************************************
******************************************************************************/

#include "dlm_internal.h"
#include "lockspace.h"
#include "lock.h"
#include "user.h"
#include "memory.h"
#include "config.h"

static int __init init_dlm(void)
{
	int error;

	error = dlm_memory_init();
	if (error)
		goto out;

	error = dlm_lockspace_init();
	if (error)
		goto out_mem;

	error = dlm_config_init();
	if (error)
		goto out_lockspace;

	error = dlm_register_debugfs();
	if (error)
		goto out_config;

	error = dlm_user_init();
	if (error)
		goto out_debug;

	error = dlm_netlink_init();
	if (error)
		goto out_user;

	error = dlm_plock_init();
	if (error)
		goto out_netlink;

	printk("DLM (built %s %s) installed\n", __DATE__, __TIME__);

	return 0;

 out_netlink:
	dlm_netlink_exit();
 out_user:
	dlm_user_exit();
 out_debug:
	dlm_unregister_debugfs();
 out_config:
	dlm_config_exit();
 out_lockspace:
	dlm_lockspace_exit();
 out_mem:
	dlm_memory_exit();
 out:
	return error;
}

static void __exit exit_dlm(void)
{
	dlm_plock_exit();
	dlm_netlink_exit();
	dlm_user_exit();
	dlm_config_exit();
	dlm_memory_exit();
	dlm_lockspace_exit();
	dlm_unregister_debugfs();
}

module_init(init_dlm);
module_exit(exit_dlm);

MODULE_DESCRIPTION("Distributed Lock Manager");
MODULE_AUTHOR("Red Hat, Inc.");
MODULE_LICENSE("GPL");

EXPORT_SYMBOL_GPL(dlm_new_lockspace);
EXPORT_SYMBOL_GPL(dlm_release_lockspace);
EXPORT_SYMBOL_GPL(dlm_lock);
EXPORT_SYMBOL_GPL(dlm_unlock);

