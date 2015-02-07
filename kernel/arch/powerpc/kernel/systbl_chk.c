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
 * This file, when run through CPP produces a list of syscall numbers
 * in the order of systbl.h.  That way we can check for gaps and syscalls
 * that are out of order.
 *
 * Unfortunately, we cannot check for the correct ordering of entries
 * using SYSX().
 *
 * Copyright © IBM Corporation
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version
 * 2 of the License, or (at your option) any later version.
 */
#include <asm/unistd.h>

#define SYSCALL(func)		__NR_##func
#define COMPAT_SYS(func)	__NR_##func
#define PPC_SYS(func)		__NR_##func
#ifdef CONFIG_PPC64
#define OLDSYS(func)		-1
#define SYS32ONLY(func)		-1
#else
#define OLDSYS(func)		__NR_old##func
#define SYS32ONLY(func)		__NR_##func
#endif
#define SYSX(f, f3264, f32)	-1

#define SYSCALL_SPU(func)	SYSCALL(func)
#define COMPAT_SYS_SPU(func)	COMPAT_SYS(func)
#define PPC_SYS_SPU(func)	PPC_SYS(func)
#define SYSX_SPU(f, f3264, f32)	SYSX(f, f3264, f32)

/* Just insert a marker for ni_syscalls */
#define	__NR_ni_syscall		-1

/*
 * These are the known exceptions.
 * Hopefully, there will be no more.
 */
#define	__NR_llseek		__NR__llseek
#undef	__NR_umount
#define	__NR_umount		__NR_umount2
#define	__NR_old_getrlimit	__NR_getrlimit
#define	__NR_newstat		__NR_stat
#define	__NR_newlstat		__NR_lstat
#define	__NR_newfstat		__NR_fstat
#define	__NR_newuname		__NR_uname
#define	__NR_sysctl		__NR__sysctl
#define __NR_olddebug_setcontext	__NR_sys_debug_setcontext

/* We call sys_ugetrlimit for syscall number __NR_getrlimit */
#define getrlimit		ugetrlimit

START_TABLE
#include <asm/systbl.h>
END_TABLE __NR_syscalls
