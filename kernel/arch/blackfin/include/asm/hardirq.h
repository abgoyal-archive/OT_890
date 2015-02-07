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

#ifndef __BFIN_HARDIRQ_H
#define __BFIN_HARDIRQ_H

#include <linux/cache.h>
#include <linux/threads.h>
#include <asm/irq.h>

typedef struct {
	unsigned int __softirq_pending;
	unsigned int __syscall_count;
	struct task_struct *__ksoftirqd_task;
} ____cacheline_aligned irq_cpustat_t;

#include <linux/irq_cpustat.h>	/* Standard mappings for irq_cpustat_t above */

/*
 * We put the hardirq and softirq counter into the preemption
 * counter. The bitmask has the following meaning:
 *
 * - bits 0-7 are the preemption count (max preemption depth: 256)
 * - bits 8-15 are the softirq count (max # of softirqs: 256)
 * - bits 16-23 are the hardirq count (max # of hardirqs: 256)
 *
 * - ( bit 26 is the PREEMPT_ACTIVE flag. )
 *
 * PREEMPT_MASK: 0x000000ff
 * HARDIRQ_MASK: 0x0000ff00
 * SOFTIRQ_MASK: 0x00ff0000
 */

#if NR_IRQS > 256
#define HARDIRQ_BITS	9
#else
#define HARDIRQ_BITS	8
#endif

#ifdef NR_IRQS
# if (1 << HARDIRQ_BITS) < NR_IRQS
# error HARDIRQ_BITS is too low!
# endif
#endif

#define __ARCH_IRQ_EXIT_IRQS_DISABLED	1

extern void ack_bad_irq(unsigned int irq);

#endif
