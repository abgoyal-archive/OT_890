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

#include <linux/spinlock.h>
#include <linux/hardirq.h>
#include <linux/ftrace.h>
#include <linux/percpu.h>
#include <linux/init.h>
#include <linux/list.h>

#include <asm/ftrace.h>

static const u32 ftrace_nop = 0x01000000;

unsigned char *ftrace_nop_replace(void)
{
	return (char *)&ftrace_nop;
}

unsigned char *ftrace_call_replace(unsigned long ip, unsigned long addr)
{
	static u32 call;
	s32 off;

	off = ((s32)addr - (s32)ip);
	call = 0x40000000 | ((u32)off >> 2);

	return (unsigned char *) &call;
}

int
ftrace_modify_code(unsigned long ip, unsigned char *old_code,
		   unsigned char *new_code)
{
	u32 old = *(u32 *)old_code;
	u32 new = *(u32 *)new_code;
	u32 replaced;
	int faulted;

	__asm__ __volatile__(
	"1:	cas	[%[ip]], %[old], %[new]\n"
	"	flush	%[ip]\n"
	"	mov	0, %[faulted]\n"
	"2:\n"
	"	.section .fixup,#alloc,#execinstr\n"
	"	.align	4\n"
	"3:	sethi	%%hi(2b), %[faulted]\n"
	"	jmpl	%[faulted] + %%lo(2b), %%g0\n"
	"	 mov	1, %[faulted]\n"
	"	.previous\n"
	"	.section __ex_table,\"a\"\n"
	"	.align	4\n"
	"	.word	1b, 3b\n"
	"	.previous\n"
	: "=r" (replaced), [faulted] "=r" (faulted)
	: [new] "0" (new), [old] "r" (old), [ip] "r" (ip)
	: "memory");

	if (replaced != old && replaced != new)
		faulted = 2;

	return faulted;
}

int ftrace_update_ftrace_func(ftrace_func_t func)
{
	unsigned long ip = (unsigned long)(&ftrace_call);
	unsigned char old[MCOUNT_INSN_SIZE], *new;

	memcpy(old, &ftrace_call, MCOUNT_INSN_SIZE);
	new = ftrace_call_replace(ip, (unsigned long)func);
	return ftrace_modify_code(ip, old, new);
}

int __init ftrace_dyn_arch_init(void *data)
{
	ftrace_mcount_set(data);
	return 0;
}
