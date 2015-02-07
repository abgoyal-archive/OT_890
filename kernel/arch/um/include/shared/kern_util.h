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
 * Copyright (C) 2000 - 2007 Jeff Dike (jdike@{addtoit,linux.intel}.com)
 * Licensed under the GPL
 */

#ifndef __KERN_UTIL_H__
#define __KERN_UTIL_H__

#include "sysdep/ptrace.h"
#include "sysdep/faultinfo.h"

extern int uml_exitcode;

extern int ncpus;
extern int kmalloc_ok;

#define UML_ROUND_UP(addr) \
	((((unsigned long) addr) + PAGE_SIZE - 1) & PAGE_MASK)

extern unsigned long alloc_stack(int order, int atomic);
extern void free_stack(unsigned long stack, int order);

extern int do_signal(void);
extern void copy_sc(struct uml_pt_regs *regs, void *from);
extern void interrupt_end(void);
extern void relay_signal(int sig, struct uml_pt_regs *regs);

extern unsigned long segv(struct faultinfo fi, unsigned long ip,
			  int is_user, struct uml_pt_regs *regs);
extern int handle_page_fault(unsigned long address, unsigned long ip,
			     int is_write, int is_user, int *code_out);

extern unsigned int do_IRQ(int irq, struct uml_pt_regs *regs);
extern int smp_sigio_handler(void);
extern void initial_thread_cb(void (*proc)(void *), void *arg);
extern int is_syscall(unsigned long addr);
extern void timer_handler(int sig, struct uml_pt_regs *regs);

extern void timer_handler(int sig, struct uml_pt_regs *regs);

extern int start_uml(void);
extern void paging_init(void);

extern void uml_cleanup(void);
extern void do_uml_exitcalls(void);

/*
 * Are we disallowed to sleep? Used to choose between GFP_KERNEL and
 * GFP_ATOMIC.
 */
extern int __cant_sleep(void);
extern void *get_current(void);
extern int copy_from_user_proc(void *to, void *from, int size);
extern int cpu(void);
extern char *uml_strdup(const char *string);

extern unsigned long to_irq_stack(unsigned long *mask_out);
extern unsigned long from_irq_stack(int nested);

extern void syscall_trace(struct uml_pt_regs *regs, int entryexit);
extern int singlestepping(void *t);

extern void segv_handler(int sig, struct uml_pt_regs *regs);
extern void bus_handler(int sig, struct uml_pt_regs *regs);
extern void winch(int sig, struct uml_pt_regs *regs);
extern void fatal_sigsegv(void) __attribute__ ((noreturn));


#endif
