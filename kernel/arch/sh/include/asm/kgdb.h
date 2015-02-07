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

#ifndef __ASM_SH_KGDB_H
#define __ASM_SH_KGDB_H

#include <asm/cacheflush.h>
#include <asm/ptrace.h>

/* Same as pt_regs but has vbr in place of syscall_nr */
struct kgdb_regs {
        unsigned long regs[16];
        unsigned long pc;
        unsigned long pr;
        unsigned long sr;
        unsigned long gbr;
        unsigned long mach;
        unsigned long macl;
        unsigned long vbr;
};

enum regnames {
	GDB_R0, GDB_R1, GDB_R2, GDB_R3, GDB_R4, GDB_R5, GDB_R6, GDB_R7,
	GDB_R8, GDB_R9, GDB_R10, GDB_R11, GDB_R12, GDB_R13, GDB_R14, GDB_R15,

	GDB_PC, GDB_PR, GDB_SR, GDB_GBR, GDB_MACH, GDB_MACL, GDB_VBR,
};

#define NUMREGBYTES    ((GDB_VBR + 1) * 4)

static inline void arch_kgdb_breakpoint(void)
{
	__asm__ __volatile__ ("trapa #0x3c\n");
}

/* State info */
extern char in_nmi;		/* Debounce flag to prevent NMI reentry*/

#define BUFMAX                 2048

#define CACHE_FLUSH_IS_SAFE	1
#define BREAK_INSTR_SIZE	2

#endif /* __ASM_SH_KGDB_H */
