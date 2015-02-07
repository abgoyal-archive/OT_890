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
 * kvm_s390.h -  definition for kvm on s390
 *
 * Copyright IBM Corp. 2008
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License (version 2 only)
 * as published by the Free Software Foundation.
 *
 *    Author(s): Carsten Otte <cotte@de.ibm.com>
 *               Christian Borntraeger <borntraeger@de.ibm.com>
 */

#ifndef ARCH_S390_KVM_S390_H
#define ARCH_S390_KVM_S390_H

#include <linux/kvm.h>
#include <linux/kvm_host.h>

typedef int (*intercept_handler_t)(struct kvm_vcpu *vcpu);

int kvm_handle_sie_intercept(struct kvm_vcpu *vcpu);

#define VM_EVENT(d_kvm, d_loglevel, d_string, d_args...)\
do { \
	debug_sprintf_event(d_kvm->arch.dbf, d_loglevel, d_string "\n", \
	  d_args); \
} while (0)

#define VCPU_EVENT(d_vcpu, d_loglevel, d_string, d_args...)\
do { \
	debug_sprintf_event(d_vcpu->kvm->arch.dbf, d_loglevel, \
	  "%02d[%016lx-%016lx]: " d_string "\n", d_vcpu->vcpu_id, \
	  d_vcpu->arch.sie_block->gpsw.mask, d_vcpu->arch.sie_block->gpsw.addr,\
	  d_args); \
} while (0)

static inline int __cpu_is_stopped(struct kvm_vcpu *vcpu)
{
	return atomic_read(&vcpu->arch.sie_block->cpuflags) & CPUSTAT_STOP_INT;
}

int kvm_s390_handle_wait(struct kvm_vcpu *vcpu);
void kvm_s390_idle_wakeup(unsigned long data);
void kvm_s390_deliver_pending_interrupts(struct kvm_vcpu *vcpu);
int kvm_s390_inject_vm(struct kvm *kvm,
		struct kvm_s390_interrupt *s390int);
int kvm_s390_inject_vcpu(struct kvm_vcpu *vcpu,
		struct kvm_s390_interrupt *s390int);
int kvm_s390_inject_program_int(struct kvm_vcpu *vcpu, u16 code);

/* implemented in priv.c */
int kvm_s390_handle_priv(struct kvm_vcpu *vcpu);

/* implemented in sigp.c */
int kvm_s390_handle_sigp(struct kvm_vcpu *vcpu);

/* implemented in kvm-s390.c */
int __kvm_s390_vcpu_store_status(struct kvm_vcpu *vcpu,
				 unsigned long addr);
/* implemented in diag.c */
int kvm_s390_handle_diag(struct kvm_vcpu *vcpu);

#endif
