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

#ifndef _LINUX_FAULT_INJECT_H
#define _LINUX_FAULT_INJECT_H

#ifdef CONFIG_FAULT_INJECTION

#include <linux/types.h>
#include <linux/debugfs.h>
#include <asm/atomic.h>

/*
 * For explanation of the elements of this struct, see
 * Documentation/fault-injection/fault-injection.txt
 */
struct fault_attr {
	unsigned long probability;
	unsigned long interval;
	atomic_t times;
	atomic_t space;
	unsigned long verbose;
	u32 task_filter;
	unsigned long stacktrace_depth;
	unsigned long require_start;
	unsigned long require_end;
	unsigned long reject_start;
	unsigned long reject_end;

	unsigned long count;

#ifdef CONFIG_FAULT_INJECTION_DEBUG_FS

	struct {
		struct dentry *dir;

		struct dentry *probability_file;
		struct dentry *interval_file;
		struct dentry *times_file;
		struct dentry *space_file;
		struct dentry *verbose_file;
		struct dentry *task_filter_file;
		struct dentry *stacktrace_depth_file;
		struct dentry *require_start_file;
		struct dentry *require_end_file;
		struct dentry *reject_start_file;
		struct dentry *reject_end_file;
	} dentries;

#endif
};

#define FAULT_ATTR_INITIALIZER {				\
		.interval = 1,					\
		.times = ATOMIC_INIT(1),			\
		.require_end = ULONG_MAX,			\
		.stacktrace_depth = 32,				\
		.verbose = 2,					\
	}

#define DECLARE_FAULT_ATTR(name) struct fault_attr name = FAULT_ATTR_INITIALIZER
int setup_fault_attr(struct fault_attr *attr, char *str);
void should_fail_srandom(unsigned long entropy);
bool should_fail(struct fault_attr *attr, ssize_t size);

#ifdef CONFIG_FAULT_INJECTION_DEBUG_FS

int init_fault_attr_dentries(struct fault_attr *attr, const char *name);
void cleanup_fault_attr_dentries(struct fault_attr *attr);

#else /* CONFIG_FAULT_INJECTION_DEBUG_FS */

static inline int init_fault_attr_dentries(struct fault_attr *attr,
					  const char *name)
{
	return -ENODEV;
}

static inline void cleanup_fault_attr_dentries(struct fault_attr *attr)
{
}

#endif /* CONFIG_FAULT_INJECTION_DEBUG_FS */

#endif /* CONFIG_FAULT_INJECTION */

#ifdef CONFIG_FAILSLAB
extern bool should_failslab(size_t size, gfp_t gfpflags);
#else
static inline bool should_failslab(size_t size, gfp_t gfpflags)
{
	return false;
}
#endif /* CONFIG_FAILSLAB */

#endif /* _LINUX_FAULT_INJECT_H */
