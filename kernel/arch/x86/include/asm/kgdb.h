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

#ifndef _ASM_X86_KGDB_H
#define _ASM_X86_KGDB_H

/*
 * Copyright (C) 2001-2004 Amit S. Kale
 * Copyright (C) 2008 Wind River Systems, Inc.
 */

/*
 * BUFMAX defines the maximum number of characters in inbound/outbound
 * buffers at least NUMREGBYTES*2 are needed for register packets
 * Longer buffer is needed to list all threads
 */
#define BUFMAX			1024

/*
 *  Note that this register image is in a different order than
 *  the register image that Linux produces at interrupt time.
 *
 *  Linux's register image is defined by struct pt_regs in ptrace.h.
 *  Just why GDB uses a different order is a historical mystery.
 */
#ifdef CONFIG_X86_32
enum regnames {
	GDB_AX,			/* 0 */
	GDB_CX,			/* 1 */
	GDB_DX,			/* 2 */
	GDB_BX,			/* 3 */
	GDB_SP,			/* 4 */
	GDB_BP,			/* 5 */
	GDB_SI,			/* 6 */
	GDB_DI,			/* 7 */
	GDB_PC,			/* 8 also known as eip */
	GDB_PS,			/* 9 also known as eflags */
	GDB_CS,			/* 10 */
	GDB_SS,			/* 11 */
	GDB_DS,			/* 12 */
	GDB_ES,			/* 13 */
	GDB_FS,			/* 14 */
	GDB_GS,			/* 15 */
};
#define NUMREGBYTES		((GDB_GS+1)*4)
#else /* ! CONFIG_X86_32 */
enum regnames64 {
	GDB_AX,			/* 0 */
	GDB_BX,			/* 1 */
	GDB_CX,			/* 2 */
	GDB_DX,			/* 3 */
	GDB_SI,			/* 4 */
	GDB_DI,			/* 5 */
	GDB_BP,			/* 6 */
	GDB_SP,			/* 7 */
	GDB_R8,			/* 8 */
	GDB_R9,			/* 9 */
	GDB_R10,		/* 10 */
	GDB_R11,		/* 11 */
	GDB_R12,		/* 12 */
	GDB_R13,		/* 13 */
	GDB_R14,		/* 14 */
	GDB_R15,		/* 15 */
	GDB_PC,			/* 16 */
};

enum regnames32 {
	GDB_PS = 34,
	GDB_CS,
	GDB_SS,
};
#define NUMREGBYTES		((GDB_SS+1)*4)
#endif /* CONFIG_X86_32 */

static inline void arch_kgdb_breakpoint(void)
{
	asm("   int $3");
}
#define BREAK_INSTR_SIZE	1
#define CACHE_FLUSH_IS_SAFE	1

#endif /* _ASM_X86_KGDB_H */
