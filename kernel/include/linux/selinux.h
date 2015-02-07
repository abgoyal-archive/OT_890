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
#ifndef _LINUX_SELINUX_H
#define _LINUX_SELINUX_H

struct selinux_audit_rule;
struct audit_context;
struct kern_ipc_perm;

#ifdef CONFIG_SECURITY_SELINUX

/**
 *     selinux_string_to_sid - map a security context string to a security ID
 *     @str: the security context string to be mapped
 *     @sid: ID value returned via this.
 *
 *     Returns 0 if successful, with the SID stored in sid.  A value
 *     of zero for sid indicates no SID could be determined (but no error
 *     occurred).
 */
int selinux_string_to_sid(char *str, u32 *sid);

/**
 *     selinux_secmark_relabel_packet_permission - secmark permission check
 *     @sid: SECMARK ID value to be applied to network packet
 *
 *     Returns 0 if the current task is allowed to set the SECMARK label of
 *     packets with the supplied security ID.  Note that it is implicit that
 *     the packet is always being relabeled from the default unlabeled value,
 *     and that the access control decision is made in the AVC.
 */
int selinux_secmark_relabel_packet_permission(u32 sid);

/**
 *     selinux_secmark_refcount_inc - increments the secmark use counter
 *
 *     SELinux keeps track of the current SECMARK targets in use so it knows
 *     when to apply SECMARK label access checks to network packets.  This
 *     function incements this reference count to indicate that a new SECMARK
 *     target has been configured.
 */
void selinux_secmark_refcount_inc(void);

/**
 *     selinux_secmark_refcount_dec - decrements the secmark use counter
 *
 *     SELinux keeps track of the current SECMARK targets in use so it knows
 *     when to apply SECMARK label access checks to network packets.  This
 *     function decements this reference count to indicate that one of the
 *     existing SECMARK targets has been removed/flushed.
 */
void selinux_secmark_refcount_dec(void);
#else

static inline int selinux_string_to_sid(const char *str, u32 *sid)
{
       *sid = 0;
       return 0;
}

static inline int selinux_secmark_relabel_packet_permission(u32 sid)
{
	return 0;
}

static inline void selinux_secmark_refcount_inc(void)
{
	return;
}

static inline void selinux_secmark_refcount_dec(void)
{
	return;
}

#endif	/* CONFIG_SECURITY_SELINUX */

#endif /* _LINUX_SELINUX_H */
