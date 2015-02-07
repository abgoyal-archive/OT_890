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

#ifndef _ASM_X86_NMI_H
#define _ASM_X86_NMI_H

#include <linux/pm.h>
#include <asm/irq.h>
#include <asm/io.h>

#ifdef ARCH_HAS_NMI_WATCHDOG

/**
 * do_nmi_callback
 *
 * Check to see if a callback exists and execute it.  Return 1
 * if the handler exists and was handled successfully.
 */
int do_nmi_callback(struct pt_regs *regs, int cpu);

extern void die_nmi(char *str, struct pt_regs *regs, int do_panic);
extern int check_nmi_watchdog(void);
extern int nmi_watchdog_enabled;
extern int avail_to_resrv_perfctr_nmi_bit(unsigned int);
extern int avail_to_resrv_perfctr_nmi(unsigned int);
extern int reserve_perfctr_nmi(unsigned int);
extern void release_perfctr_nmi(unsigned int);
extern int reserve_evntsel_nmi(unsigned int);
extern void release_evntsel_nmi(unsigned int);

extern void setup_apic_nmi_watchdog(void *);
extern void stop_apic_nmi_watchdog(void *);
extern void disable_timer_nmi_watchdog(void);
extern void enable_timer_nmi_watchdog(void);
extern int nmi_watchdog_tick(struct pt_regs *regs, unsigned reason);
extern void cpu_nmi_set_wd_enabled(void);

extern atomic_t nmi_active;
extern unsigned int nmi_watchdog;
#define NMI_NONE	0
#define NMI_IO_APIC	1
#define NMI_LOCAL_APIC	2
#define NMI_INVALID	3

struct ctl_table;
struct file;
extern int proc_nmi_enabled(struct ctl_table *, int , struct file *,
			void __user *, size_t *, loff_t *);
extern int unknown_nmi_panic;

void __trigger_all_cpu_backtrace(void);
#define trigger_all_cpu_backtrace() __trigger_all_cpu_backtrace()

static inline void localise_nmi_watchdog(void)
{
	if (nmi_watchdog == NMI_IO_APIC)
		nmi_watchdog = NMI_LOCAL_APIC;
}

/* check if nmi_watchdog is active (ie was specified at boot) */
static inline int nmi_watchdog_active(void)
{
	/*
	 * actually it should be:
	 * 	return (nmi_watchdog == NMI_LOCAL_APIC ||
	 * 		nmi_watchdog == NMI_IO_APIC)
	 * but since they are power of two we could use a
	 * cheaper way --cvg
	 */
	return nmi_watchdog & 0x3;
}
#endif

void lapic_watchdog_stop(void);
int lapic_watchdog_init(unsigned nmi_hz);
int lapic_wd_event(unsigned nmi_hz);
unsigned lapic_adjust_nmi_hz(unsigned hz);
int lapic_watchdog_ok(void);
void disable_lapic_nmi_watchdog(void);
void enable_lapic_nmi_watchdog(void);
void stop_nmi(void);
void restart_nmi(void);

#endif /* _ASM_X86_NMI_H */
