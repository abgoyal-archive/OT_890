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

#ifndef _XEN_MULTICALLS_H
#define _XEN_MULTICALLS_H

#include "xen-ops.h"

/* Multicalls */
struct multicall_space
{
	struct multicall_entry *mc;
	void *args;
};

/* Allocate room for a multicall and its args */
struct multicall_space __xen_mc_entry(size_t args);

DECLARE_PER_CPU(unsigned long, xen_mc_irq_flags);

/* Call to start a batch of multiple __xen_mc_entry()s.  Must be
   paired with xen_mc_issue() */
static inline void xen_mc_batch(void)
{
	unsigned long flags;
	/* need to disable interrupts until this entry is complete */
	local_irq_save(flags);
	__get_cpu_var(xen_mc_irq_flags) = flags;
}

static inline struct multicall_space xen_mc_entry(size_t args)
{
	xen_mc_batch();
	return __xen_mc_entry(args);
}

/* Flush all pending multicalls */
void xen_mc_flush(void);

/* Issue a multicall if we're not in a lazy mode */
static inline void xen_mc_issue(unsigned mode)
{
	if ((paravirt_get_lazy_mode() & mode) == 0)
		xen_mc_flush();

	/* restore flags saved in xen_mc_batch */
	local_irq_restore(x86_read_percpu(xen_mc_irq_flags));
}

/* Set up a callback to be called when the current batch is flushed */
void xen_mc_callback(void (*fn)(void *), void *data);

/*
 * Try to extend the arguments of the previous multicall command.  The
 * previous command's op must match.  If it does, then it attempts to
 * extend the argument space allocated to the multicall entry by
 * arg_size bytes.
 *
 * The returned multicall_space will return with mc pointing to the
 * command on success, or NULL on failure, and args pointing to the
 * newly allocated space.
 */
struct multicall_space xen_mc_extend_args(unsigned long op, size_t arg_size);

#endif /* _XEN_MULTICALLS_H */
