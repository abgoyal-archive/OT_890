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
 * Copyright 2001-2003 Pavel Machek <pavel@suse.cz>
 * Based on code
 * Copyright 2001 Patrick Mochel <mochel@osdl.org>
 */
#ifndef _ASM_X86_SUSPEND_64_H
#define _ASM_X86_SUSPEND_64_H

#include <asm/desc.h>
#include <asm/i387.h>

static inline int arch_prepare_suspend(void)
{
	return 0;
}

/*
 * Image of the saved processor state, used by the low level ACPI suspend to
 * RAM code and by the low level hibernation code.
 *
 * If you modify it, fix arch/x86/kernel/acpi/wakeup_64.S and make sure that
 * __save/__restore_processor_state(), defined in arch/x86/kernel/suspend_64.c,
 * still work as required.
 */
struct saved_context {
	struct pt_regs regs;
	u16 ds, es, fs, gs, ss;
	unsigned long gs_base, gs_kernel_base, fs_base;
	unsigned long cr0, cr2, cr3, cr4, cr8;
	unsigned long efer;
	u16 gdt_pad;
	u16 gdt_limit;
	unsigned long gdt_base;
	u16 idt_pad;
	u16 idt_limit;
	unsigned long idt_base;
	u16 ldt;
	u16 tss;
	unsigned long tr;
	unsigned long safety;
	unsigned long return_address;
} __attribute__((packed));

#define loaddebug(thread,register) \
	set_debugreg((thread)->debugreg##register, register)

/* routines for saving/restoring kernel state */
extern int acpi_save_state_mem(void);
extern char core_restore_code;
extern char restore_registers;

#endif /* _ASM_X86_SUSPEND_64_H */
