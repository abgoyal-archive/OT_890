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
 * Fast batching percpu counters.
 */

#include <linux/percpu_counter.h>
#include <linux/notifier.h>
#include <linux/mutex.h>
#include <linux/init.h>
#include <linux/cpu.h>
#include <linux/module.h>

static LIST_HEAD(percpu_counters);
static DEFINE_MUTEX(percpu_counters_lock);

void percpu_counter_set(struct percpu_counter *fbc, s64 amount)
{
	int cpu;

	spin_lock(&fbc->lock);
	for_each_possible_cpu(cpu) {
		s32 *pcount = per_cpu_ptr(fbc->counters, cpu);
		*pcount = 0;
	}
	fbc->count = amount;
	spin_unlock(&fbc->lock);
}
EXPORT_SYMBOL(percpu_counter_set);

void __percpu_counter_add(struct percpu_counter *fbc, s64 amount, s32 batch)
{
	s64 count;
	s32 *pcount;
	int cpu = get_cpu();

	pcount = per_cpu_ptr(fbc->counters, cpu);
	count = *pcount + amount;
	if (count >= batch || count <= -batch) {
		spin_lock(&fbc->lock);
		fbc->count += count;
		*pcount = 0;
		spin_unlock(&fbc->lock);
	} else {
		*pcount = count;
	}
	put_cpu();
}
EXPORT_SYMBOL(__percpu_counter_add);

/*
 * Add up all the per-cpu counts, return the result.  This is a more accurate
 * but much slower version of percpu_counter_read_positive()
 */
s64 __percpu_counter_sum(struct percpu_counter *fbc)
{
	s64 ret;
	int cpu;

	spin_lock(&fbc->lock);
	ret = fbc->count;
	for_each_online_cpu(cpu) {
		s32 *pcount = per_cpu_ptr(fbc->counters, cpu);
		ret += *pcount;
	}
	spin_unlock(&fbc->lock);
	return ret;
}
EXPORT_SYMBOL(__percpu_counter_sum);

int __percpu_counter_init(struct percpu_counter *fbc, s64 amount,
			  struct lock_class_key *key)
{
	spin_lock_init(&fbc->lock);
	lockdep_set_class(&fbc->lock, key);
	fbc->count = amount;
	fbc->counters = alloc_percpu(s32);
	if (!fbc->counters)
		return -ENOMEM;
#ifdef CONFIG_HOTPLUG_CPU
	mutex_lock(&percpu_counters_lock);
	list_add(&fbc->list, &percpu_counters);
	mutex_unlock(&percpu_counters_lock);
#endif
	return 0;
}
EXPORT_SYMBOL(__percpu_counter_init);

void percpu_counter_destroy(struct percpu_counter *fbc)
{
	if (!fbc->counters)
		return;

#ifdef CONFIG_HOTPLUG_CPU
	mutex_lock(&percpu_counters_lock);
	list_del(&fbc->list);
	mutex_unlock(&percpu_counters_lock);
#endif
	free_percpu(fbc->counters);
	fbc->counters = NULL;
}
EXPORT_SYMBOL(percpu_counter_destroy);

int percpu_counter_batch __read_mostly = 32;
EXPORT_SYMBOL(percpu_counter_batch);

static void compute_batch_value(void)
{
	int nr = num_online_cpus();

	percpu_counter_batch = max(32, nr*2);
}

static int __cpuinit percpu_counter_hotcpu_callback(struct notifier_block *nb,
					unsigned long action, void *hcpu)
{
#ifdef CONFIG_HOTPLUG_CPU
	unsigned int cpu;
	struct percpu_counter *fbc;

	compute_batch_value();
	if (action != CPU_DEAD)
		return NOTIFY_OK;

	cpu = (unsigned long)hcpu;
	mutex_lock(&percpu_counters_lock);
	list_for_each_entry(fbc, &percpu_counters, list) {
		s32 *pcount;
		unsigned long flags;

		spin_lock_irqsave(&fbc->lock, flags);
		pcount = per_cpu_ptr(fbc->counters, cpu);
		fbc->count += *pcount;
		*pcount = 0;
		spin_unlock_irqrestore(&fbc->lock, flags);
	}
	mutex_unlock(&percpu_counters_lock);
#endif
	return NOTIFY_OK;
}

static int __init percpu_counter_startup(void)
{
	compute_batch_value();
	hotcpu_notifier(percpu_counter_hotcpu_callback, 0);
	return 0;
}
module_init(percpu_counter_startup);
