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
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License, version 2, as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 *
 * Copyright IBM Corp. 2007
 *
 * Authors: Hollis Blanchard <hollisb@us.ibm.com>
 */

#ifndef __KVM_POWERPC_TLB_H__
#define __KVM_POWERPC_TLB_H__

#include <linux/kvm_host.h>
#include <asm/mmu-44x.h>

extern int kvmppc_44x_tlb_index(struct kvm_vcpu *vcpu, gva_t eaddr,
                                unsigned int pid, unsigned int as);
extern int kvmppc_44x_dtlb_index(struct kvm_vcpu *vcpu, gva_t eaddr);
extern int kvmppc_44x_itlb_index(struct kvm_vcpu *vcpu, gva_t eaddr);

extern int kvmppc_44x_emul_tlbsx(struct kvm_vcpu *vcpu, u8 rt, u8 ra, u8 rb,
                                 u8 rc);
extern int kvmppc_44x_emul_tlbwe(struct kvm_vcpu *vcpu, u8 ra, u8 rs, u8 ws);

/* TLB helper functions */
static inline unsigned int get_tlb_size(const struct kvmppc_44x_tlbe *tlbe)
{
	return (tlbe->word0 >> 4) & 0xf;
}

static inline gva_t get_tlb_eaddr(const struct kvmppc_44x_tlbe *tlbe)
{
	return tlbe->word0 & 0xfffffc00;
}

static inline gva_t get_tlb_bytes(const struct kvmppc_44x_tlbe *tlbe)
{
	unsigned int pgsize = get_tlb_size(tlbe);
	return 1 << 10 << (pgsize << 1);
}

static inline gva_t get_tlb_end(const struct kvmppc_44x_tlbe *tlbe)
{
	return get_tlb_eaddr(tlbe) + get_tlb_bytes(tlbe) - 1;
}

static inline u64 get_tlb_raddr(const struct kvmppc_44x_tlbe *tlbe)
{
	u64 word1 = tlbe->word1;
	return ((word1 & 0xf) << 32) | (word1 & 0xfffffc00);
}

static inline unsigned int get_tlb_tid(const struct kvmppc_44x_tlbe *tlbe)
{
	return tlbe->tid & 0xff;
}

static inline unsigned int get_tlb_ts(const struct kvmppc_44x_tlbe *tlbe)
{
	return (tlbe->word0 >> 8) & 0x1;
}

static inline unsigned int get_tlb_v(const struct kvmppc_44x_tlbe *tlbe)
{
	return (tlbe->word0 >> 9) & 0x1;
}

static inline unsigned int get_mmucr_stid(const struct kvm_vcpu *vcpu)
{
	return vcpu->arch.mmucr & 0xff;
}

static inline unsigned int get_mmucr_sts(const struct kvm_vcpu *vcpu)
{
	return (vcpu->arch.mmucr >> 16) & 0x1;
}

static inline gpa_t tlb_xlate(struct kvmppc_44x_tlbe *tlbe, gva_t eaddr)
{
	unsigned int pgmask = get_tlb_bytes(tlbe) - 1;

	return get_tlb_raddr(tlbe) | (eaddr & pgmask);
}

#endif /* __KVM_POWERPC_TLB_H__ */
