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

#ifndef _ASM_X86_NUMA_64_H
#define _ASM_X86_NUMA_64_H

#include <linux/nodemask.h>
#include <asm/apicdef.h>

struct bootnode {
	u64 start;
	u64 end;
};

extern int compute_hash_shift(struct bootnode *nodes, int numblks,
			      int *nodeids);

#define ZONE_ALIGN (1UL << (MAX_ORDER+PAGE_SHIFT))

extern void numa_init_array(void);
extern int numa_off;

extern void srat_reserve_add_area(int nodeid);
extern int hotadd_percent;

extern s16 apicid_to_node[MAX_LOCAL_APIC];

extern unsigned long numa_free_all_bootmem(void);
extern void setup_node_bootmem(int nodeid, unsigned long start,
			       unsigned long end);

#ifdef CONFIG_NUMA
extern void __init init_cpu_to_node(void);
extern void __cpuinit numa_set_node(int cpu, int node);
extern void __cpuinit numa_clear_node(int cpu);
extern void __cpuinit numa_add_cpu(int cpu);
extern void __cpuinit numa_remove_cpu(int cpu);
#else
static inline void init_cpu_to_node(void)		{ }
static inline void numa_set_node(int cpu, int node)	{ }
static inline void numa_clear_node(int cpu)		{ }
static inline void numa_add_cpu(int cpu, int node)	{ }
static inline void numa_remove_cpu(int cpu)		{ }
#endif

#endif /* _ASM_X86_NUMA_64_H */
