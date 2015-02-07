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

#include <linux/bug.h>
#include <linux/io.h>
#include <linux/types.h>
#include <linux/kdebug.h>
#include <linux/signal.h>
#include <linux/sched.h>
#include <linux/uaccess.h>
#include <asm/system.h>

#ifdef CONFIG_BUG
static void handle_BUG(struct pt_regs *regs)
{
	enum bug_trap_type tt;
	tt = report_bug(regs->pc, regs);
	if (tt == BUG_TRAP_TYPE_WARN) {
		regs->pc += instruction_size(regs->pc);
		return;
	}

	die("Kernel BUG", regs, TRAPA_BUG_OPCODE & 0xff);
}

int is_valid_bugaddr(unsigned long addr)
{
	unsigned short opcode;

	if (addr < PAGE_OFFSET)
		return 0;
	if (probe_kernel_address((u16 *)addr, opcode))
		return 0;

	return opcode == TRAPA_BUG_OPCODE;
}
#endif

/*
 * Generic trap handler.
 */
BUILD_TRAP_HANDLER(debug)
{
	TRAP_HANDLER_DECL;

	/* Rewind */
	regs->pc -= instruction_size(ctrl_inw(regs->pc - 4));

	if (notify_die(DIE_TRAP, "debug trap", regs, 0, vec & 0xff,
		       SIGTRAP) == NOTIFY_STOP)
		return;

	force_sig(SIGTRAP, current);
}

/*
 * Special handler for BUG() traps.
 */
BUILD_TRAP_HANDLER(bug)
{
	TRAP_HANDLER_DECL;

	/* Rewind */
	regs->pc -= instruction_size(ctrl_inw(regs->pc - 4));

	if (notify_die(DIE_TRAP, "bug trap", regs, 0, TRAPA_BUG_OPCODE & 0xff,
		       SIGTRAP) == NOTIFY_STOP)
		return;

#ifdef CONFIG_BUG
	if (__kernel_text_address(instruction_pointer(regs))) {
		opcode_t insn = *(opcode_t *)instruction_pointer(regs);
		if (insn == TRAPA_BUG_OPCODE)
			handle_BUG(regs);
	}
#endif

	force_sig(SIGTRAP, current);
}
