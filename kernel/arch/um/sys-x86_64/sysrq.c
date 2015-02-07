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
 * Copyright 2003 PathScale, Inc.
 *
 * Licensed under the GPL
 */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/sched.h>
#include <linux/utsname.h>
#include <asm/current.h>
#include <asm/ptrace.h>
#include "sysrq.h"

void __show_regs(struct pt_regs *regs)
{
	printk("\n");
	print_modules();
	printk(KERN_INFO "Pid: %d, comm: %.20s %s %s\n", task_pid_nr(current),
		current->comm, print_tainted(), init_utsname()->release);
	printk(KERN_INFO "RIP: %04lx:[<%016lx>]\n", PT_REGS_CS(regs) & 0xffff,
	       PT_REGS_RIP(regs));
	printk(KERN_INFO "RSP: %016lx  EFLAGS: %08lx\n", PT_REGS_RSP(regs),
	       PT_REGS_EFLAGS(regs));
	printk(KERN_INFO "RAX: %016lx RBX: %016lx RCX: %016lx\n",
	       PT_REGS_RAX(regs), PT_REGS_RBX(regs), PT_REGS_RCX(regs));
	printk(KERN_INFO "RDX: %016lx RSI: %016lx RDI: %016lx\n",
	       PT_REGS_RDX(regs), PT_REGS_RSI(regs), PT_REGS_RDI(regs));
	printk(KERN_INFO "RBP: %016lx R08: %016lx R09: %016lx\n",
	       PT_REGS_RBP(regs), PT_REGS_R8(regs), PT_REGS_R9(regs));
	printk(KERN_INFO "R10: %016lx R11: %016lx R12: %016lx\n",
	       PT_REGS_R10(regs), PT_REGS_R11(regs), PT_REGS_R12(regs));
	printk(KERN_INFO "R13: %016lx R14: %016lx R15: %016lx\n",
	       PT_REGS_R13(regs), PT_REGS_R14(regs), PT_REGS_R15(regs));
}

void show_regs(struct pt_regs *regs)
{
	__show_regs(regs);
	show_trace(current, (unsigned long *) &regs);
}
