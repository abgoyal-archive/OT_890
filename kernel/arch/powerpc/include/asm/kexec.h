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

#ifndef _ASM_POWERPC_KEXEC_H
#define _ASM_POWERPC_KEXEC_H
#ifdef __KERNEL__

/*
 * Maximum page that is mapped directly into kernel memory.
 * XXX: Since we copy virt we can use any page we allocate
 */
#define KEXEC_SOURCE_MEMORY_LIMIT (-1UL)

/*
 * Maximum address we can reach in physical address mode.
 * XXX: I want to allow initrd in highmem. Otherwise set to rmo on LPAR.
 */
#define KEXEC_DESTINATION_MEMORY_LIMIT (-1UL)

/* Maximum address we can use for the control code buffer */
#ifdef __powerpc64__
#define KEXEC_CONTROL_MEMORY_LIMIT (-1UL)
#else
/* TASK_SIZE, probably left over from use_mm ?? */
#define KEXEC_CONTROL_MEMORY_LIMIT TASK_SIZE
#endif

#define KEXEC_CONTROL_PAGE_SIZE 4096

/* The native architecture */
#ifdef __powerpc64__
#define KEXEC_ARCH KEXEC_ARCH_PPC64
#else
#define KEXEC_ARCH KEXEC_ARCH_PPC
#endif

#ifndef __ASSEMBLY__
#include <linux/cpumask.h>
#include <asm/reg.h>

typedef void (*crash_shutdown_t)(void);

#ifdef CONFIG_KEXEC

/*
 * This function is responsible for capturing register states if coming
 * via panic or invoking dump using sysrq-trigger.
 */
static inline void crash_setup_regs(struct pt_regs *newregs,
					struct pt_regs *oldregs)
{
	if (oldregs)
		memcpy(newregs, oldregs, sizeof(*newregs));
	else
		ppc_save_regs(newregs);
}

extern void kexec_smp_wait(void);	/* get and clear naca physid, wait for
					  master to copy new code to 0 */
extern int crashing_cpu;
extern void crash_send_ipi(void (*crash_ipi_callback)(struct pt_regs *));
extern cpumask_t cpus_in_sr;
static inline int kexec_sr_activated(int cpu)
{
	return cpu_isset(cpu,cpus_in_sr);
}

struct kimage;
struct pt_regs;
extern void default_machine_kexec(struct kimage *image);
extern int default_machine_kexec_prepare(struct kimage *image);
extern void default_machine_crash_shutdown(struct pt_regs *regs);
extern int crash_shutdown_register(crash_shutdown_t handler);
extern int crash_shutdown_unregister(crash_shutdown_t handler);

extern void machine_kexec_simple(struct kimage *image);
extern void crash_kexec_secondary(struct pt_regs *regs);
extern int overlaps_crashkernel(unsigned long start, unsigned long size);
extern void reserve_crashkernel(void);

#else /* !CONFIG_KEXEC */
static inline int kexec_sr_activated(int cpu) { return 0; }
static inline void crash_kexec_secondary(struct pt_regs *regs) { }

static inline int overlaps_crashkernel(unsigned long start, unsigned long size)
{
	return 0;
}

static inline void reserve_crashkernel(void) { ; }

static inline int crash_shutdown_register(crash_shutdown_t handler)
{
	return 0;
}

static inline int crash_shutdown_unregister(crash_shutdown_t handler)
{
	return 0;
}

#endif /* CONFIG_KEXEC */
#endif /* ! __ASSEMBLY__ */
#endif /* __KERNEL__ */
#endif /* _ASM_POWERPC_KEXEC_H */
