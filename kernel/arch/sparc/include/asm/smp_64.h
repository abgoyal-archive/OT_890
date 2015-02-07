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

/* smp.h: Sparc64 specific SMP stuff.
 *
 * Copyright (C) 1996, 2008 David S. Miller (davem@davemloft.net)
 */

#ifndef _SPARC64_SMP_H
#define _SPARC64_SMP_H

#include <linux/threads.h>
#include <asm/asi.h>
#include <asm/starfire.h>
#include <asm/spitfire.h>

#ifndef __ASSEMBLY__

#include <linux/cpumask.h>
#include <linux/cache.h>

#endif /* !(__ASSEMBLY__) */

#ifdef CONFIG_SMP

#ifndef __ASSEMBLY__

/*
 *	Private routines/data
 */

#include <linux/bitops.h>
#include <asm/atomic.h>
#include <asm/percpu.h>

DECLARE_PER_CPU(cpumask_t, cpu_sibling_map);
extern cpumask_t cpu_core_map[NR_CPUS];
extern int sparc64_multi_core;

extern void arch_send_call_function_single_ipi(int cpu);
extern void arch_send_call_function_ipi(cpumask_t mask);

/*
 *	General functions that each host system must provide.
 */

extern int hard_smp_processor_id(void);
#define raw_smp_processor_id() (current_thread_info()->cpu)

extern void smp_fill_in_sib_core_maps(void);
extern void cpu_play_dead(void);

extern void smp_fetch_global_regs(void);

#ifdef CONFIG_HOTPLUG_CPU
extern int __cpu_disable(void);
extern void __cpu_die(unsigned int cpu);
#endif

#endif /* !(__ASSEMBLY__) */

#else

#define hard_smp_processor_id()		0
#define smp_fill_in_sib_core_maps() do { } while (0)
#define smp_fetch_global_regs() do { } while (0)

#endif /* !(CONFIG_SMP) */

#endif /* !(_SPARC64_SMP_H) */
