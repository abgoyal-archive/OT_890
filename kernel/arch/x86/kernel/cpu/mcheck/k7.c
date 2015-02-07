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
 * Athlon specific Machine Check Exception Reporting
 * (C) Copyright 2002 Dave Jones <davej@redhat.com>
 */

#include <linux/init.h>
#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/interrupt.h>
#include <linux/smp.h>

#include <asm/processor.h>
#include <asm/system.h>
#include <asm/msr.h>

#include "mce.h"

/* Machine Check Handler For AMD Athlon/Duron */
static void k7_machine_check(struct pt_regs *regs, long error_code)
{
	int recover = 1;
	u32 alow, ahigh, high, low;
	u32 mcgstl, mcgsth;
	int i;

	rdmsr(MSR_IA32_MCG_STATUS, mcgstl, mcgsth);
	if (mcgstl & (1<<0))	/* Recoverable ? */
		recover = 0;

	printk(KERN_EMERG "CPU %d: Machine Check Exception: %08x%08x\n",
		smp_processor_id(), mcgsth, mcgstl);

	for (i = 1; i < nr_mce_banks; i++) {
		rdmsr(MSR_IA32_MC0_STATUS+i*4, low, high);
		if (high&(1<<31)) {
			char misc[20];
			char addr[24];
			misc[0] = addr[0] = '\0';
			if (high & (1<<29))
				recover |= 1;
			if (high & (1<<25))
				recover |= 2;
			high &= ~(1<<31);
			if (high & (1<<27)) {
				rdmsr(MSR_IA32_MC0_MISC+i*4, alow, ahigh);
				snprintf(misc, 20, "[%08x%08x]", ahigh, alow);
			}
			if (high & (1<<26)) {
				rdmsr(MSR_IA32_MC0_ADDR+i*4, alow, ahigh);
				snprintf(addr, 24, " at %08x%08x", ahigh, alow);
			}
			printk(KERN_EMERG "CPU %d: Bank %d: %08x%08x%s%s\n",
				smp_processor_id(), i, high, low, misc, addr);
			/* Clear it */
			wrmsr(MSR_IA32_MC0_STATUS+i*4, 0UL, 0UL);
			/* Serialize */
			wmb();
			add_taint(TAINT_MACHINE_CHECK);
		}
	}

	if (recover&2)
		panic("CPU context corrupt");
	if (recover&1)
		panic("Unable to continue");
	printk(KERN_EMERG "Attempting to continue.\n");
	mcgstl &= ~(1<<2);
	wrmsr(MSR_IA32_MCG_STATUS, mcgstl, mcgsth);
}


/* AMD K7 machine check is Intel like */
void amd_mcheck_init(struct cpuinfo_x86 *c)
{
	u32 l, h;
	int i;

	if (!cpu_has(c, X86_FEATURE_MCE))
		return;

	machine_check_vector = k7_machine_check;
	wmb();

	printk(KERN_INFO "Intel machine check architecture supported.\n");
	rdmsr(MSR_IA32_MCG_CAP, l, h);
	if (l & (1<<8))	/* Control register present ? */
		wrmsr(MSR_IA32_MCG_CTL, 0xffffffff, 0xffffffff);
	nr_mce_banks = l & 0xff;

	/* Clear status for MC index 0 separately, we don't touch CTL,
	 * as some K7 Athlons cause spurious MCEs when its enabled. */
	if (boot_cpu_data.x86 == 6) {
		wrmsr(MSR_IA32_MC0_STATUS, 0x0, 0x0);
		i = 1;
	} else
		i = 0;
	for (; i < nr_mce_banks; i++) {
		wrmsr(MSR_IA32_MC0_CTL+4*i, 0xffffffff, 0xffffffff);
		wrmsr(MSR_IA32_MC0_STATUS+4*i, 0x0, 0x0);
	}

	set_in_cr4(X86_CR4_MCE);
	printk(KERN_INFO "Intel machine check reporting enabled on CPU#%d.\n",
		smp_processor_id());
}
