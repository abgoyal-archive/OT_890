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
 * Copyright (C) 2000 Jeff Dike (jdike@karaya.com)
 * Licensed under the GPL
 */

typedef long syscall_handler_t(unsigned long arg1, unsigned long arg2,
			       unsigned long arg3, unsigned long arg4,
			       unsigned long arg5, unsigned long arg6);

#define EXECUTE_SYSCALL(syscall, regs) \
        (*sys_call_table[syscall])(UM_SYSCALL_ARG1(&regs), \
			           UM_SYSCALL_ARG2(&regs), \
				   UM_SYSCALL_ARG3(&regs), \
				   UM_SYSCALL_ARG4(&regs), \
				   UM_SYSCALL_ARG5(&regs), \
				   UM_SYSCALL_ARG6(&regs))

extern syscall_handler_t sys_mincore;
extern syscall_handler_t sys_madvise;

/* old_mmap needs the correct prototype since syscall_kern.c includes
 * this file.
 */
int old_mmap(unsigned long addr, unsigned long len,
	     unsigned long prot, unsigned long flags,
	     unsigned long fd, unsigned long offset);

#define ARCH_SYSCALLS \
	[ __NR_modify_ldt ] = sys_ni_syscall, \
	[ __NR_pciconfig_read ] = sys_ni_syscall, \
	[ __NR_pciconfig_write ] = sys_ni_syscall, \
	[ __NR_pciconfig_iobase ] = sys_ni_syscall, \
	[ __NR_pivot_root ] = sys_ni_syscall, \
	[ __NR_multiplexer ] = sys_ni_syscall, \
	[ __NR_mmap ] = old_mmap, \
	[ __NR_madvise ] = sys_madvise, \
	[ __NR_mincore ] = sys_mincore, \
	[ __NR_iopl ] = (syscall_handler_t *) sys_ni_syscall, \
	[ __NR_utimes ] = (syscall_handler_t *) sys_utimes, \
	[ __NR_fadvise64 ] = (syscall_handler_t *) sys_fadvise64,

#define LAST_ARCH_SYSCALL __NR_fadvise64

/*
 * Overrides for Emacs so that we follow Linus's tabbing style.
 * Emacs will notice this stuff at the end of the file and automatically
 * adjust the settings for this buffer only.  This must remain at the end
 * of the file.
 * ---------------------------------------------------------------------------
 * Local variables:
 * c-file-style: "linux"
 * End:
 */
