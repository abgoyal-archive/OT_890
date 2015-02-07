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
 * irq_comm.c: Common API for in kernel interrupt controller
 * Copyright (c) 2007, Intel Corporation.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms and conditions of the GNU General Public License,
 * version 2, as published by the Free Software Foundation.
 *
 * This program is distributed in the hope it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program; if not, write to the Free Software Foundation, Inc., 59 Temple
 * Place - Suite 330, Boston, MA 02111-1307 USA.
 * Authors:
 *   Yaozu (Eddie) Dong <Eddie.dong@intel.com>
 *
 */

#include <linux/kvm_host.h>
#include "irq.h"

#include "ioapic.h"

/* This should be called with the kvm->lock mutex held */
void kvm_set_irq(struct kvm *kvm, int irq_source_id, int irq, int level)
{
	unsigned long *irq_state = (unsigned long *)&kvm->arch.irq_states[irq];

	/* Logical OR for level trig interrupt */
	if (level)
		set_bit(irq_source_id, irq_state);
	else
		clear_bit(irq_source_id, irq_state);

	/* Not possible to detect if the guest uses the PIC or the
	 * IOAPIC.  So set the bit in both. The guest will ignore
	 * writes to the unused one.
	 */
	kvm_ioapic_set_irq(kvm->arch.vioapic, irq, !!(*irq_state));
#ifdef CONFIG_X86
	kvm_pic_set_irq(pic_irqchip(kvm), irq, !!(*irq_state));
#endif
}

void kvm_notify_acked_irq(struct kvm *kvm, unsigned gsi)
{
	struct kvm_irq_ack_notifier *kian;
	struct hlist_node *n;

	hlist_for_each_entry(kian, n, &kvm->arch.irq_ack_notifier_list, link)
		if (kian->gsi == gsi)
			kian->irq_acked(kian);
}

void kvm_register_irq_ack_notifier(struct kvm *kvm,
				   struct kvm_irq_ack_notifier *kian)
{
	hlist_add_head(&kian->link, &kvm->arch.irq_ack_notifier_list);
}

void kvm_unregister_irq_ack_notifier(struct kvm_irq_ack_notifier *kian)
{
	hlist_del_init(&kian->link);
}

/* The caller must hold kvm->lock mutex */
int kvm_request_irq_source_id(struct kvm *kvm)
{
	unsigned long *bitmap = &kvm->arch.irq_sources_bitmap;
	int irq_source_id = find_first_zero_bit(bitmap,
				sizeof(kvm->arch.irq_sources_bitmap));

	if (irq_source_id >= sizeof(kvm->arch.irq_sources_bitmap)) {
		printk(KERN_WARNING "kvm: exhaust allocatable IRQ sources!\n");
		return -EFAULT;
	}

	ASSERT(irq_source_id != KVM_USERSPACE_IRQ_SOURCE_ID);
	set_bit(irq_source_id, bitmap);

	return irq_source_id;
}

void kvm_free_irq_source_id(struct kvm *kvm, int irq_source_id)
{
	int i;

	ASSERT(irq_source_id != KVM_USERSPACE_IRQ_SOURCE_ID);

	if (irq_source_id < 0 ||
	    irq_source_id >= sizeof(kvm->arch.irq_sources_bitmap)) {
		printk(KERN_ERR "kvm: IRQ source ID out of range!\n");
		return;
	}
	for (i = 0; i < KVM_IOAPIC_NUM_PINS; i++)
		clear_bit(irq_source_id, &kvm->arch.irq_states[i]);
	clear_bit(irq_source_id, &kvm->arch.irq_sources_bitmap);
}
