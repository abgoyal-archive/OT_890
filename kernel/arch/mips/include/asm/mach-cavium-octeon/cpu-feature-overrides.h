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
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 *
 * Copyright (C) 2004 Cavium Networks
 */
#ifndef __ASM_MACH_CAVIUM_OCTEON_CPU_FEATURE_OVERRIDES_H
#define __ASM_MACH_CAVIUM_OCTEON_CPU_FEATURE_OVERRIDES_H

#include <linux/types.h>
#include <asm/mipsregs.h>

/*
 * Cavium Octeons are MIPS64v2 processors
 */
#define cpu_dcache_line_size()	128
#define cpu_icache_line_size()	128


#define cpu_has_4kex		1
#define cpu_has_3k_cache	0
#define cpu_has_4k_cache	0
#define cpu_has_tx39_cache	0
#define cpu_has_fpu		0
#define cpu_has_counter		1
#define cpu_has_watch		1
#define cpu_has_divec		1
#define cpu_has_vce		0
#define cpu_has_cache_cdex_p	0
#define cpu_has_cache_cdex_s	0
#define cpu_has_prefetch	1

/*
 * We should disable LL/SC on non SMP systems as it is faster to
 * disable interrupts for atomic access than a LL/SC.  Unfortunatly we
 * cannot as this breaks asm/futex.h
 */
#define cpu_has_llsc		1
#define cpu_has_vtag_icache	1
#define cpu_has_dc_aliases	0
#define cpu_has_ic_fills_f_dc	0
#define cpu_has_64bits		1
#define cpu_has_octeon_cache	1
#define cpu_has_saa		octeon_has_saa()
#define cpu_has_mips32r1	0
#define cpu_has_mips32r2	0
#define cpu_has_mips64r1	0
#define cpu_has_mips64r2	1
#define cpu_has_dsp		0
#define cpu_has_mipsmt		0
#define cpu_has_userlocal	0
#define cpu_has_vint		0
#define cpu_has_veic		0
#define ARCH_HAS_READ_CURRENT_TIMER 1
#define ARCH_HAS_IRQ_PER_CPU	1
#define ARCH_HAS_SPINLOCK_PREFETCH 1
#define spin_lock_prefetch(x) prefetch(x)
#define PREFETCH_STRIDE 128

static inline int read_current_timer(unsigned long *result)
{
	asm volatile ("rdhwr %0,$31\n"
#ifndef CONFIG_64BIT
		      "\tsll %0, 0"
#endif
		      : "=r" (*result));
	return 0;
}

static inline int octeon_has_saa(void)
{
	int id;
	asm volatile ("mfc0 %0, $15,0" : "=r" (id));
	return id >= 0x000d0300;
}

#endif
