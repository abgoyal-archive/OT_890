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
 * System call callback functions for SPUs
 */

#undef DEBUG

#include <linux/kallsyms.h>
#include <linux/module.h>
#include <linux/syscalls.h>

#include <asm/spu.h>
#include <asm/syscalls.h>
#include <asm/unistd.h>

/*
 * This table defines the system calls that an SPU can call.
 * It is currently a subset of the 64 bit powerpc system calls,
 * with the exact semantics.
 *
 * The reasons for disabling some of the system calls are:
 * 1. They interact with the way SPU syscalls are handled
 *    and we can't let them execute ever:
 *	restart_syscall, exit, for, execve, ptrace, ...
 * 2. They are deprecated and replaced by other means:
 *	uselib, pciconfig_*, sysfs, ...
 * 3. They are somewhat interacting with the system in a way
 *    we don't want an SPU to:
 *	reboot, init_module, mount, kexec_load
 * 4. They are optional and we can't rely on them being
 *    linked into the kernel. Unfortunately, the cond_syscall
 *    helper does not work here as it does not add the necessary
 *    opd symbols:
 *	mbind, mq_open, ipc, ...
 */

static void *spu_syscall_table[] = {
#define SYSCALL(func)		sys_ni_syscall,
#define COMPAT_SYS(func)	sys_ni_syscall,
#define PPC_SYS(func)		sys_ni_syscall,
#define OLDSYS(func)		sys_ni_syscall,
#define SYS32ONLY(func)		sys_ni_syscall,
#define SYSX(f, f3264, f32)	sys_ni_syscall,

#define SYSCALL_SPU(func)	sys_##func,
#define COMPAT_SYS_SPU(func)	sys_##func,
#define PPC_SYS_SPU(func)	ppc_##func,
#define SYSX_SPU(f, f3264, f32)	f,

#include <asm/systbl.h>
};

long spu_sys_callback(struct spu_syscall_block *s)
{
	long (*syscall)(u64 a1, u64 a2, u64 a3, u64 a4, u64 a5, u64 a6);

	if (s->nr_ret >= ARRAY_SIZE(spu_syscall_table)) {
		pr_debug("%s: invalid syscall #%lld", __func__, s->nr_ret);
		return -ENOSYS;
	}

	syscall = spu_syscall_table[s->nr_ret];

#ifdef DEBUG
	print_symbol(KERN_DEBUG "SPU-syscall %s:", (unsigned long)syscall);
	printk("syscall%ld(%lx, %lx, %lx, %lx, %lx, %lx)\n",
			s->nr_ret,
			s->parm[0], s->parm[1], s->parm[2],
			s->parm[3], s->parm[4], s->parm[5]);
#endif

	return syscall(s->parm[0], s->parm[1], s->parm[2],
		       s->parm[3], s->parm[4], s->parm[5]);
}
EXPORT_SYMBOL_GPL(spu_sys_callback);
