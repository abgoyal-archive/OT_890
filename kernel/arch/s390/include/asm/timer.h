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
 *  include/asm-s390/timer.h
 *
 *  (C) Copyright IBM Corp. 2003,2006
 *  Virtual CPU timer
 *
 *  Author: Jan Glauber (jang@de.ibm.com)
 */

#ifndef _ASM_S390_TIMER_H
#define _ASM_S390_TIMER_H

#ifdef __KERNEL__

#include <linux/timer.h>

#define VTIMER_MAX_SLICE (0x7ffffffffffff000LL)

struct vtimer_list {
	struct list_head entry;

	int cpu;
	__u64 expires;
	__u64 interval;

	void (*function)(unsigned long);
	unsigned long data;
};

/* the vtimer value will wrap after ca. 71 years */
struct vtimer_queue {
	struct list_head list;
	spinlock_t lock;
	__u64 timer;		/* last programmed timer */
	__u64 elapsed;		/* elapsed time of timer expire values */
	__u64 idle;		/* temp var for idle */
	int do_spt;		/* =1: reprogram cpu timer in idle */
};

extern void init_virt_timer(struct vtimer_list *timer);
extern void add_virt_timer(void *new);
extern void add_virt_timer_periodic(void *new);
extern int mod_virt_timer(struct vtimer_list *timer, __u64 expires);
extern int del_virt_timer(struct vtimer_list *timer);

extern void init_cpu_vtimer(void);
extern void vtime_init(void);

extern void vtime_stop_cpu(void);
extern void vtime_start_leave(void);

#endif /* __KERNEL__ */

#endif /* _ASM_S390_TIMER_H */
