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
 *
 * The following software/firmware and/or related documentation ("MediaTek Software")
 * have been modified by MediaTek Inc. All revisions are subject to any receiver's
 * applicable license agreements with MediaTek Inc.
 */

#ifndef __ASM_ARM_CPUTYPE_H
#define __ASM_ARM_CPUTYPE_H

#include <linux/stringify.h>

#define CPUID_ID	0
#define CPUID_CACHETYPE	1
#define CPUID_TCM	2
#define CPUID_TLBTYPE	3

#ifdef CONFIG_CPU_CP15
#define read_cpuid(reg)							\
	({								\
		unsigned int __val;					\
		asm("mrc	p15, 0, %0, c0, c0, " __stringify(reg)	\
		    : "=r" (__val)					\
		    :							\
		    : "cc");						\
		__val;							\
	})
#else
extern unsigned int processor_id;
#define read_cpuid(reg) (processor_id)
#endif

/*
 * The CPU ID never changes at run time, so we might as well tell the
 * compiler that it's constant.  Use this function to read the CPU ID
 * rather than directly reading processor_id or read_cpuid() directly.
 */
static inline unsigned int __attribute_const__ read_cpuid_id(void)
{
	return read_cpuid(CPUID_ID);
}

static inline unsigned int __attribute_const__ read_cpuid_cachetype(void)
{
	return read_cpuid(CPUID_CACHETYPE);
}

#ifdef CONFIG_HAVE_TCM
static inline unsigned int __attribute_const__ read_cpuid_tcmstatus(void)
{
       return read_cpuid(CPUID_TCM);
}
#endif

/*
 * Intel's XScale3 core supports some v6 features (supersections, L2)
 * but advertises itself as v5 as it does not support the v6 ISA.  For
 * this reason, we need a way to explicitly test for this type of CPU.
 */
#ifndef CONFIG_CPU_XSC3
#define cpu_is_xsc3()	0
#else
static inline int cpu_is_xsc3(void)
{
	if ((read_cpuid_id() & 0xffffe000) == 0x69056000)
		return 1;

	return 0;
}
#endif

#if !defined(CONFIG_CPU_XSCALE) && !defined(CONFIG_CPU_XSC3)
#define	cpu_is_xscale()	0
#else
#define	cpu_is_xscale()	1
#endif

#endif
