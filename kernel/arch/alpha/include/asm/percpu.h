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

#ifndef __ALPHA_PERCPU_H
#define __ALPHA_PERCPU_H
#include <linux/compiler.h>
#include <linux/threads.h>

/*
 * Determine the real variable name from the name visible in the
 * kernel sources.
 */
#define per_cpu_var(var) per_cpu__##var

#ifdef CONFIG_SMP

/*
 * per_cpu_offset() is the offset that has to be added to a
 * percpu variable to get to the instance for a certain processor.
 */
extern unsigned long __per_cpu_offset[NR_CPUS];

#define per_cpu_offset(x) (__per_cpu_offset[x])

#define __my_cpu_offset per_cpu_offset(raw_smp_processor_id())
#ifdef CONFIG_DEBUG_PREEMPT
#define my_cpu_offset per_cpu_offset(smp_processor_id())
#else
#define my_cpu_offset __my_cpu_offset
#endif

#ifndef MODULE
#define SHIFT_PERCPU_PTR(var, offset) RELOC_HIDE(&per_cpu_var(var), (offset))
#define PER_CPU_ATTRIBUTES
#else
/*
 * To calculate addresses of locally defined variables, GCC uses 32-bit
 * displacement from the GP. Which doesn't work for per cpu variables in
 * modules, as an offset to the kernel per cpu area is way above 4G.
 *
 * This forces allocation of a GOT entry for per cpu variable using
 * ldq instruction with a 'literal' relocation.
 */
#define SHIFT_PERCPU_PTR(var, offset) ({		\
	extern int simple_identifier_##var(void);	\
	unsigned long __ptr, tmp_gp;			\
	asm (  "br	%1, 1f		  	      \n\
	1:	ldgp	%1, 0(%1)	    	      \n\
		ldq %0, per_cpu__" #var"(%1)\t!literal"		\
		: "=&r"(__ptr), "=&r"(tmp_gp));		\
	(typeof(&per_cpu_var(var)))(__ptr + (offset)); })

#define PER_CPU_ATTRIBUTES	__used

#endif /* MODULE */

/*
 * A percpu variable may point to a discarded regions. The following are
 * established ways to produce a usable pointer from the percpu variable
 * offset.
 */
#define per_cpu(var, cpu) \
	(*SHIFT_PERCPU_PTR(var, per_cpu_offset(cpu)))
#define __get_cpu_var(var) \
	(*SHIFT_PERCPU_PTR(var, my_cpu_offset))
#define __raw_get_cpu_var(var) \
	(*SHIFT_PERCPU_PTR(var, __my_cpu_offset))

#else /* ! SMP */

#define per_cpu(var, cpu)		(*((void)(cpu), &per_cpu_var(var)))
#define __get_cpu_var(var)		per_cpu_var(var)
#define __raw_get_cpu_var(var)		per_cpu_var(var)

#define PER_CPU_ATTRIBUTES

#endif /* SMP */

#define DECLARE_PER_CPU(type, name) extern __typeof__(type) per_cpu_var(name)

#endif /* __ALPHA_PERCPU_H */
