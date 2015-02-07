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

#ifndef _LINUX_STOP_MACHINE
#define _LINUX_STOP_MACHINE
/* "Bogolock": stop the entire machine, disable interrupts.  This is a
   very heavy lock, which is equivalent to grabbing every spinlock
   (and more).  So the "read" side to such a lock is anything which
   disables preeempt. */
#include <linux/cpu.h>
#include <linux/cpumask.h>
#include <asm/system.h>

#if defined(CONFIG_STOP_MACHINE) && defined(CONFIG_SMP)

/**
 * stop_machine: freeze the machine on all CPUs and run this function
 * @fn: the function to run
 * @data: the data ptr for the @fn()
 * @cpus: the cpus to run the @fn() on (NULL = any online cpu)
 *
 * Description: This causes a thread to be scheduled on every cpu,
 * each of which disables interrupts.  The result is that noone is
 * holding a spinlock or inside any other preempt-disabled region when
 * @fn() runs.
 *
 * This can be thought of as a very heavy write lock, equivalent to
 * grabbing every spinlock in the kernel. */
int stop_machine(int (*fn)(void *), void *data, const struct cpumask *cpus);

/**
 * __stop_machine: freeze the machine on all CPUs and run this function
 * @fn: the function to run
 * @data: the data ptr for the @fn
 * @cpus: the cpus to run the @fn() on (NULL = any online cpu)
 *
 * Description: This is a special version of the above, which assumes cpus
 * won't come or go while it's being called.  Used by hotplug cpu.
 */
int __stop_machine(int (*fn)(void *), void *data, const struct cpumask *cpus);

/**
 * stop_machine_create: create all stop_machine threads
 *
 * Description: This causes all stop_machine threads to be created before
 * stop_machine actually gets called. This can be used by subsystems that
 * need a non failing stop_machine infrastructure.
 */
int stop_machine_create(void);

/**
 * stop_machine_destroy: destroy all stop_machine threads
 *
 * Description: This causes all stop_machine threads which were created with
 * stop_machine_create to be destroyed again.
 */
void stop_machine_destroy(void);

#else

static inline int stop_machine(int (*fn)(void *), void *data,
			       const struct cpumask *cpus)
{
	int ret;
	local_irq_disable();
	ret = fn(data);
	local_irq_enable();
	return ret;
}

static inline int stop_machine_create(void) { return 0; }
static inline void stop_machine_destroy(void) { }

#endif /* CONFIG_SMP */
#endif /* _LINUX_STOP_MACHINE */
