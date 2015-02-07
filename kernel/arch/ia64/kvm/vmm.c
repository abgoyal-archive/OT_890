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
 * vmm.c: vmm module interface with kvm module
 *
 * Copyright (c) 2007, Intel Corporation.
 *
 *  Xiantao Zhang (xiantao.zhang@intel.com)
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
 */


#include<linux/kernel.h>
#include<linux/module.h>
#include<asm/fpswa.h>

#include "vcpu.h"

MODULE_AUTHOR("Intel");
MODULE_LICENSE("GPL");

extern char kvm_ia64_ivt;
extern fpswa_interface_t *vmm_fpswa_interface;

long vmm_sanity = 1;

struct kvm_vmm_info vmm_info = {
	.module	     = THIS_MODULE,
	.vmm_entry   = vmm_entry,
	.tramp_entry = vmm_trampoline,
	.vmm_ivt     = (unsigned long)&kvm_ia64_ivt,
};

static int __init  kvm_vmm_init(void)
{

	vmm_fpswa_interface = fpswa_interface;

	/*Register vmm data to kvm side*/
	return kvm_init(&vmm_info, 1024, THIS_MODULE);
}

static void __exit kvm_vmm_exit(void)
{
	kvm_exit();
	return ;
}

void vmm_spin_lock(spinlock_t *lock)
{
	_vmm_raw_spin_lock(lock);
}

void vmm_spin_unlock(spinlock_t *lock)
{
	_vmm_raw_spin_unlock(lock);
}

static void vcpu_debug_exit(struct kvm_vcpu *vcpu)
{
	struct exit_ctl_data *p = &vcpu->arch.exit_data;
	long psr;

	local_irq_save(psr);
	p->exit_reason = EXIT_REASON_DEBUG;
	vmm_transition(vcpu);
	local_irq_restore(psr);
}

asmlinkage int printk(const char *fmt, ...)
{
	struct kvm_vcpu *vcpu = current_vcpu;
	va_list args;
	int r;

	memset(vcpu->arch.log_buf, 0, VMM_LOG_LEN);
	va_start(args, fmt);
	r = vsnprintf(vcpu->arch.log_buf, VMM_LOG_LEN, fmt, args);
	va_end(args);
	vcpu_debug_exit(vcpu);
	return r;
}

module_init(kvm_vmm_init)
module_exit(kvm_vmm_exit)
