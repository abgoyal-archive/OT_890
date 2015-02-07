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
 *  arch/arm/include/asm/glue.h
 *
 *  Copyright (C) 1997-1999 Russell King
 *  Copyright (C) 2000-2002 Deep Blue Solutions Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 *  This file provides the glue to stick the processor-specific bits
 *  into the kernel in an efficient manner.  The idea is to use branches
 *  when we're only targetting one class of TLB, or indirect calls
 *  when we're targetting multiple classes of TLBs.
 */
#ifdef __KERNEL__


#ifdef __STDC__
#define ____glue(name,fn)	name##fn
#else
#define ____glue(name,fn)	name/**/fn
#endif
#define __glue(name,fn)		____glue(name,fn)



/*
 *	Data Abort Model
 *	================
 *
 *	We have the following to choose from:
 *	  arm6          - ARM6 style
 *	  arm7		- ARM7 style
 *	  v4_early	- ARMv4 without Thumb early abort handler
 *	  v4t_late	- ARMv4 with Thumb late abort handler
 *	  v4t_early	- ARMv4 with Thumb early abort handler
 *	  v5tej_early	- ARMv5 with Thumb and Java early abort handler
 *	  xscale	- ARMv5 with Thumb with Xscale extensions
 *	  v6_early	- ARMv6 generic early abort handler
 *	  v7_early	- ARMv7 generic early abort handler
 */
#undef CPU_DABORT_HANDLER
#undef MULTI_DABORT

#if defined(CONFIG_CPU_ARM610)
# ifdef CPU_DABORT_HANDLER
#  define MULTI_DABORT 1
# else
#  define CPU_DABORT_HANDLER cpu_arm6_data_abort
# endif
#endif

#if defined(CONFIG_CPU_ARM710)
# ifdef CPU_DABORT_HANDLER
#  define MULTI_DABORT 1
# else
#  define CPU_DABORT_HANDLER cpu_arm7_data_abort
# endif
#endif

#ifdef CONFIG_CPU_ABRT_LV4T
# ifdef CPU_DABORT_HANDLER
#  define MULTI_DABORT 1
# else
#  define CPU_DABORT_HANDLER v4t_late_abort
# endif
#endif

#ifdef CONFIG_CPU_ABRT_EV4
# ifdef CPU_DABORT_HANDLER
#  define MULTI_DABORT 1
# else
#  define CPU_DABORT_HANDLER v4_early_abort
# endif
#endif

#ifdef CONFIG_CPU_ABRT_EV4T
# ifdef CPU_DABORT_HANDLER
#  define MULTI_DABORT 1
# else
#  define CPU_DABORT_HANDLER v4t_early_abort
# endif
#endif

#ifdef CONFIG_CPU_ABRT_EV5TJ
# ifdef CPU_DABORT_HANDLER
#  define MULTI_DABORT 1
# else
#  define CPU_DABORT_HANDLER v5tj_early_abort
# endif
#endif

#ifdef CONFIG_CPU_ABRT_EV5T
# ifdef CPU_DABORT_HANDLER
#  define MULTI_DABORT 1
# else
#  define CPU_DABORT_HANDLER v5t_early_abort
# endif
#endif

#ifdef CONFIG_CPU_ABRT_EV6
# ifdef CPU_DABORT_HANDLER
#  define MULTI_DABORT 1
# else
#  define CPU_DABORT_HANDLER v6_early_abort
# endif
#endif

#ifdef CONFIG_CPU_ABRT_EV7
# ifdef CPU_DABORT_HANDLER
#  define MULTI_DABORT 1
# else
#  define CPU_DABORT_HANDLER v7_early_abort
# endif
#endif

#ifndef CPU_DABORT_HANDLER
#error Unknown data abort handler type
#endif

/*
 * Prefetch abort handler.  If the CPU has an IFAR use that, otherwise
 * use the address of the aborted instruction
 */
#undef CPU_PABORT_HANDLER
#undef MULTI_PABORT

#ifdef CONFIG_CPU_PABRT_IFAR
# ifdef CPU_PABORT_HANDLER
#  define MULTI_PABORT 1
# else
#  define CPU_PABORT_HANDLER(reg, insn)	mrc p15, 0, reg, cr6, cr0, 2
# endif
#endif

#ifdef CONFIG_CPU_PABRT_NOIFAR
# ifdef CPU_PABORT_HANDLER
#  define MULTI_PABORT 1
# else
#  define CPU_PABORT_HANDLER(reg, insn)	mov reg, insn
# endif
#endif

#ifndef CPU_PABORT_HANDLER
#error Unknown prefetch abort handler type
#endif

#endif
