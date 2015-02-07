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
 *  arch/arm/include/asm/proc-fns.h
 *
 *  Copyright (C) 1997-1999 Russell King
 *  Copyright (C) 2000 Deep Blue Solutions Ltd
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */
#ifndef __ASM_PROCFNS_H
#define __ASM_PROCFNS_H

#ifdef __KERNEL__


/*
 * Work out if we need multiple CPU support
 */
#undef MULTI_CPU
#undef CPU_NAME

/*
 * CPU_NAME - the prefix for CPU related functions
 */

#ifdef CONFIG_CPU_32
# ifdef CONFIG_CPU_ARM610
#  ifdef CPU_NAME
#   undef  MULTI_CPU
#   define MULTI_CPU
#  else
#   define CPU_NAME cpu_arm6
#  endif
# endif
# ifdef CONFIG_CPU_ARM7TDMI
#  ifdef CPU_NAME
#   undef  MULTI_CPU
#   define MULTI_CPU
#  else
#   define CPU_NAME cpu_arm7tdmi
#  endif
# endif
# ifdef CONFIG_CPU_ARM710
#  ifdef CPU_NAME
#   undef  MULTI_CPU
#   define MULTI_CPU
#  else
#   define CPU_NAME cpu_arm7
#  endif
# endif
# ifdef CONFIG_CPU_ARM720T
#  ifdef CPU_NAME
#   undef  MULTI_CPU
#   define MULTI_CPU
#  else
#   define CPU_NAME cpu_arm720
#  endif
# endif
# ifdef CONFIG_CPU_ARM740T
#  ifdef CPU_NAME
#   undef  MULTI_CPU
#   define MULTI_CPU
#  else
#   define CPU_NAME cpu_arm740
#  endif
# endif
# ifdef CONFIG_CPU_ARM9TDMI
#  ifdef CPU_NAME
#   undef  MULTI_CPU
#   define MULTI_CPU
#  else
#   define CPU_NAME cpu_arm9tdmi
#  endif
# endif
# ifdef CONFIG_CPU_ARM920T
#  ifdef CPU_NAME
#   undef  MULTI_CPU
#   define MULTI_CPU
#  else
#   define CPU_NAME cpu_arm920
#  endif
# endif
# ifdef CONFIG_CPU_ARM922T
#  ifdef CPU_NAME
#   undef  MULTI_CPU
#   define MULTI_CPU
#  else
#   define CPU_NAME cpu_arm922
#  endif
# endif
# ifdef CONFIG_CPU_ARM925T
#  ifdef CPU_NAME
#   undef  MULTI_CPU
#   define MULTI_CPU
#  else
#   define CPU_NAME cpu_arm925
#  endif
# endif
# ifdef CONFIG_CPU_ARM926T
#  ifdef CPU_NAME
#   undef  MULTI_CPU
#   define MULTI_CPU
#  else
#   define CPU_NAME cpu_arm926
#  endif
# endif
# ifdef CONFIG_CPU_ARM940T
#  ifdef CPU_NAME
#   undef  MULTI_CPU
#   define MULTI_CPU
#  else
#   define CPU_NAME cpu_arm940
#  endif
# endif
# ifdef CONFIG_CPU_ARM946E
#  ifdef CPU_NAME
#   undef  MULTI_CPU
#   define MULTI_CPU
#  else
#   define CPU_NAME cpu_arm946
#  endif
# endif
# ifdef CONFIG_CPU_SA110
#  ifdef CPU_NAME
#   undef  MULTI_CPU
#   define MULTI_CPU
#  else
#   define CPU_NAME cpu_sa110
#  endif
# endif
# ifdef CONFIG_CPU_SA1100
#  ifdef CPU_NAME
#   undef  MULTI_CPU
#   define MULTI_CPU
#  else
#   define CPU_NAME cpu_sa1100
#  endif
# endif
# ifdef CONFIG_CPU_ARM1020
#  ifdef CPU_NAME
#   undef  MULTI_CPU
#   define MULTI_CPU
#  else
#   define CPU_NAME cpu_arm1020
#  endif
# endif
# ifdef CONFIG_CPU_ARM1020E
#  ifdef CPU_NAME
#   undef  MULTI_CPU
#   define MULTI_CPU
#  else
#   define CPU_NAME cpu_arm1020e
#  endif
# endif
# ifdef CONFIG_CPU_ARM1022
#  ifdef CPU_NAME
#   undef  MULTI_CPU
#   define MULTI_CPU
#  else
#   define CPU_NAME cpu_arm1022
#  endif
# endif
# ifdef CONFIG_CPU_ARM1026
#  ifdef CPU_NAME
#   undef  MULTI_CPU
#   define MULTI_CPU
#  else
#   define CPU_NAME cpu_arm1026
#  endif
# endif
# ifdef CONFIG_CPU_XSCALE
#  ifdef CPU_NAME
#   undef  MULTI_CPU
#   define MULTI_CPU
#  else
#   define CPU_NAME cpu_xscale
#  endif
# endif
# ifdef CONFIG_CPU_XSC3
#  ifdef CPU_NAME
#   undef  MULTI_CPU
#   define MULTI_CPU
#  else
#   define CPU_NAME cpu_xsc3
#  endif
# endif
# ifdef CONFIG_CPU_FEROCEON
#  ifdef CPU_NAME
#   undef  MULTI_CPU
#   define MULTI_CPU
#  else
#   define CPU_NAME cpu_feroceon
#  endif
# endif
# ifdef CONFIG_CPU_V6
#  ifdef CPU_NAME
#   undef  MULTI_CPU
#   define MULTI_CPU
#  else
#   define CPU_NAME cpu_v6
#  endif
# endif
# ifdef CONFIG_CPU_V7
#  ifdef CPU_NAME
#   undef  MULTI_CPU
#   define MULTI_CPU
#  else
#   define CPU_NAME cpu_v7
#  endif
# endif
#endif

#ifndef __ASSEMBLY__

#ifndef MULTI_CPU
#include <asm/cpu-single.h>
#else
#include <asm/cpu-multi32.h>
#endif

#include <asm/memory.h>

#ifdef CONFIG_MMU

#define cpu_switch_mm(pgd,mm) cpu_do_switch_mm(virt_to_phys(pgd),mm)

#define cpu_get_pgd()	\
	({						\
		unsigned long pg;			\
		__asm__("mrc	p15, 0, %0, c2, c0, 0"	\
			 : "=r" (pg) : : "cc");		\
		pg &= ~0x3fff;				\
		(pgd_t *)phys_to_virt(pg);		\
	})

#endif

#endif /* __ASSEMBLY__ */
#endif /* __KERNEL__ */
#endif /* __ASM_PROCFNS_H */
