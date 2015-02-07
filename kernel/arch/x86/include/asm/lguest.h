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

#ifndef _ASM_X86_LGUEST_H
#define _ASM_X86_LGUEST_H

#define GDT_ENTRY_LGUEST_CS	10
#define GDT_ENTRY_LGUEST_DS	11
#define LGUEST_CS		(GDT_ENTRY_LGUEST_CS * 8)
#define LGUEST_DS		(GDT_ENTRY_LGUEST_DS * 8)

#ifndef __ASSEMBLY__
#include <asm/desc.h>

#define GUEST_PL 1

/* Every guest maps the core switcher code. */
#define SHARED_SWITCHER_PAGES \
	DIV_ROUND_UP(end_switcher_text - start_switcher_text, PAGE_SIZE)
/* Pages for switcher itself, then two pages per cpu */
#define TOTAL_SWITCHER_PAGES (SHARED_SWITCHER_PAGES + 2 * nr_cpu_ids)

/* We map at -4M for ease of mapping into the guest (one PTE page). */
#define SWITCHER_ADDR 0xFFC00000

/* Found in switcher.S */
extern unsigned long default_idt_entries[];

/* Declarations for definitions in lguest_guest.S */
extern char lguest_noirq_start[], lguest_noirq_end[];
extern const char lgstart_cli[], lgend_cli[];
extern const char lgstart_sti[], lgend_sti[];
extern const char lgstart_popf[], lgend_popf[];
extern const char lgstart_pushf[], lgend_pushf[];
extern const char lgstart_iret[], lgend_iret[];

extern void lguest_iret(void);
extern void lguest_init(void);

struct lguest_regs {
	/* Manually saved part. */
	unsigned long eax, ebx, ecx, edx;
	unsigned long esi, edi, ebp;
	unsigned long gs;
	unsigned long fs, ds, es;
	unsigned long trapnum, errcode;
	/* Trap pushed part */
	unsigned long eip;
	unsigned long cs;
	unsigned long eflags;
	unsigned long esp;
	unsigned long ss;
};

/* This is a guest-specific page (mapped ro) into the guest. */
struct lguest_ro_state {
	/* Host information we need to restore when we switch back. */
	u32 host_cr3;
	struct desc_ptr host_idt_desc;
	struct desc_ptr host_gdt_desc;
	u32 host_sp;

	/* Fields which are used when guest is running. */
	struct desc_ptr guest_idt_desc;
	struct desc_ptr guest_gdt_desc;
	struct x86_hw_tss guest_tss;
	struct desc_struct guest_idt[IDT_ENTRIES];
	struct desc_struct guest_gdt[GDT_ENTRIES];
};

struct lg_cpu_arch {
	/* The GDT entries copied into lguest_ro_state when running. */
	struct desc_struct gdt[GDT_ENTRIES];

	/* The IDT entries: some copied into lguest_ro_state when running. */
	struct desc_struct idt[IDT_ENTRIES];

	/* The address of the last guest-visible pagefault (ie. cr2). */
	unsigned long last_pagefault;
};

static inline void lguest_set_ts(void)
{
	u32 cr0;

	cr0 = read_cr0();
	if (!(cr0 & 8))
		write_cr0(cr0 | 8);
}

/* Full 4G segment descriptors, suitable for CS and DS. */
#define FULL_EXEC_SEGMENT ((struct desc_struct){ { {0x0000ffff, 0x00cf9b00} } })
#define FULL_SEGMENT ((struct desc_struct){ { {0x0000ffff, 0x00cf9300} } })

#endif /* __ASSEMBLY__ */

#endif /* _ASM_X86_LGUEST_H */
