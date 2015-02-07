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
 * diag.c - handling diagnose instructions
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

#include <linux/kvm.h>
#include <linux/kvm_host.h>
#include "kvm-s390.h"

static int __diag_time_slice_end(struct kvm_vcpu *vcpu)
{
	VCPU_EVENT(vcpu, 5, "%s", "diag time slice end");
	vcpu->stat.diagnose_44++;
	vcpu_put(vcpu);
	yield();
	vcpu_load(vcpu);
	return 0;
}

static int __diag_ipl_functions(struct kvm_vcpu *vcpu)
{
	unsigned int reg = vcpu->arch.sie_block->ipa & 0xf;
	unsigned long subcode = vcpu->arch.guest_gprs[reg] & 0xffff;

	VCPU_EVENT(vcpu, 5, "diag ipl functions, subcode %lx", subcode);
	switch (subcode) {
	case 3:
		vcpu->run->s390_reset_flags = KVM_S390_RESET_CLEAR;
		break;
	case 4:
		vcpu->run->s390_reset_flags = 0;
		break;
	default:
		return -ENOTSUPP;
	}

	atomic_clear_mask(CPUSTAT_RUNNING, &vcpu->arch.sie_block->cpuflags);
	vcpu->run->s390_reset_flags |= KVM_S390_RESET_SUBSYSTEM;
	vcpu->run->s390_reset_flags |= KVM_S390_RESET_IPL;
	vcpu->run->s390_reset_flags |= KVM_S390_RESET_CPU_INIT;
	vcpu->run->exit_reason = KVM_EXIT_S390_RESET;
	VCPU_EVENT(vcpu, 3, "requesting userspace resets %llx",
	  vcpu->run->s390_reset_flags);
	return -EREMOTE;
}

int kvm_s390_handle_diag(struct kvm_vcpu *vcpu)
{
	int code = (vcpu->arch.sie_block->ipb & 0xfff0000) >> 16;

	switch (code) {
	case 0x44:
		return __diag_time_slice_end(vcpu);
	case 0x308:
		return __diag_ipl_functions(vcpu);
	default:
		return -ENOTSUPP;
	}
}
