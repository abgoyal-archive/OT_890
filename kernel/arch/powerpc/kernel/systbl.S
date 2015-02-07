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
 * This file contains the table of syscall-handling functions.
 *    Copyright (C) 1995-1996 Gary Thomas (gdt@linuxppc.org)
 *
 * Largely rewritten by Cort Dougan (cort@cs.nmt.edu)
 * and Paul Mackerras.
 *
 * Adapted for iSeries by Mike Corrigan (mikejc@us.ibm.com)
 * PPC64 updates by Dave Engebretsen (engebret@us.ibm.com) 
 * 
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version
 * 2 of the License, or (at your option) any later version.
 */

#include <asm/ppc_asm.h>

#ifdef CONFIG_PPC64
#define SYSCALL(func)		.llong	.sys_##func,.sys_##func
#define COMPAT_SYS(func)	.llong	.sys_##func,.compat_sys_##func
#define PPC_SYS(func)		.llong	.ppc_##func,.ppc_##func
#define OLDSYS(func)		.llong	.sys_ni_syscall,.sys_ni_syscall
#define SYS32ONLY(func)		.llong	.sys_ni_syscall,.compat_sys_##func
#define SYSX(f, f3264, f32)	.llong	.f,.f3264
#else
#define SYSCALL(func)		.long	sys_##func
#define COMPAT_SYS(func)	.long	sys_##func
#define PPC_SYS(func)		.long	ppc_##func
#define OLDSYS(func)		.long	sys_##func
#define SYS32ONLY(func)		.long	sys_##func
#define SYSX(f, f3264, f32)	.long	f32
#endif
#define SYSCALL_SPU(func)	SYSCALL(func)
#define COMPAT_SYS_SPU(func)	COMPAT_SYS(func)
#define PPC_SYS_SPU(func)	PPC_SYS(func)
#define SYSX_SPU(f, f3264, f32)	SYSX(f, f3264, f32)

#ifdef CONFIG_PPC64
#define sys_sigpending	sys_ni_syscall
#define sys_old_getrlimit sys_ni_syscall

	.p2align	3
#endif

_GLOBAL(sys_call_table)
#include <asm/systbl.h>
