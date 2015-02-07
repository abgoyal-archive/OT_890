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

#ifndef _XEN_EVENTS_H
#define _XEN_EVENTS_H

#include <linux/interrupt.h>

#include <xen/interface/event_channel.h>
#include <asm/xen/hypercall.h>
#include <asm/xen/events.h>

int bind_evtchn_to_irq(unsigned int evtchn);
int bind_evtchn_to_irqhandler(unsigned int evtchn,
			      irq_handler_t handler,
			      unsigned long irqflags, const char *devname,
			      void *dev_id);
int bind_virq_to_irqhandler(unsigned int virq, unsigned int cpu,
			    irq_handler_t handler,
			    unsigned long irqflags, const char *devname,
			    void *dev_id);
int bind_ipi_to_irqhandler(enum ipi_vector ipi,
			   unsigned int cpu,
			   irq_handler_t handler,
			   unsigned long irqflags,
			   const char *devname,
			   void *dev_id);

/*
 * Common unbind function for all event sources. Takes IRQ to unbind from.
 * Automatically closes the underlying event channel (even for bindings
 * made with bind_evtchn_to_irqhandler()).
 */
void unbind_from_irqhandler(unsigned int irq, void *dev_id);

void xen_send_IPI_one(unsigned int cpu, enum ipi_vector vector);
int resend_irq_on_evtchn(unsigned int irq);
void rebind_evtchn_irq(int evtchn, int irq);

static inline void notify_remote_via_evtchn(int port)
{
	struct evtchn_send send = { .port = port };
	(void)HYPERVISOR_event_channel_op(EVTCHNOP_send, &send);
}

extern void notify_remote_via_irq(int irq);

extern void xen_irq_resume(void);

/* Clear an irq's pending state, in preparation for polling on it */
void xen_clear_irq_pending(int irq);
void xen_set_irq_pending(int irq);
bool xen_test_irq_pending(int irq);

/* Poll waiting for an irq to become pending.  In the usual case, the
   irq will be disabled so it won't deliver an interrupt. */
void xen_poll_irq(int irq);

#endif	/* _XEN_EVENTS_H */
