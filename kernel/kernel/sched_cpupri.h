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

#ifndef _LINUX_CPUPRI_H
#define _LINUX_CPUPRI_H

#include <linux/sched.h>

#define CPUPRI_NR_PRIORITIES	(MAX_RT_PRIO + 2)
#define CPUPRI_NR_PRI_WORDS	BITS_TO_LONGS(CPUPRI_NR_PRIORITIES)

#define CPUPRI_INVALID -1
#define CPUPRI_IDLE     0
#define CPUPRI_NORMAL   1
/* values 2-101 are RT priorities 0-99 */

struct cpupri_vec {
	spinlock_t lock;
	int        count;
	cpumask_var_t mask;
};

struct cpupri {
	struct cpupri_vec pri_to_cpu[CPUPRI_NR_PRIORITIES];
	long              pri_active[CPUPRI_NR_PRI_WORDS];
	int               cpu_to_pri[NR_CPUS];
};

#ifdef CONFIG_SMP
int  cpupri_find(struct cpupri *cp,
		 struct task_struct *p, cpumask_t *lowest_mask);
void cpupri_set(struct cpupri *cp, int cpu, int pri);
int cpupri_init(struct cpupri *cp, bool bootmem);
void cpupri_cleanup(struct cpupri *cp);
#else
#define cpupri_set(cp, cpu, pri) do { } while (0)
#define cpupri_init() do { } while (0)
#endif

#endif /* _LINUX_CPUPRI_H */
