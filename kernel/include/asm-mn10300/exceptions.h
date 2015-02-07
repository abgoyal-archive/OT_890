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

/* MN10300 Microcontroller core exceptions
 *
 * Copyright (C) 2007 Red Hat, Inc. All Rights Reserved.
 * Written by David Howells (dhowells@redhat.com)
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public Licence
 * as published by the Free Software Foundation; either version
 * 2 of the Licence, or (at your option) any later version.
 */
#ifndef _ASM_EXCEPTIONS_H
#define _ASM_EXCEPTIONS_H

#include <linux/linkage.h>

/*
 * define the breakpoint instruction opcode to use
 * - note that the JTAG unit steals 0xFF, so we want to avoid that if we can
 *   (can use 0xF7)
 */
#define GDBSTUB_BKPT		0xFF

#ifndef __ASSEMBLY__

/*
 * enumeration of exception codes (as extracted from TBR MSW)
 */
enum exception_code {
	EXCEP_RESET		= 0x000000,	/* reset */

	/* MMU exceptions */
	EXCEP_ITLBMISS		= 0x000100,	/* instruction TLB miss */
	EXCEP_DTLBMISS		= 0x000108,	/* data TLB miss */
	EXCEP_IAERROR		= 0x000110,	/* instruction address */
	EXCEP_DAERROR		= 0x000118,	/* data address */

	/* system exceptions */
	EXCEP_TRAP		= 0x000128,	/* program interrupt (PI instruction) */
	EXCEP_ISTEP		= 0x000130,	/* single step */
	EXCEP_IBREAK		= 0x000150,	/* instruction breakpoint */
	EXCEP_OBREAK		= 0x000158,	/* operand breakpoint */
	EXCEP_PRIVINS		= 0x000160,	/* privileged instruction execution */
	EXCEP_UNIMPINS		= 0x000168,	/* unimplemented instruction execution */
	EXCEP_UNIMPEXINS	= 0x000170,	/* unimplemented extended instruction execution */
	EXCEP_MEMERR		= 0x000178,	/* illegal memory access */
	EXCEP_MISALIGN		= 0x000180,	/* misalignment */
	EXCEP_BUSERROR		= 0x000188,	/* bus error */
	EXCEP_ILLINSACC		= 0x000190,	/* illegal instruction access */
	EXCEP_ILLDATACC		= 0x000198,	/* illegal data access */
	EXCEP_IOINSACC		= 0x0001a0,	/* I/O space instruction access */
	EXCEP_PRIVINSACC	= 0x0001a8,	/* privileged space instruction access */
	EXCEP_PRIVDATACC	= 0x0001b0,	/* privileged space data access */
	EXCEP_DATINSACC		= 0x0001b8,	/* data space instruction access */
	EXCEP_DOUBLE_FAULT	= 0x000200,	/* double fault */

	/* FPU exceptions */
	EXCEP_FPU_DISABLED	= 0x0001c0,	/* FPU disabled */
	EXCEP_FPU_UNIMPINS	= 0x0001c8,	/* FPU unimplemented operation */
	EXCEP_FPU_OPERATION	= 0x0001d0,	/* FPU operation */

	/* interrupts */
	EXCEP_WDT		= 0x000240,	/* watchdog timer overflow */
	EXCEP_NMI		= 0x000248,	/* non-maskable interrupt */
	EXCEP_IRQ_LEVEL0	= 0x000280,	/* level 0 maskable interrupt */
	EXCEP_IRQ_LEVEL1	= 0x000288,	/* level 1 maskable interrupt */
	EXCEP_IRQ_LEVEL2	= 0x000290,	/* level 2 maskable interrupt */
	EXCEP_IRQ_LEVEL3	= 0x000298,	/* level 3 maskable interrupt */
	EXCEP_IRQ_LEVEL4	= 0x0002a0,	/* level 4 maskable interrupt */
	EXCEP_IRQ_LEVEL5	= 0x0002a8,	/* level 5 maskable interrupt */
	EXCEP_IRQ_LEVEL6	= 0x0002b0,	/* level 6 maskable interrupt */

	/* system calls */
	EXCEP_SYSCALL0		= 0x000300,	/* system call 0 */
	EXCEP_SYSCALL1		= 0x000308,	/* system call 1 */
	EXCEP_SYSCALL2		= 0x000310,	/* system call 2 */
	EXCEP_SYSCALL3		= 0x000318,	/* system call 3 */
	EXCEP_SYSCALL4		= 0x000320,	/* system call 4 */
	EXCEP_SYSCALL5		= 0x000328,	/* system call 5 */
	EXCEP_SYSCALL6		= 0x000330,	/* system call 6 */
	EXCEP_SYSCALL7		= 0x000338,	/* system call 7 */
	EXCEP_SYSCALL8		= 0x000340,	/* system call 8 */
	EXCEP_SYSCALL9		= 0x000348,	/* system call 9 */
	EXCEP_SYSCALL10		= 0x000350,	/* system call 10 */
	EXCEP_SYSCALL11		= 0x000358,	/* system call 11 */
	EXCEP_SYSCALL12		= 0x000360,	/* system call 12 */
	EXCEP_SYSCALL13		= 0x000368,	/* system call 13 */
	EXCEP_SYSCALL14		= 0x000370,	/* system call 14 */
	EXCEP_SYSCALL15		= 0x000378,	/* system call 15 */
};

extern void __set_intr_stub(enum exception_code code, void *handler);
extern void set_intr_stub(enum exception_code code, void *handler);
extern void set_jtag_stub(enum exception_code code, void *handler);

struct pt_regs;

extern asmlinkage void __common_exception(void);
extern asmlinkage void itlb_miss(void);
extern asmlinkage void dtlb_miss(void);
extern asmlinkage void itlb_aerror(void);
extern asmlinkage void dtlb_aerror(void);
extern asmlinkage void raw_bus_error(void);
extern asmlinkage void double_fault(void);
extern asmlinkage int  system_call(struct pt_regs *);
extern asmlinkage void fpu_exception(struct pt_regs *, enum exception_code);
extern asmlinkage void nmi(struct pt_regs *, enum exception_code);
extern asmlinkage void uninitialised_exception(struct pt_regs *,
					       enum exception_code);
extern asmlinkage void irq_handler(void);
extern asmlinkage void profile_handler(void);
extern asmlinkage void nmi_handler(void);
extern asmlinkage void misalignment(struct pt_regs *, enum exception_code);

extern void die(const char *, struct pt_regs *, enum exception_code)
	ATTRIB_NORET;

extern int die_if_no_fixup(const char *, struct pt_regs *, enum exception_code);

#endif /* __ASSEMBLY__ */

#endif /* _ASM_EXCEPTIONS_H */
