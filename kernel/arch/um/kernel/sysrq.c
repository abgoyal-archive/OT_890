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
 * Copyright (C) 2001 - 2007 Jeff Dike (jdike@{addtoit,linux.intel}.com)
 * Licensed under the GPL
 */

#include <linux/kallsyms.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/sched.h>
#include "sysrq.h"

/* Catch non-i386 SUBARCH's. */
#if !defined(CONFIG_UML_X86) || defined(CONFIG_64BIT)
void show_trace(struct task_struct *task, unsigned long * stack)
{
	unsigned long addr;

	if (!stack) {
		stack = (unsigned long*) &stack;
		WARN_ON(1);
	}

	printk(KERN_INFO "Call Trace: \n");
	while (((long) stack & (THREAD_SIZE-1)) != 0) {
		addr = *stack;
		if (__kernel_text_address(addr)) {
			printk(KERN_INFO "%08lx:  [<%08lx>]",
			       (unsigned long) stack, addr);
			print_symbol(KERN_CONT " %s", addr);
			printk(KERN_CONT "\n");
		}
		stack++;
	}
	printk(KERN_INFO "\n");
}
#endif

/*
 * stack dumps generator - this is used by arch-independent code.
 * And this is identical to i386 currently.
 */
void dump_stack(void)
{
	unsigned long stack;

	show_trace(current, &stack);
}
EXPORT_SYMBOL(dump_stack);

/*Stolen from arch/i386/kernel/traps.c */
static const int kstack_depth_to_print = 24;

/* This recently started being used in arch-independent code too, as in
 * kernel/sched.c.*/
void show_stack(struct task_struct *task, unsigned long *esp)
{
	unsigned long *stack;
	int i;

	if (esp == NULL) {
		if (task != current && task != NULL) {
			esp = (unsigned long *) KSTK_ESP(task);
		} else {
			esp = (unsigned long *) &esp;
		}
	}

	stack = esp;
	for (i = 0; i < kstack_depth_to_print; i++) {
		if (kstack_end(stack))
			break;
		if (i && ((i % 8) == 0))
			printk("\n" KERN_INFO "       ");
		printk("%08lx ", *stack++);
	}

	show_trace(task, esp);
}
