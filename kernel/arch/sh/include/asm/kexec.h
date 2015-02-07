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

#ifndef __ASM_SH_KEXEC_H
#define __ASM_SH_KEXEC_H

#include <asm/ptrace.h>
#include <asm/string.h>

/*
 * KEXEC_SOURCE_MEMORY_LIMIT maximum page get_free_page can return.
 * I.e. Maximum page that is mapped directly into kernel memory,
 * and kmap is not required.
 *
 * Someone correct me if FIXADDR_START - PAGEOFFSET is not the correct
 * calculation for the amount of memory directly mappable into the
 * kernel memory space.
 */

/* Maximum physical address we can use pages from */
#define KEXEC_SOURCE_MEMORY_LIMIT (-1UL)
/* Maximum address we can reach in physical address mode */
#define KEXEC_DESTINATION_MEMORY_LIMIT (-1UL)
/* Maximum address we can use for the control code buffer */
#define KEXEC_CONTROL_MEMORY_LIMIT TASK_SIZE

#define KEXEC_CONTROL_PAGE_SIZE	4096

/* The native architecture */
#define KEXEC_ARCH KEXEC_ARCH_SH

static inline void crash_setup_regs(struct pt_regs *newregs,
				    struct pt_regs *oldregs)
{
	if (oldregs)
		memcpy(newregs, oldregs, sizeof(*newregs));
	else {
		__asm__ __volatile__ ("mov r0, %0" : "=r" (newregs->regs[0]));
		__asm__ __volatile__ ("mov r1, %0" : "=r" (newregs->regs[1]));
		__asm__ __volatile__ ("mov r2, %0" : "=r" (newregs->regs[2]));
		__asm__ __volatile__ ("mov r3, %0" : "=r" (newregs->regs[3]));
		__asm__ __volatile__ ("mov r4, %0" : "=r" (newregs->regs[4]));
		__asm__ __volatile__ ("mov r5, %0" : "=r" (newregs->regs[5]));
		__asm__ __volatile__ ("mov r6, %0" : "=r" (newregs->regs[6]));
		__asm__ __volatile__ ("mov r7, %0" : "=r" (newregs->regs[7]));
		__asm__ __volatile__ ("mov r8, %0" : "=r" (newregs->regs[8]));
		__asm__ __volatile__ ("mov r9, %0" : "=r" (newregs->regs[9]));
		__asm__ __volatile__ ("mov r10, %0" : "=r" (newregs->regs[10]));
		__asm__ __volatile__ ("mov r11, %0" : "=r" (newregs->regs[11]));
		__asm__ __volatile__ ("mov r12, %0" : "=r" (newregs->regs[12]));
		__asm__ __volatile__ ("mov r13, %0" : "=r" (newregs->regs[13]));
		__asm__ __volatile__ ("mov r14, %0" : "=r" (newregs->regs[14]));
		__asm__ __volatile__ ("mov r15, %0" : "=r" (newregs->regs[15]));

		__asm__ __volatile__ ("sts pr, %0" : "=r" (newregs->pr));
		__asm__ __volatile__ ("sts macl, %0" : "=r" (newregs->macl));
		__asm__ __volatile__ ("sts mach, %0" : "=r" (newregs->mach));

		__asm__ __volatile__ ("stc gbr, %0" : "=r" (newregs->gbr));
		__asm__ __volatile__ ("stc sr, %0" : "=r" (newregs->sr));

		newregs->pc = (unsigned long)current_text_addr();
	}
}
#endif /* __ASM_SH_KEXEC_H */
