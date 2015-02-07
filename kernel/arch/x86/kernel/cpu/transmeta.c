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

#include <linux/kernel.h>
#include <linux/mm.h>
#include <linux/init.h>
#include <asm/processor.h>
#include <asm/msr.h>
#include "cpu.h"

static void __cpuinit early_init_transmeta(struct cpuinfo_x86 *c)
{
	u32 xlvl;

	/* Transmeta-defined flags: level 0x80860001 */
	xlvl = cpuid_eax(0x80860000);
	if ((xlvl & 0xffff0000) == 0x80860000) {
		if (xlvl >= 0x80860001)
			c->x86_capability[2] = cpuid_edx(0x80860001);
	}
}

static void __cpuinit init_transmeta(struct cpuinfo_x86 *c)
{
	unsigned int cap_mask, uk, max, dummy;
	unsigned int cms_rev1, cms_rev2;
	unsigned int cpu_rev, cpu_freq = 0, cpu_flags, new_cpu_rev;
	char cpu_info[65];

	early_init_transmeta(c);

	display_cacheinfo(c);

	/* Print CMS and CPU revision */
	max = cpuid_eax(0x80860000);
	cpu_rev = 0;
	if (max >= 0x80860001) {
		cpuid(0x80860001, &dummy, &cpu_rev, &cpu_freq, &cpu_flags);
		if (cpu_rev != 0x02000000) {
			printk(KERN_INFO "CPU: Processor revision %u.%u.%u.%u, %u MHz\n",
				(cpu_rev >> 24) & 0xff,
				(cpu_rev >> 16) & 0xff,
				(cpu_rev >> 8) & 0xff,
				cpu_rev & 0xff,
				cpu_freq);
		}
	}
	if (max >= 0x80860002) {
		cpuid(0x80860002, &new_cpu_rev, &cms_rev1, &cms_rev2, &dummy);
		if (cpu_rev == 0x02000000) {
			printk(KERN_INFO "CPU: Processor revision %08X, %u MHz\n",
				new_cpu_rev, cpu_freq);
		}
		printk(KERN_INFO "CPU: Code Morphing Software revision %u.%u.%u-%u-%u\n",
		       (cms_rev1 >> 24) & 0xff,
		       (cms_rev1 >> 16) & 0xff,
		       (cms_rev1 >> 8) & 0xff,
		       cms_rev1 & 0xff,
		       cms_rev2);
	}
	if (max >= 0x80860006) {
		cpuid(0x80860003,
		      (void *)&cpu_info[0],
		      (void *)&cpu_info[4],
		      (void *)&cpu_info[8],
		      (void *)&cpu_info[12]);
		cpuid(0x80860004,
		      (void *)&cpu_info[16],
		      (void *)&cpu_info[20],
		      (void *)&cpu_info[24],
		      (void *)&cpu_info[28]);
		cpuid(0x80860005,
		      (void *)&cpu_info[32],
		      (void *)&cpu_info[36],
		      (void *)&cpu_info[40],
		      (void *)&cpu_info[44]);
		cpuid(0x80860006,
		      (void *)&cpu_info[48],
		      (void *)&cpu_info[52],
		      (void *)&cpu_info[56],
		      (void *)&cpu_info[60]);
		cpu_info[64] = '\0';
		printk(KERN_INFO "CPU: %s\n", cpu_info);
	}

	/* Unhide possibly hidden capability flags */
	rdmsr(0x80860004, cap_mask, uk);
	wrmsr(0x80860004, ~0, uk);
	c->x86_capability[0] = cpuid_edx(0x00000001);
	wrmsr(0x80860004, cap_mask, uk);

	/* All Transmeta CPUs have a constant TSC */
	set_cpu_cap(c, X86_FEATURE_CONSTANT_TSC);

#ifdef CONFIG_SYSCTL
	/*
	 * randomize_va_space slows us down enormously;
	 * it probably triggers retranslation of x86->native bytecode
	 */
	randomize_va_space = 0;
#endif
}

static struct cpu_dev transmeta_cpu_dev __cpuinitdata = {
	.c_vendor	= "Transmeta",
	.c_ident	= { "GenuineTMx86", "TransmetaCPU" },
	.c_early_init	= early_init_transmeta,
	.c_init		= init_transmeta,
	.c_x86_vendor	= X86_VENDOR_TRANSMETA,
};

cpu_dev_register(transmeta_cpu_dev);
