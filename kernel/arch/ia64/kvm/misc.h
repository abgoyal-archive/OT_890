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

#ifndef __KVM_IA64_MISC_H
#define __KVM_IA64_MISC_H

#include <linux/kvm_host.h>
/*
 * misc.h
 * 	Copyright (C) 2007, Intel Corporation.
 *  	Xiantao Zhang  (xiantao.zhang@intel.com)
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
 *
 */

/*
 *Return p2m base address at host side!
 */
static inline uint64_t *kvm_host_get_pmt(struct kvm *kvm)
{
	return (uint64_t *)(kvm->arch.vm_base +
				offsetof(struct kvm_vm_data, kvm_p2m));
}

static inline void kvm_set_pmt_entry(struct kvm *kvm, gfn_t gfn,
		u64 paddr, u64 mem_flags)
{
	uint64_t *pmt_base = kvm_host_get_pmt(kvm);
	unsigned long pte;

	pte = PAGE_ALIGN(paddr) | mem_flags;
	pmt_base[gfn] = pte;
}

/*Function for translating host address to guest address*/

static inline void *to_guest(struct kvm *kvm, void *addr)
{
	return (void *)((unsigned long)(addr) - kvm->arch.vm_base +
			KVM_VM_DATA_BASE);
}

/*Function for translating guest address to host address*/

static inline void *to_host(struct kvm *kvm, void *addr)
{
	return (void *)((unsigned long)addr - KVM_VM_DATA_BASE
			+ kvm->arch.vm_base);
}

/* Get host context of the vcpu */
static inline union context *kvm_get_host_context(struct kvm_vcpu *vcpu)
{
	union context *ctx = &vcpu->arch.host;
	return to_guest(vcpu->kvm, ctx);
}

/* Get guest context of the vcpu */
static inline union context *kvm_get_guest_context(struct kvm_vcpu *vcpu)
{
	union context *ctx = &vcpu->arch.guest;
	return  to_guest(vcpu->kvm, ctx);
}

/* kvm get exit data from gvmm! */
static inline struct exit_ctl_data *kvm_get_exit_data(struct kvm_vcpu *vcpu)
{
	return &vcpu->arch.exit_data;
}

/*kvm get vcpu ioreq for kvm module!*/
static inline struct kvm_mmio_req *kvm_get_vcpu_ioreq(struct kvm_vcpu *vcpu)
{
	struct exit_ctl_data *p_ctl_data;

	if (vcpu) {
		p_ctl_data = kvm_get_exit_data(vcpu);
		if (p_ctl_data->exit_reason == EXIT_REASON_MMIO_INSTRUCTION)
			return &p_ctl_data->u.ioreq;
	}

	return NULL;
}

#endif
