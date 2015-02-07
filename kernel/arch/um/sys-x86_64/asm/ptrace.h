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

#ifndef __UM_PTRACE_X86_64_H
#define __UM_PTRACE_X86_64_H

#include "linux/compiler.h"
#include "asm/errno.h"

#define __FRAME_OFFSETS /* Needed to get the R* macros */
#include "asm/ptrace-generic.h"

#define HOST_AUDIT_ARCH AUDIT_ARCH_X86_64

#define PT_REGS_RBX(r) UPT_RBX(&(r)->regs)
#define PT_REGS_RCX(r) UPT_RCX(&(r)->regs)
#define PT_REGS_RDX(r) UPT_RDX(&(r)->regs)
#define PT_REGS_RSI(r) UPT_RSI(&(r)->regs)
#define PT_REGS_RDI(r) UPT_RDI(&(r)->regs)
#define PT_REGS_RBP(r) UPT_RBP(&(r)->regs)
#define PT_REGS_RAX(r) UPT_RAX(&(r)->regs)
#define PT_REGS_R8(r) UPT_R8(&(r)->regs)
#define PT_REGS_R9(r) UPT_R9(&(r)->regs)
#define PT_REGS_R10(r) UPT_R10(&(r)->regs)
#define PT_REGS_R11(r) UPT_R11(&(r)->regs)
#define PT_REGS_R12(r) UPT_R12(&(r)->regs)
#define PT_REGS_R13(r) UPT_R13(&(r)->regs)
#define PT_REGS_R14(r) UPT_R14(&(r)->regs)
#define PT_REGS_R15(r) UPT_R15(&(r)->regs)

#define PT_REGS_FS(r) UPT_FS(&(r)->regs)
#define PT_REGS_GS(r) UPT_GS(&(r)->regs)
#define PT_REGS_DS(r) UPT_DS(&(r)->regs)
#define PT_REGS_ES(r) UPT_ES(&(r)->regs)
#define PT_REGS_SS(r) UPT_SS(&(r)->regs)
#define PT_REGS_CS(r) UPT_CS(&(r)->regs)

#define PT_REGS_ORIG_RAX(r) UPT_ORIG_RAX(&(r)->regs)
#define PT_REGS_RIP(r) UPT_IP(&(r)->regs)
#define PT_REGS_RSP(r) UPT_SP(&(r)->regs)

#define PT_REGS_EFLAGS(r) UPT_EFLAGS(&(r)->regs)

/* XXX */
#define user_mode(r) UPT_IS_USER(&(r)->regs)
#define PT_REGS_ORIG_SYSCALL(r) PT_REGS_RAX(r)
#define PT_REGS_SYSCALL_RET(r) PT_REGS_RAX(r)

#define PT_FIX_EXEC_STACK(sp) do ; while(0)

#define profile_pc(regs) PT_REGS_IP(regs)

struct user_desc;

static inline int ptrace_get_thread_area(struct task_struct *child, int idx,
                                         struct user_desc __user *user_desc)
{
        return -ENOSYS;
}

static inline int ptrace_set_thread_area(struct task_struct *child, int idx,
                                         struct user_desc __user *user_desc)
{
        return -ENOSYS;
}

extern long arch_prctl(struct task_struct *task, int code,
		       unsigned long __user *addr);
#endif
