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
 * Copyright IBM Corp. 2008
 *
 * Authors: Christian Ehrhardt <ehrhardt@linux.vnet.ibm.com>
 */

#ifndef __POWERPC_KVM_EXITTIMING_H__
#define __POWERPC_KVM_EXITTIMING_H__

#include <linux/kvm_host.h>
#include <asm/kvm_host.h>

#ifdef CONFIG_KVM_EXIT_TIMING
void kvmppc_init_timing_stats(struct kvm_vcpu *vcpu);
void kvmppc_update_timing_stats(struct kvm_vcpu *vcpu);
void kvmppc_create_vcpu_debugfs(struct kvm_vcpu *vcpu, unsigned int id);
void kvmppc_remove_vcpu_debugfs(struct kvm_vcpu *vcpu);

static inline void kvmppc_set_exit_type(struct kvm_vcpu *vcpu, int type)
{
	vcpu->arch.last_exit_type = type;
}

#else
/* if exit timing is not configured there is no need to build the c file */
static inline void kvmppc_init_timing_stats(struct kvm_vcpu *vcpu) {}
static inline void kvmppc_update_timing_stats(struct kvm_vcpu *vcpu) {}
static inline void kvmppc_create_vcpu_debugfs(struct kvm_vcpu *vcpu,
						unsigned int id) {}
static inline void kvmppc_remove_vcpu_debugfs(struct kvm_vcpu *vcpu) {}
static inline void kvmppc_set_exit_type(struct kvm_vcpu *vcpu, int type) {}
#endif /* CONFIG_KVM_EXIT_TIMING */

/* account the exit in kvm_stats */
static inline void kvmppc_account_exit_stat(struct kvm_vcpu *vcpu, int type)
{
	/* type has to be known at build time for optimization */
	BUILD_BUG_ON(__builtin_constant_p(type));
	switch (type) {
	case EXT_INTR_EXITS:
		vcpu->stat.ext_intr_exits++;
		break;
	case DEC_EXITS:
		vcpu->stat.dec_exits++;
		break;
	case EMULATED_INST_EXITS:
		vcpu->stat.emulated_inst_exits++;
		break;
	case DCR_EXITS:
		vcpu->stat.dcr_exits++;
		break;
	case DSI_EXITS:
		vcpu->stat.dsi_exits++;
		break;
	case ISI_EXITS:
		vcpu->stat.isi_exits++;
		break;
	case SYSCALL_EXITS:
		vcpu->stat.syscall_exits++;
		break;
	case DTLB_REAL_MISS_EXITS:
		vcpu->stat.dtlb_real_miss_exits++;
		break;
	case DTLB_VIRT_MISS_EXITS:
		vcpu->stat.dtlb_virt_miss_exits++;
		break;
	case MMIO_EXITS:
		vcpu->stat.mmio_exits++;
		break;
	case ITLB_REAL_MISS_EXITS:
		vcpu->stat.itlb_real_miss_exits++;
		break;
	case ITLB_VIRT_MISS_EXITS:
		vcpu->stat.itlb_virt_miss_exits++;
		break;
	case SIGNAL_EXITS:
		vcpu->stat.signal_exits++;
		break;
	}
}

/* wrapper to set exit time and account for it in kvm_stats */
static inline void kvmppc_account_exit(struct kvm_vcpu *vcpu, int type)
{
	kvmppc_set_exit_type(vcpu, type);
	kvmppc_account_exit_stat(vcpu, type);
}

#endif /* __POWERPC_KVM_EXITTIMING_H__ */
