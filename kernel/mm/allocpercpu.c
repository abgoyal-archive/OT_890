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
 * linux/mm/allocpercpu.c
 *
 * Separated from slab.c August 11, 2006 Christoph Lameter
 */
#include <linux/mm.h>
#include <linux/module.h>

#ifndef cache_line_size
#define cache_line_size()	L1_CACHE_BYTES
#endif

/**
 * percpu_depopulate - depopulate per-cpu data for given cpu
 * @__pdata: per-cpu data to depopulate
 * @cpu: depopulate per-cpu data for this cpu
 *
 * Depopulating per-cpu data for a cpu going offline would be a typical
 * use case. You need to register a cpu hotplug handler for that purpose.
 */
static void percpu_depopulate(void *__pdata, int cpu)
{
	struct percpu_data *pdata = __percpu_disguise(__pdata);

	kfree(pdata->ptrs[cpu]);
	pdata->ptrs[cpu] = NULL;
}

/**
 * percpu_depopulate_mask - depopulate per-cpu data for some cpu's
 * @__pdata: per-cpu data to depopulate
 * @mask: depopulate per-cpu data for cpu's selected through mask bits
 */
static void __percpu_depopulate_mask(void *__pdata, cpumask_t *mask)
{
	int cpu;
	for_each_cpu_mask_nr(cpu, *mask)
		percpu_depopulate(__pdata, cpu);
}

#define percpu_depopulate_mask(__pdata, mask) \
	__percpu_depopulate_mask((__pdata), &(mask))

/**
 * percpu_populate - populate per-cpu data for given cpu
 * @__pdata: per-cpu data to populate further
 * @size: size of per-cpu object
 * @gfp: may sleep or not etc.
 * @cpu: populate per-data for this cpu
 *
 * Populating per-cpu data for a cpu coming online would be a typical
 * use case. You need to register a cpu hotplug handler for that purpose.
 * Per-cpu object is populated with zeroed buffer.
 */
static void *percpu_populate(void *__pdata, size_t size, gfp_t gfp, int cpu)
{
	struct percpu_data *pdata = __percpu_disguise(__pdata);
	int node = cpu_to_node(cpu);

	/*
	 * We should make sure each CPU gets private memory.
	 */
	size = roundup(size, cache_line_size());

	BUG_ON(pdata->ptrs[cpu]);
	if (node_online(node))
		pdata->ptrs[cpu] = kmalloc_node(size, gfp|__GFP_ZERO, node);
	else
		pdata->ptrs[cpu] = kzalloc(size, gfp);
	return pdata->ptrs[cpu];
}

/**
 * percpu_populate_mask - populate per-cpu data for more cpu's
 * @__pdata: per-cpu data to populate further
 * @size: size of per-cpu object
 * @gfp: may sleep or not etc.
 * @mask: populate per-cpu data for cpu's selected through mask bits
 *
 * Per-cpu objects are populated with zeroed buffers.
 */
static int __percpu_populate_mask(void *__pdata, size_t size, gfp_t gfp,
				  cpumask_t *mask)
{
	cpumask_t populated;
	int cpu;

	cpus_clear(populated);
	for_each_cpu_mask_nr(cpu, *mask)
		if (unlikely(!percpu_populate(__pdata, size, gfp, cpu))) {
			__percpu_depopulate_mask(__pdata, &populated);
			return -ENOMEM;
		} else
			cpu_set(cpu, populated);
	return 0;
}

#define percpu_populate_mask(__pdata, size, gfp, mask) \
	__percpu_populate_mask((__pdata), (size), (gfp), &(mask))

/**
 * percpu_alloc_mask - initial setup of per-cpu data
 * @size: size of per-cpu object
 * @gfp: may sleep or not etc.
 * @mask: populate per-data for cpu's selected through mask bits
 *
 * Populating per-cpu data for all online cpu's would be a typical use case,
 * which is simplified by the percpu_alloc() wrapper.
 * Per-cpu objects are populated with zeroed buffers.
 */
void *__percpu_alloc_mask(size_t size, gfp_t gfp, cpumask_t *mask)
{
	/*
	 * We allocate whole cache lines to avoid false sharing
	 */
	size_t sz = roundup(nr_cpu_ids * sizeof(void *), cache_line_size());
	void *pdata = kzalloc(sz, gfp);
	void *__pdata = __percpu_disguise(pdata);

	if (unlikely(!pdata))
		return NULL;
	if (likely(!__percpu_populate_mask(__pdata, size, gfp, mask)))
		return __pdata;
	kfree(pdata);
	return NULL;
}
EXPORT_SYMBOL_GPL(__percpu_alloc_mask);

/**
 * percpu_free - final cleanup of per-cpu data
 * @__pdata: object to clean up
 *
 * We simply clean up any per-cpu object left. No need for the client to
 * track and specify through a bis mask which per-cpu objects are to free.
 */
void percpu_free(void *__pdata)
{
	if (unlikely(!__pdata))
		return;
	__percpu_depopulate_mask(__pdata, &cpu_possible_map);
	kfree(__percpu_disguise(__pdata));
}
EXPORT_SYMBOL_GPL(percpu_free);
