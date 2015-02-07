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
 *  include/asm-s390/hardirq.h
 *
 *  S390 version
 *    Copyright (C) 1999,2000 IBM Deutschland Entwicklung GmbH, IBM Corporation
 *    Author(s): Martin Schwidefsky (schwidefsky@de.ibm.com),
 *               Denis Joseph Barrow (djbarrow@de.ibm.com,barrow_dj@yahoo.com)
 *
 *  Derived from "include/asm-i386/hardirq.h"
 */

#ifndef __ASM_HARDIRQ_H
#define __ASM_HARDIRQ_H

#include <linux/threads.h>
#include <linux/sched.h>
#include <linux/cache.h>
#include <linux/interrupt.h>
#include <asm/lowcore.h>

/* irq_cpustat_t is unused currently, but could be converted
 * into a percpu variable instead of storing softirq_pending
 * on the lowcore */
typedef struct {
	unsigned int __softirq_pending;
} irq_cpustat_t;

#define local_softirq_pending() (S390_lowcore.softirq_pending)

#define __ARCH_IRQ_STAT
#define __ARCH_HAS_DO_SOFTIRQ

#define HARDIRQ_BITS	8

void clock_comparator_work(void);

static inline unsigned long long local_tick_disable(void)
{
	unsigned long long old;

	old = S390_lowcore.clock_comparator;
	S390_lowcore.clock_comparator = -1ULL;
	return old;
}

static inline void local_tick_enable(unsigned long long comp)
{
	S390_lowcore.clock_comparator = comp;
}

#endif /* __ASM_HARDIRQ_H */
