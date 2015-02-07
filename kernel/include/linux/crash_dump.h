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

#ifndef LINUX_CRASH_DUMP_H
#define LINUX_CRASH_DUMP_H

#ifdef CONFIG_CRASH_DUMP
#include <linux/kexec.h>
#include <linux/smp_lock.h>
#include <linux/device.h>
#include <linux/proc_fs.h>

#define ELFCORE_ADDR_MAX	(-1ULL)
#define ELFCORE_ADDR_ERR	(-2ULL)

extern unsigned long long elfcorehdr_addr;

extern ssize_t copy_oldmem_page(unsigned long, char *, size_t,
						unsigned long, int);

/* Architecture code defines this if there are other possible ELF
 * machine types, e.g. on bi-arch capable hardware. */
#ifndef vmcore_elf_check_arch_cross
#define vmcore_elf_check_arch_cross(x) 0
#endif

#define vmcore_elf_check_arch(x) (elf_check_arch(x) || vmcore_elf_check_arch_cross(x))

/*
 * is_kdump_kernel() checks whether this kernel is booting after a panic of
 * previous kernel or not. This is determined by checking if previous kernel
 * has passed the elf core header address on command line.
 *
 * This is not just a test if CONFIG_CRASH_DUMP is enabled or not. It will
 * return 1 if CONFIG_CRASH_DUMP=y and if kernel is booting after a panic of
 * previous kernel.
 */

static inline int is_kdump_kernel(void)
{
	return (elfcorehdr_addr != ELFCORE_ADDR_MAX) ? 1 : 0;
}

/* is_vmcore_usable() checks if the kernel is booting after a panic and
 * the vmcore region is usable.
 *
 * This makes use of the fact that due to alignment -2ULL is not
 * a valid pointer, much in the vain of IS_ERR(), except
 * dealing directly with an unsigned long long rather than a pointer.
 */

static inline int is_vmcore_usable(void)
{
	return is_kdump_kernel() && elfcorehdr_addr != ELFCORE_ADDR_ERR ? 1 : 0;
}

/* vmcore_unusable() marks the vmcore as unusable,
 * without disturbing the logic of is_kdump_kernel()
 */

static inline void vmcore_unusable(void)
{
	if (is_kdump_kernel())
		elfcorehdr_addr = ELFCORE_ADDR_ERR;
}
#else /* !CONFIG_CRASH_DUMP */
static inline int is_kdump_kernel(void) { return 0; }
#endif /* CONFIG_CRASH_DUMP */

extern unsigned long saved_max_pfn;
#endif /* LINUX_CRASHDUMP_H */
