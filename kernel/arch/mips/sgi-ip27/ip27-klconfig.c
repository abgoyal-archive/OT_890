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
 * Copyright (C) 1999, 2000 Ralf Baechle (ralf@gnu.org)
 * Copyright (C) 1999, 2000 Silicon Graphics, Inc.
 */
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/interrupt.h>
#include <linux/kernel_stat.h>
#include <linux/param.h>
#include <linux/timex.h>
#include <linux/mm.h>

#include <asm/sn/klconfig.h>
#include <asm/sn/arch.h>
#include <asm/sn/gda.h>

klinfo_t *find_component(lboard_t *brd, klinfo_t *kli, unsigned char struct_type)
{
	int index, j;

	if (kli == (klinfo_t *)NULL) {
		index = 0;
	} else {
		for (j = 0; j < KLCF_NUM_COMPS(brd); j++)
			if (kli == KLCF_COMP(brd, j))
				break;
		index = j;
		if (index == KLCF_NUM_COMPS(brd)) {
			printk("find_component: Bad pointer: 0x%p\n", kli);
			return (klinfo_t *)NULL;
		}
		index++;		/* next component */
	}

	for (; index < KLCF_NUM_COMPS(brd); index++) {
		kli = KLCF_COMP(brd, index);
		if (KLCF_COMP_TYPE(kli) == struct_type)
			return kli;
	}

	/* Didn't find it. */
	return (klinfo_t *)NULL;
}

klinfo_t *find_first_component(lboard_t *brd, unsigned char struct_type)
{
	return find_component(brd, (klinfo_t *)NULL, struct_type);
}

lboard_t * find_lboard(lboard_t *start, unsigned char brd_type)
{
	/* Search all boards stored on this node. */
	while (start) {
		if (start->brd_type == brd_type)
			return start;
		start = KLCF_NEXT(start);
	}
	/* Didn't find it. */
	return (lboard_t *)NULL;
}

lboard_t * find_lboard_class(lboard_t *start, unsigned char brd_type)
{
	/* Search all boards stored on this node. */
	while (start) {
		if (KLCLASS(start->brd_type) == KLCLASS(brd_type))
			return start;
		start = KLCF_NEXT(start);
	}

	/* Didn't find it. */
	return (lboard_t *)NULL;
}

cnodeid_t get_cpu_cnode(cpuid_t cpu)
{
	return CPUID_TO_COMPACT_NODEID(cpu);
}

klcpu_t * nasid_slice_to_cpuinfo(nasid_t nasid, int slice)
{
	lboard_t *brd;
	klcpu_t *acpu;

	if (!(brd = find_lboard((lboard_t *)KL_CONFIG_INFO(nasid), KLTYPE_IP27)))
		return (klcpu_t *)NULL;

	if (!(acpu = (klcpu_t *)find_first_component(brd, KLSTRUCT_CPU)))
		return (klcpu_t *)NULL;

	do {
		if ((acpu->cpu_info.physid) == slice)
			return acpu;
	} while ((acpu = (klcpu_t *)find_component(brd, (klinfo_t *)acpu,
								KLSTRUCT_CPU)));
	return (klcpu_t *)NULL;
}

klcpu_t * sn_get_cpuinfo(cpuid_t cpu)
{
	nasid_t nasid;
	int slice;
	klcpu_t *acpu;
	gda_t *gdap = GDA;
	cnodeid_t cnode;

	if (!(cpu < MAXCPUS)) {
		printk("sn_get_cpuinfo: illegal cpuid 0x%lx\n", cpu);
		return NULL;
	}

	cnode = get_cpu_cnode(cpu);
	if (cnode == INVALID_CNODEID)
		return NULL;

	if ((nasid = gdap->g_nasidtable[cnode]) == INVALID_NASID)
		return NULL;

	for (slice = 0; slice < CPUS_PER_NODE; slice++) {
		acpu = nasid_slice_to_cpuinfo(nasid, slice);
		if (acpu && acpu->cpu_info.virtid == cpu)
			return acpu;
	}
	return NULL;
}

int get_cpu_slice(cpuid_t cpu)
{
	klcpu_t *acpu;

	if ((acpu = sn_get_cpuinfo(cpu)) == NULL)
		return -1;
	return acpu->cpu_info.physid;
}
