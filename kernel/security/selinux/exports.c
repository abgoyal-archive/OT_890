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
 * SELinux services exported to the rest of the kernel.
 *
 * Author: James Morris <jmorris@redhat.com>
 *
 * Copyright (C) 2005 Red Hat, Inc., James Morris <jmorris@redhat.com>
 * Copyright (C) 2006 Trusted Computer Solutions, Inc. <dgoeddel@trustedcs.com>
 * Copyright (C) 2006 IBM Corporation, Timothy R. Chavez <tinytim@us.ibm.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2,
 * as published by the Free Software Foundation.
 */
#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/selinux.h>
#include <linux/fs.h>
#include <linux/ipc.h>
#include <asm/atomic.h>

#include "security.h"
#include "objsec.h"

/* SECMARK reference count */
extern atomic_t selinux_secmark_refcount;

int selinux_string_to_sid(char *str, u32 *sid)
{
	if (selinux_enabled)
		return security_context_to_sid(str, strlen(str), sid);
	else {
		*sid = 0;
		return 0;
	}
}
EXPORT_SYMBOL_GPL(selinux_string_to_sid);

int selinux_secmark_relabel_packet_permission(u32 sid)
{
	if (selinux_enabled) {
		const struct task_security_struct *__tsec;
		u32 tsid;

		__tsec = current_security();
		tsid = __tsec->sid;

		return avc_has_perm(tsid, sid, SECCLASS_PACKET,
				    PACKET__RELABELTO, NULL);
	}
	return 0;
}
EXPORT_SYMBOL_GPL(selinux_secmark_relabel_packet_permission);

void selinux_secmark_refcount_inc(void)
{
	atomic_inc(&selinux_secmark_refcount);
}
EXPORT_SYMBOL_GPL(selinux_secmark_refcount_inc);

void selinux_secmark_refcount_dec(void)
{
	atomic_dec(&selinux_secmark_refcount);
}
EXPORT_SYMBOL_GPL(selinux_secmark_refcount_dec);
