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

/* a.out coredump register dumper
 *
 * Copyright (C) 2007 Red Hat, Inc. All Rights Reserved.
 * Written by David Howells (dhowells@redhat.com)
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public Licence
 * as published by the Free Software Foundation; either version
 * 2 of the Licence, or (at your option) any later version.
 */

#ifndef _ASM_X86_A_OUT_CORE_H
#define _ASM_X86_A_OUT_CORE_H

#ifdef __KERNEL__
#ifdef CONFIG_X86_32

#include <linux/user.h>
#include <linux/elfcore.h>

/*
 * fill in the user structure for an a.out core dump
 */
static inline void aout_dump_thread(struct pt_regs *regs, struct user *dump)
{
/* changed the size calculations - should hopefully work better. lbt */
	dump->magic = CMAGIC;
	dump->start_code = 0;
	dump->start_stack = regs->sp & ~(PAGE_SIZE - 1);
	dump->u_tsize = ((unsigned long)current->mm->end_code) >> PAGE_SHIFT;
	dump->u_dsize = ((unsigned long)(current->mm->brk + (PAGE_SIZE - 1)))
			>> PAGE_SHIFT;
	dump->u_dsize -= dump->u_tsize;
	dump->u_ssize = 0;
	dump->u_debugreg[0] = current->thread.debugreg0;
	dump->u_debugreg[1] = current->thread.debugreg1;
	dump->u_debugreg[2] = current->thread.debugreg2;
	dump->u_debugreg[3] = current->thread.debugreg3;
	dump->u_debugreg[4] = 0;
	dump->u_debugreg[5] = 0;
	dump->u_debugreg[6] = current->thread.debugreg6;
	dump->u_debugreg[7] = current->thread.debugreg7;

	if (dump->start_stack < TASK_SIZE)
		dump->u_ssize = ((unsigned long)(TASK_SIZE - dump->start_stack))
				>> PAGE_SHIFT;

	dump->regs.bx = regs->bx;
	dump->regs.cx = regs->cx;
	dump->regs.dx = regs->dx;
	dump->regs.si = regs->si;
	dump->regs.di = regs->di;
	dump->regs.bp = regs->bp;
	dump->regs.ax = regs->ax;
	dump->regs.ds = (u16)regs->ds;
	dump->regs.es = (u16)regs->es;
	dump->regs.fs = (u16)regs->fs;
	savesegment(gs, dump->regs.gs);
	dump->regs.orig_ax = regs->orig_ax;
	dump->regs.ip = regs->ip;
	dump->regs.cs = (u16)regs->cs;
	dump->regs.flags = regs->flags;
	dump->regs.sp = regs->sp;
	dump->regs.ss = (u16)regs->ss;

	dump->u_fpvalid = dump_fpu(regs, &dump->i387);
}

#endif /* CONFIG_X86_32 */
#endif /* __KERNEL__ */
#endif /* _ASM_X86_A_OUT_CORE_H */
