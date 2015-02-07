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

#ifndef _LINUX_UTSNAME_H
#define _LINUX_UTSNAME_H

#define __OLD_UTS_LEN 8

struct oldold_utsname {
	char sysname[9];
	char nodename[9];
	char release[9];
	char version[9];
	char machine[9];
};

#define __NEW_UTS_LEN 64

struct old_utsname {
	char sysname[65];
	char nodename[65];
	char release[65];
	char version[65];
	char machine[65];
};

struct new_utsname {
	char sysname[65];
	char nodename[65];
	char release[65];
	char version[65];
	char machine[65];
	char domainname[65];
};

#ifdef __KERNEL__

#include <linux/sched.h>
#include <linux/kref.h>
#include <linux/nsproxy.h>
#include <linux/err.h>
#include <asm/atomic.h>

struct uts_namespace {
	struct kref kref;
	struct new_utsname name;
};
extern struct uts_namespace init_uts_ns;

#ifdef CONFIG_UTS_NS
static inline void get_uts_ns(struct uts_namespace *ns)
{
	kref_get(&ns->kref);
}

extern struct uts_namespace *copy_utsname(unsigned long flags,
					struct uts_namespace *ns);
extern void free_uts_ns(struct kref *kref);

static inline void put_uts_ns(struct uts_namespace *ns)
{
	kref_put(&ns->kref, free_uts_ns);
}
#else
static inline void get_uts_ns(struct uts_namespace *ns)
{
}

static inline void put_uts_ns(struct uts_namespace *ns)
{
}

static inline struct uts_namespace *copy_utsname(unsigned long flags,
					struct uts_namespace *ns)
{
	if (flags & CLONE_NEWUTS)
		return ERR_PTR(-EINVAL);

	return ns;
}
#endif

static inline struct new_utsname *utsname(void)
{
	return &current->nsproxy->uts_ns->name;
}

static inline struct new_utsname *init_utsname(void)
{
	return &init_uts_ns.name;
}

extern struct rw_semaphore uts_sem;

#endif /* __KERNEL__ */

#endif /* _LINUX_UTSNAME_H */
