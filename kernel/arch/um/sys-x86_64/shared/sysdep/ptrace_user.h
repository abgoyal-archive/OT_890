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

#ifndef __SYSDEP_X86_64_PTRACE_USER_H__
#define __SYSDEP_X86_64_PTRACE_USER_H__

#define __FRAME_OFFSETS
#include <sys/ptrace.h>
#include <linux/ptrace.h>
#include <asm/ptrace.h>
#undef __FRAME_OFFSETS
#include "user_constants.h"

#define PT_INDEX(off) ((off) / sizeof(unsigned long))

#define PT_SYSCALL_NR(regs) ((regs)[PT_INDEX(ORIG_RAX)])
#define PT_SYSCALL_NR_OFFSET (ORIG_RAX)

#define PT_SYSCALL_ARG1(regs) (((unsigned long *) (regs))[PT_INDEX(RDI)])
#define PT_SYSCALL_ARG1_OFFSET (RDI)

#define PT_SYSCALL_ARG2(regs) (((unsigned long *) (regs))[PT_INDEX(RSI)])
#define PT_SYSCALL_ARG2_OFFSET (RSI)

#define PT_SYSCALL_ARG3(regs) (((unsigned long *) (regs))[PT_INDEX(RDX)])
#define PT_SYSCALL_ARG3_OFFSET (RDX)

#define PT_SYSCALL_ARG4(regs) (((unsigned long *) (regs))[PT_INDEX(RCX)])
#define PT_SYSCALL_ARG4_OFFSET (RCX)

#define PT_SYSCALL_ARG5(regs) (((unsigned long *) (regs))[PT_INDEX(R8)])
#define PT_SYSCALL_ARG5_OFFSET (R8)

#define PT_SYSCALL_ARG6(regs) (((unsigned long *) (regs))[PT_INDEX(R9)])
#define PT_SYSCALL_ARG6_OFFSET (R9)

#define PT_SYSCALL_RET_OFFSET (RAX)

#define PT_IP_OFFSET (RIP)
#define PT_IP(regs) ((regs)[PT_INDEX(RIP)])

#define PT_SP_OFFSET (RSP)
#define PT_SP(regs) ((regs)[PT_INDEX(RSP)])

#define PT_ORIG_RAX_OFFSET (ORIG_RAX)
#define PT_ORIG_RAX(regs) ((regs)[PT_INDEX(ORIG_RAX)])

/*
 * x86_64 FC3 doesn't define this in /usr/include/linux/ptrace.h even though
 * it's defined in the kernel's include/linux/ptrace.h. Additionally, use the
 * 2.4 name and value for 2.4 host compatibility.
 */
#ifndef PTRACE_OLDSETOPTIONS
#define PTRACE_OLDSETOPTIONS 21
#endif

/*
 * These are before the system call, so the system call number is RAX
 * rather than ORIG_RAX, and arg4 is R10 rather than RCX
 */
#define REGS_SYSCALL_NR PT_INDEX(RAX)
#define REGS_SYSCALL_ARG1 PT_INDEX(RDI)
#define REGS_SYSCALL_ARG2 PT_INDEX(RSI)
#define REGS_SYSCALL_ARG3 PT_INDEX(RDX)
#define REGS_SYSCALL_ARG4 PT_INDEX(R10)
#define REGS_SYSCALL_ARG5 PT_INDEX(R8)
#define REGS_SYSCALL_ARG6 PT_INDEX(R9)

#define REGS_IP_INDEX PT_INDEX(RIP)
#define REGS_SP_INDEX PT_INDEX(RSP)

#define FP_SIZE (HOST_FP_SIZE)

#endif
