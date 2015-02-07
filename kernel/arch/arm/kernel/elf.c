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

#include <linux/module.h>
#include <linux/sched.h>
#include <linux/personality.h>
#include <linux/binfmts.h>
#include <linux/elf.h>

int elf_check_arch(const struct elf32_hdr *x)
{
	unsigned int eflags;

	/* Make sure it's an ARM executable */
	if (x->e_machine != EM_ARM)
		return 0;

	/* Make sure the entry address is reasonable */
	if (x->e_entry & 1) {
		if (!(elf_hwcap & HWCAP_THUMB))
			return 0;
	} else if (x->e_entry & 3)
		return 0;

	eflags = x->e_flags;
	if ((eflags & EF_ARM_EABI_MASK) == EF_ARM_EABI_UNKNOWN) {
		unsigned int flt_fmt;

		/* APCS26 is only allowed if the CPU supports it */
		if ((eflags & EF_ARM_APCS_26) && !(elf_hwcap & HWCAP_26BIT))
			return 0;

		flt_fmt = eflags & (EF_ARM_VFP_FLOAT | EF_ARM_SOFT_FLOAT);

		/* VFP requires the supporting code */
		if (flt_fmt == EF_ARM_VFP_FLOAT && !(elf_hwcap & HWCAP_VFP))
			return 0;
	}
	return 1;
}
EXPORT_SYMBOL(elf_check_arch);

void elf_set_personality(const struct elf32_hdr *x)
{
	unsigned int eflags = x->e_flags;
	unsigned int personality = PER_LINUX_32BIT;

	/*
	 * APCS-26 is only valid for OABI executables
	 */
	if ((eflags & EF_ARM_EABI_MASK) == EF_ARM_EABI_UNKNOWN) {
		if (eflags & EF_ARM_APCS_26)
			personality = PER_LINUX;
	}

	set_personality(personality);

	/*
	 * Since the FPA coprocessor uses CP1 and CP2, and iWMMXt uses CP0
	 * and CP1, we only enable access to the iWMMXt coprocessor if the
	 * binary is EABI or softfloat (and thus, guaranteed not to use
	 * FPA instructions.)
	 */
	if (elf_hwcap & HWCAP_IWMMXT &&
	    eflags & (EF_ARM_EABI_MASK | EF_ARM_SOFT_FLOAT)) {
		set_thread_flag(TIF_USING_IWMMXT);
	} else {
		clear_thread_flag(TIF_USING_IWMMXT);
	}
}
EXPORT_SYMBOL(elf_set_personality);

/*
 * Set READ_IMPLIES_EXEC if:
 *  - the binary requires an executable stack
 *  - we're running on a CPU which doesn't support NX.
 */
int arm_elf_read_implies_exec(const struct elf32_hdr *x, int executable_stack)
{
	if (executable_stack != EXSTACK_DISABLE_X)
		return 1;
	if (cpu_architecture() < CPU_ARCH_ARMv6)
		return 1;
	return 0;
}
EXPORT_SYMBOL(arm_elf_read_implies_exec);
