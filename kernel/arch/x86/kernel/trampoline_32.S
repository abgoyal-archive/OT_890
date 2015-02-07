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
 *
 *	Trampoline.S	Derived from Setup.S by Linus Torvalds
 *
 *	4 Jan 1997 Michael Chastain: changed to gnu as.
 *
 *	This is only used for booting secondary CPUs in SMP machine
 *
 *	Entry: CS:IP point to the start of our code, we are 
 *	in real mode with no stack, but the rest of the 
 *	trampoline page to make our stack and everything else
 *	is a mystery.
 *
 *	We jump into arch/x86/kernel/head_32.S.
 *
 *	On entry to trampoline_data, the processor is in real mode
 *	with 16-bit addressing and 16-bit data.  CS has some value
 *	and IP is zero.  Thus, data addresses need to be absolute
 *	(no relocation) and are taken with regard to r_base.
 *
 *	If you work on this file, check the object module with
 *	objdump --reloc to make sure there are no relocation
 *	entries except for:
 *
 *	TYPE              VALUE
 *	R_386_32          startup_32_smp
 *	R_386_32          boot_gdt
 */

#include <linux/linkage.h>
#include <asm/segment.h>
#include <asm/page.h>

/* We can free up trampoline after bootup if cpu hotplug is not supported. */
#ifndef CONFIG_HOTPLUG_CPU
.section ".cpuinit.data","aw",@progbits
#else
.section .rodata,"a",@progbits
#endif

.code16

ENTRY(trampoline_data)
r_base = .
	wbinvd			# Needed for NUMA-Q should be harmless for others
	mov	%cs, %ax	# Code and data in the same place
	mov	%ax, %ds

	cli			# We should be safe anyway

	movl	$0xA5A5A5A5, trampoline_data - r_base
				# write marker for master knows we're running

	/* GDT tables in non default location kernel can be beyond 16MB and
	 * lgdt will not be able to load the address as in real mode default
	 * operand size is 16bit. Use lgdtl instead to force operand size
	 * to 32 bit.
	 */

	lidtl	boot_idt_descr - r_base	# load idt with 0, 0
	lgdtl	boot_gdt_descr - r_base	# load gdt with whatever is appropriate

	xor	%ax, %ax
	inc	%ax		# protected mode (PE) bit
	lmsw	%ax		# into protected mode
	# flush prefetch and jump to startup_32_smp in arch/i386/kernel/head.S
	ljmpl	$__BOOT_CS, $(startup_32_smp-__PAGE_OFFSET)

	# These need to be in the same 64K segment as the above;
	# hence we don't use the boot_gdt_descr defined in head.S
boot_gdt_descr:
	.word	__BOOT_DS + 7			# gdt limit
	.long	boot_gdt - __PAGE_OFFSET	# gdt base

boot_idt_descr:
	.word	0				# idt limit = 0
	.long	0				# idt base = 0L

.globl trampoline_end
trampoline_end:
