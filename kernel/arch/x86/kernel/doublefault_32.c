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

#include <linux/mm.h>
#include <linux/sched.h>
#include <linux/init.h>
#include <linux/init_task.h>
#include <linux/fs.h>

#include <asm/uaccess.h>
#include <asm/pgtable.h>
#include <asm/processor.h>
#include <asm/desc.h>

#define DOUBLEFAULT_STACKSIZE (1024)
static unsigned long doublefault_stack[DOUBLEFAULT_STACKSIZE];
#define STACK_START (unsigned long)(doublefault_stack+DOUBLEFAULT_STACKSIZE)

#define ptr_ok(x) ((x) > PAGE_OFFSET && (x) < PAGE_OFFSET + MAXMEM)

static void doublefault_fn(void)
{
	struct desc_ptr gdt_desc = {0, 0};
	unsigned long gdt, tss;

	store_gdt(&gdt_desc);
	gdt = gdt_desc.address;

	printk(KERN_EMERG "PANIC: double fault, gdt at %08lx [%d bytes]\n", gdt, gdt_desc.size);

	if (ptr_ok(gdt)) {
		gdt += GDT_ENTRY_TSS << 3;
		tss = *(u16 *)(gdt+2);
		tss += *(u8 *)(gdt+4) << 16;
		tss += *(u8 *)(gdt+7) << 24;
		printk(KERN_EMERG "double fault, tss at %08lx\n", tss);

		if (ptr_ok(tss)) {
			struct x86_hw_tss *t = (struct x86_hw_tss *)tss;

			printk(KERN_EMERG "eip = %08lx, esp = %08lx\n",
			       t->ip, t->sp);

			printk(KERN_EMERG "eax = %08lx, ebx = %08lx, ecx = %08lx, edx = %08lx\n",
				t->ax, t->bx, t->cx, t->dx);
			printk(KERN_EMERG "esi = %08lx, edi = %08lx\n",
				t->si, t->di);
		}
	}

	for (;;)
		cpu_relax();
}

struct tss_struct doublefault_tss __cacheline_aligned = {
	.x86_tss = {
		.sp0		= STACK_START,
		.ss0		= __KERNEL_DS,
		.ldt		= 0,
		.io_bitmap_base	= INVALID_IO_BITMAP_OFFSET,

		.ip		= (unsigned long) doublefault_fn,
		/* 0x2 bit is always set */
		.flags		= X86_EFLAGS_SF | 0x2,
		.sp		= STACK_START,
		.es		= __USER_DS,
		.cs		= __KERNEL_CS,
		.ss		= __KERNEL_DS,
		.ds		= __USER_DS,
		.fs		= __KERNEL_PERCPU,

		.__cr3		= __pa_nodebug(swapper_pg_dir),
	}
};
