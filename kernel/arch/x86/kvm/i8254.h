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

#ifndef __I8254_H
#define __I8254_H

#include "iodev.h"

struct kvm_kpit_timer {
	struct hrtimer timer;
	int irq;
	s64 period; /* unit: ns */
	s64 scheduled;
	atomic_t pending;
};

struct kvm_kpit_channel_state {
	u32 count; /* can be 65536 */
	u16 latched_count;
	u8 count_latched;
	u8 status_latched;
	u8 status;
	u8 read_state;
	u8 write_state;
	u8 write_latch;
	u8 rw_mode;
	u8 mode;
	u8 bcd; /* not supported */
	u8 gate; /* timer start */
	ktime_t count_load_time;
};

struct kvm_kpit_state {
	struct kvm_kpit_channel_state channels[3];
	struct kvm_kpit_timer pit_timer;
	u32    speaker_data_on;
	struct mutex lock;
	struct kvm_pit *pit;
	spinlock_t inject_lock;
	unsigned long irq_ack;
	struct kvm_irq_ack_notifier irq_ack_notifier;
};

struct kvm_pit {
	unsigned long base_addresss;
	struct kvm_io_device dev;
	struct kvm_io_device speaker_dev;
	struct kvm *kvm;
	struct kvm_kpit_state pit_state;
	int irq_source_id;
};

#define KVM_PIT_BASE_ADDRESS	    0x40
#define KVM_SPEAKER_BASE_ADDRESS    0x61
#define KVM_PIT_MEM_LENGTH	    4
#define KVM_PIT_FREQ		    1193181
#define KVM_MAX_PIT_INTR_INTERVAL   HZ / 100
#define KVM_PIT_CHANNEL_MASK	    0x3

void kvm_inject_pit_timer_irqs(struct kvm_vcpu *vcpu);
void kvm_pit_load_count(struct kvm *kvm, int channel, u32 val);
struct kvm_pit *kvm_create_pit(struct kvm *kvm);
void kvm_free_pit(struct kvm *kvm);
void kvm_pit_reset(struct kvm_pit *pit);

#endif
