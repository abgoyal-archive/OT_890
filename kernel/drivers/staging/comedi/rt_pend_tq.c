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

#define __NO_VERSION__
/* rt_pend_tq.c */
#include <linux/kernel.h>
#include <linux/errno.h>
#include <linux/sched.h>
#include "comedidev.h"	// for rt spinlocks
#include "rt_pend_tq.h"
#ifdef CONFIG_COMEDI_RTAI
#include <rtai.h>
#endif
#ifdef CONFIG_COMEDI_FUSION
#include <nucleus/asm/hal.h>
#endif
#ifdef CONFIG_COMEDI_RTL
#include <rtl_core.h>
#endif

#ifdef standalone
#include <linux/module.h>
#define rt_pend_tq_init init_module
#define rt_pend_tq_cleanup cleanup_module
#endif

volatile static struct rt_pend_tq rt_pend_tq[RT_PEND_TQ_SIZE];
volatile static struct rt_pend_tq *volatile rt_pend_head = rt_pend_tq,
	*volatile rt_pend_tail = rt_pend_tq;
int rt_pend_tq_irq = 0;
spinlock_t rt_pend_tq_lock = SPIN_LOCK_UNLOCKED;

// WARNING: following code not checked against race conditions yet.
#define INC_CIRCULAR_PTR(ptr,begin,size) do {if(++(ptr)>=(begin)+(size)) (ptr)=(begin); } while(0)
#define DEC_CIRCULAR_PTR(ptr,begin,size) do {if(--(ptr)<(begin)) (ptr)=(begin)+(size)-1; } while(0)

int rt_pend_call(void (*func) (int arg1, void *arg2), int arg1, void *arg2)
{
	unsigned long flags;

	if (func == NULL)
		return -EINVAL;
	if (rt_pend_tq_irq <= 0)
		return -ENODEV;
	comedi_spin_lock_irqsave(&rt_pend_tq_lock, flags);
	INC_CIRCULAR_PTR(rt_pend_head, rt_pend_tq, RT_PEND_TQ_SIZE);
	if (rt_pend_head == rt_pend_tail) {
		// overflow, we just refuse to take this request
		DEC_CIRCULAR_PTR(rt_pend_head, rt_pend_tq, RT_PEND_TQ_SIZE);
		comedi_spin_unlock_irqrestore(&rt_pend_tq_lock, flags);
		return -EAGAIN;
	}
	rt_pend_head->func = func;
	rt_pend_head->arg1 = arg1;
	rt_pend_head->arg2 = arg2;
	comedi_spin_unlock_irqrestore(&rt_pend_tq_lock, flags);
#ifdef CONFIG_COMEDI_RTAI
	rt_pend_linux_srq(rt_pend_tq_irq);
#endif
#ifdef CONFIG_COMEDI_FUSION
	rthal_apc_schedule(rt_pend_tq_irq);
#endif
#ifdef CONFIG_COMEDI_RTL
	rtl_global_pend_irq(rt_pend_tq_irq);

#endif
	return 0;
}

#ifdef CONFIG_COMEDI_RTAI
void rt_pend_irq_handler(void)
#elif defined(CONFIG_COMEDI_FUSION)
void rt_pend_irq_handler(void *cookie)
#elif defined(CONFIG_COMEDI_RTL)
void rt_pend_irq_handler(int irq, void *dev PT_REGS_ARG)
#endif
{
	while (rt_pend_head != rt_pend_tail) {
		INC_CIRCULAR_PTR(rt_pend_tail, rt_pend_tq, RT_PEND_TQ_SIZE);
		rt_pend_tail->func(rt_pend_tail->arg1, rt_pend_tail->arg2);
	}
}

int rt_pend_tq_init(void)
{
	rt_pend_head = rt_pend_tail = rt_pend_tq;
#ifdef CONFIG_COMEDI_RTAI
	rt_pend_tq_irq = rt_request_srq(0, rt_pend_irq_handler, NULL);
#endif
#ifdef CONFIG_COMEDI_FUSION
	rt_pend_tq_irq =
		rthal_apc_alloc("comedi APC", rt_pend_irq_handler, NULL);
#endif
#ifdef CONFIG_COMEDI_RTL
	rt_pend_tq_irq = rtl_get_soft_irq(rt_pend_irq_handler, "rt_pend_irq");
#endif
	if (rt_pend_tq_irq > 0)
		printk("rt_pend_tq: RT bottom half scheduler initialized OK\n");
	else
		printk("rt_pend_tq: rtl_get_soft_irq failed\n");
	return 0;
}

void rt_pend_tq_cleanup(void)
{
	printk("rt_pend_tq: unloading\n");
#ifdef CONFIG_COMEDI_RTAI
	rt_free_srq(rt_pend_tq_irq);
#endif
#ifdef CONFIG_COMEDI_FUSION
	rthal_apc_free(rt_pend_tq_irq);
#endif
#ifdef CONFIG_COMEDI_RTL
	free_irq(rt_pend_tq_irq, NULL);
#endif
}
