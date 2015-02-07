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
 * Suspend support specific for i386.
 *
 * Distribute under GPLv2
 *
 * Copyright (c) 2002 Pavel Machek <pavel@suse.cz>
 * Copyright (c) 2001 Patrick Mochel <mochel@osdl.org>
 */

#include <linux/module.h>
#include <linux/suspend.h>
#include <asm/mtrr.h>
#include <asm/mce.h>
#include <asm/xcr.h>

static struct saved_context saved_context;

unsigned long saved_context_ebx;
unsigned long saved_context_esp, saved_context_ebp;
unsigned long saved_context_esi, saved_context_edi;
unsigned long saved_context_eflags;

static void __save_processor_state(struct saved_context *ctxt)
{
	mtrr_save_fixed_ranges(NULL);
	kernel_fpu_begin();

	/*
	 * descriptor tables
	 */
	store_gdt(&ctxt->gdt);
	store_idt(&ctxt->idt);
	store_tr(ctxt->tr);

	/*
	 * segment registers
	 */
	savesegment(es, ctxt->es);
	savesegment(fs, ctxt->fs);
	savesegment(gs, ctxt->gs);
	savesegment(ss, ctxt->ss);

	/*
	 * control registers
	 */
	ctxt->cr0 = read_cr0();
	ctxt->cr2 = read_cr2();
	ctxt->cr3 = read_cr3();
	ctxt->cr4 = read_cr4_safe();
}

/* Needed by apm.c */
void save_processor_state(void)
{
	__save_processor_state(&saved_context);
}
EXPORT_SYMBOL(save_processor_state);

static void do_fpu_end(void)
{
	/*
	 * Restore FPU regs if necessary.
	 */
	kernel_fpu_end();
}

static void fix_processor_context(void)
{
	int cpu = smp_processor_id();
	struct tss_struct *t = &per_cpu(init_tss, cpu);

	set_tss_desc(cpu, t);	/*
				 * This just modifies memory; should not be
				 * necessary. But... This is necessary, because
				 * 386 hardware has concept of busy TSS or some
				 * similar stupidity.
				 */

	load_TR_desc();				/* This does ltr */
	load_LDT(&current->active_mm->context);	/* This does lldt */

	/*
	 * Now maybe reload the debug registers
	 */
	if (current->thread.debugreg7) {
		set_debugreg(current->thread.debugreg0, 0);
		set_debugreg(current->thread.debugreg1, 1);
		set_debugreg(current->thread.debugreg2, 2);
		set_debugreg(current->thread.debugreg3, 3);
		/* no 4 and 5 */
		set_debugreg(current->thread.debugreg6, 6);
		set_debugreg(current->thread.debugreg7, 7);
	}

}

static void __restore_processor_state(struct saved_context *ctxt)
{
	/*
	 * control registers
	 */
	/* cr4 was introduced in the Pentium CPU */
	if (ctxt->cr4)
		write_cr4(ctxt->cr4);
	write_cr3(ctxt->cr3);
	write_cr2(ctxt->cr2);
	write_cr0(ctxt->cr0);

	/*
	 * now restore the descriptor tables to their proper values
	 * ltr is done i fix_processor_context().
	 */
	load_gdt(&ctxt->gdt);
	load_idt(&ctxt->idt);

	/*
	 * segment registers
	 */
	loadsegment(es, ctxt->es);
	loadsegment(fs, ctxt->fs);
	loadsegment(gs, ctxt->gs);
	loadsegment(ss, ctxt->ss);

	/*
	 * sysenter MSRs
	 */
	if (boot_cpu_has(X86_FEATURE_SEP))
		enable_sep_cpu();

	/*
	 * restore XCR0 for xsave capable cpu's.
	 */
	if (cpu_has_xsave)
		xsetbv(XCR_XFEATURE_ENABLED_MASK, pcntxt_mask);

	fix_processor_context();
	do_fpu_end();
	mtrr_ap_init();
	mcheck_init(&boot_cpu_data);
}

/* Needed by apm.c */
void restore_processor_state(void)
{
	__restore_processor_state(&saved_context);
}
EXPORT_SYMBOL(restore_processor_state);
