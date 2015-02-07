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

/* CPU virtualization extensions handling
 *
 * This should carry the code for handling CPU virtualization extensions
 * that needs to live in the kernel core.
 *
 * Author: Eduardo Habkost <ehabkost@redhat.com>
 *
 * Copyright (C) 2008, Red Hat Inc.
 *
 * Contains code from KVM, Copyright (C) 2006 Qumranet, Inc.
 *
 * This work is licensed under the terms of the GNU GPL, version 2.  See
 * the COPYING file in the top-level directory.
 */
#ifndef _ASM_X86_VIRTEX_H
#define _ASM_X86_VIRTEX_H

#include <asm/processor.h>
#include <asm/system.h>

#include <asm/vmx.h>
#include <asm/svm.h>

/*
 * VMX functions:
 */

static inline int cpu_has_vmx(void)
{
	unsigned long ecx = cpuid_ecx(1);
	return test_bit(5, &ecx); /* CPUID.1:ECX.VMX[bit 5] -> VT */
}


/** Disable VMX on the current CPU
 *
 * vmxoff causes a undefined-opcode exception if vmxon was not run
 * on the CPU previously. Only call this function if you know VMX
 * is enabled.
 */
static inline void cpu_vmxoff(void)
{
	asm volatile (ASM_VMX_VMXOFF : : : "cc");
	write_cr4(read_cr4() & ~X86_CR4_VMXE);
}

static inline int cpu_vmx_enabled(void)
{
	return read_cr4() & X86_CR4_VMXE;
}

/** Disable VMX if it is enabled on the current CPU
 *
 * You shouldn't call this if cpu_has_vmx() returns 0.
 */
static inline void __cpu_emergency_vmxoff(void)
{
	if (cpu_vmx_enabled())
		cpu_vmxoff();
}

/** Disable VMX if it is supported and enabled on the current CPU
 */
static inline void cpu_emergency_vmxoff(void)
{
	if (cpu_has_vmx())
		__cpu_emergency_vmxoff();
}




/*
 * SVM functions:
 */

/** Check if the CPU has SVM support
 *
 * You can use the 'msg' arg to get a message describing the problem,
 * if the function returns zero. Simply pass NULL if you are not interested
 * on the messages; gcc should take care of not generating code for
 * the messages on this case.
 */
static inline int cpu_has_svm(const char **msg)
{
	uint32_t eax, ebx, ecx, edx;

	if (boot_cpu_data.x86_vendor != X86_VENDOR_AMD) {
		if (msg)
			*msg = "not amd";
		return 0;
	}

	cpuid(0x80000000, &eax, &ebx, &ecx, &edx);
	if (eax < SVM_CPUID_FUNC) {
		if (msg)
			*msg = "can't execute cpuid_8000000a";
		return 0;
	}

	cpuid(0x80000001, &eax, &ebx, &ecx, &edx);
	if (!(ecx & (1 << SVM_CPUID_FEATURE_SHIFT))) {
		if (msg)
			*msg = "svm not available";
		return 0;
	}
	return 1;
}


/** Disable SVM on the current CPU
 *
 * You should call this only if cpu_has_svm() returned true.
 */
static inline void cpu_svm_disable(void)
{
	uint64_t efer;

	wrmsrl(MSR_VM_HSAVE_PA, 0);
	rdmsrl(MSR_EFER, efer);
	wrmsrl(MSR_EFER, efer & ~MSR_EFER_SVME_MASK);
}

/** Makes sure SVM is disabled, if it is supported on the CPU
 */
static inline void cpu_emergency_svm_disable(void)
{
	if (cpu_has_svm(NULL))
		cpu_svm_disable();
}

#endif /* _ASM_X86_VIRTEX_H */
