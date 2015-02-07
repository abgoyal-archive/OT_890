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
 * Instruction-patching support.
 *
 * Copyright (C) 2003 Hewlett-Packard Co
 *	David Mosberger-Tang <davidm@hpl.hp.com>
 */
#include <linux/init.h>
#include <linux/string.h>

#include <asm/patch.h>
#include <asm/processor.h>
#include <asm/sections.h>
#include <asm/system.h>
#include <asm/unistd.h>

/*
 * This was adapted from code written by Tony Luck:
 *
 * The 64-bit value in a "movl reg=value" is scattered between the two words of the bundle
 * like this:
 *
 * 6  6         5         4         3         2         1
 * 3210987654321098765432109876543210987654321098765432109876543210
 * ABBBBBBBBBBBBBBBBBBBBBBBCCCCCCCCCCCCCCCCCCDEEEEEFFFFFFFFFGGGGGGG
 *
 * CCCCCCCCCCCCCCCCCCxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
 * xxxxAFFFFFFFFFEEEEEDxGGGGGGGxxxxxxxxxxxxxBBBBBBBBBBBBBBBBBBBBBBB
 */
static u64
get_imm64 (u64 insn_addr)
{
	u64 *p = (u64 *) (insn_addr & -16);	/* mask out slot number */

	return ( (p[1] & 0x0800000000000000UL) << 4)  | /*A*/
		((p[1] & 0x00000000007fffffUL) << 40) | /*B*/
		((p[0] & 0xffffc00000000000UL) >> 24) | /*C*/
		((p[1] & 0x0000100000000000UL) >> 23) | /*D*/
		((p[1] & 0x0003e00000000000UL) >> 29) | /*E*/
		((p[1] & 0x07fc000000000000UL) >> 43) | /*F*/
		((p[1] & 0x000007f000000000UL) >> 36);  /*G*/
}

/* Patch instruction with "val" where "mask" has 1 bits. */
void
ia64_patch (u64 insn_addr, u64 mask, u64 val)
{
	u64 m0, m1, v0, v1, b0, b1, *b = (u64 *) (insn_addr & -16);
#	define insn_mask ((1UL << 41) - 1)
	unsigned long shift;

	b0 = b[0]; b1 = b[1];
	shift = 5 + 41 * (insn_addr % 16); /* 5 bits of template, then 3 x 41-bit instructions */
	if (shift >= 64) {
		m1 = mask << (shift - 64);
		v1 = val << (shift - 64);
	} else {
		m0 = mask << shift; m1 = mask >> (64 - shift);
		v0 = val  << shift; v1 = val >> (64 - shift);
		b[0] = (b0 & ~m0) | (v0 & m0);
	}
	b[1] = (b1 & ~m1) | (v1 & m1);
}

void
ia64_patch_imm64 (u64 insn_addr, u64 val)
{
	/* The assembler may generate offset pointing to either slot 1
	   or slot 2 for a long (2-slot) instruction, occupying slots 1
	   and 2.  */
  	insn_addr &= -16UL;
	ia64_patch(insn_addr + 2,
		   0x01fffefe000UL, (  ((val & 0x8000000000000000UL) >> 27) /* bit 63 -> 36 */
				     | ((val & 0x0000000000200000UL) <<  0) /* bit 21 -> 21 */
				     | ((val & 0x00000000001f0000UL) <<  6) /* bit 16 -> 22 */
				     | ((val & 0x000000000000ff80UL) << 20) /* bit  7 -> 27 */
				     | ((val & 0x000000000000007fUL) << 13) /* bit  0 -> 13 */));
	ia64_patch(insn_addr + 1, 0x1ffffffffffUL, val >> 22);
}

void
ia64_patch_imm60 (u64 insn_addr, u64 val)
{
	/* The assembler may generate offset pointing to either slot 1
	   or slot 2 for a long (2-slot) instruction, occupying slots 1
	   and 2.  */
  	insn_addr &= -16UL;
	ia64_patch(insn_addr + 2,
		   0x011ffffe000UL, (  ((val & 0x0800000000000000UL) >> 23) /* bit 59 -> 36 */
				     | ((val & 0x00000000000fffffUL) << 13) /* bit  0 -> 13 */));
	ia64_patch(insn_addr + 1, 0x1fffffffffcUL, val >> 18);
}

/*
 * We need sometimes to load the physical address of a kernel
 * object.  Often we can convert the virtual address to physical
 * at execution time, but sometimes (either for performance reasons
 * or during error recovery) we cannot to this.  Patch the marked
 * bundles to load the physical address.
 */
void __init
ia64_patch_vtop (unsigned long start, unsigned long end)
{
	s32 *offp = (s32 *) start;
	u64 ip;

	while (offp < (s32 *) end) {
		ip = (u64) offp + *offp;

		/* replace virtual address with corresponding physical address: */
		ia64_patch_imm64(ip, ia64_tpa(get_imm64(ip)));
		ia64_fc((void *) ip);
		++offp;
	}
	ia64_sync_i();
	ia64_srlz_i();
}

/*
 * Disable the RSE workaround by turning the conditional branch
 * that we tagged in each place the workaround was used into an
 * unconditional branch.
 */
void __init
ia64_patch_rse (unsigned long start, unsigned long end)
{
	s32 *offp = (s32 *) start;
	u64 ip, *b;

	while (offp < (s32 *) end) {
		ip = (u64) offp + *offp;

		b = (u64 *)(ip & -16);
		b[1] &= ~0xf800000L;
		ia64_fc((void *) ip);
		++offp;
	}
	ia64_sync_i();
	ia64_srlz_i();
}

void __init
ia64_patch_mckinley_e9 (unsigned long start, unsigned long end)
{
	static int first_time = 1;
	int need_workaround;
	s32 *offp = (s32 *) start;
	u64 *wp;

	need_workaround = (local_cpu_data->family == 0x1f && local_cpu_data->model == 0);

	if (first_time) {
		first_time = 0;
		if (need_workaround)
			printk(KERN_INFO "Leaving McKinley Errata 9 workaround enabled\n");
	}
	if (need_workaround)
		return;

	while (offp < (s32 *) end) {
		wp = (u64 *) ia64_imva((char *) offp + *offp);
		wp[0] = 0x0000000100000011UL; /* nop.m 0; nop.i 0; br.ret.sptk.many b6 */
		wp[1] = 0x0084006880000200UL;
		wp[2] = 0x0000000100000000UL; /* nop.m 0; nop.i 0; nop.i 0 */
		wp[3] = 0x0004000000000200UL;
		ia64_fc(wp); ia64_fc(wp + 2);
		++offp;
	}
	ia64_sync_i();
	ia64_srlz_i();
}

static void __init
patch_fsyscall_table (unsigned long start, unsigned long end)
{
	extern unsigned long fsyscall_table[NR_syscalls];
	s32 *offp = (s32 *) start;
	u64 ip;

	while (offp < (s32 *) end) {
		ip = (u64) ia64_imva((char *) offp + *offp);
		ia64_patch_imm64(ip, (u64) fsyscall_table);
		ia64_fc((void *) ip);
		++offp;
	}
	ia64_sync_i();
	ia64_srlz_i();
}

static void __init
patch_brl_fsys_bubble_down (unsigned long start, unsigned long end)
{
	extern char fsys_bubble_down[];
	s32 *offp = (s32 *) start;
	u64 ip;

	while (offp < (s32 *) end) {
		ip = (u64) offp + *offp;
		ia64_patch_imm60((u64) ia64_imva((void *) ip),
				 (u64) (fsys_bubble_down - (ip & -16)) / 16);
		ia64_fc((void *) ip);
		++offp;
	}
	ia64_sync_i();
	ia64_srlz_i();
}

void __init
ia64_patch_gate (void)
{
#	define START(name)	((unsigned long) __start_gate_##name##_patchlist)
#	define END(name)	((unsigned long)__end_gate_##name##_patchlist)

	patch_fsyscall_table(START(fsyscall), END(fsyscall));
	patch_brl_fsys_bubble_down(START(brl_fsys_bubble_down), END(brl_fsys_bubble_down));
	ia64_patch_vtop(START(vtop), END(vtop));
	ia64_patch_mckinley_e9(START(mckinley_e9), END(mckinley_e9));
}

void ia64_patch_phys_stack_reg(unsigned long val)
{
	s32 * offp = (s32 *) __start___phys_stack_reg_patchlist;
	s32 * end = (s32 *) __end___phys_stack_reg_patchlist;
	u64 ip, mask, imm;

	/* see instruction format A4: adds r1 = imm13, r3 */
	mask = (0x3fUL << 27) | (0x7f << 13);
	imm = (((val >> 7) & 0x3f) << 27) | (val & 0x7f) << 13;

	while (offp < end) {
		ip = (u64) offp + *offp;
		ia64_patch(ip, mask, imm);
		ia64_fc(ip);
		++offp;
	}
	ia64_sync_i();
	ia64_srlz_i();
}
