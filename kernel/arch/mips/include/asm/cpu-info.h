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
 * Copyright (C) 1994 Waldorf GMBH
 * Copyright (C) 1995, 1996, 1997, 1998, 1999, 2001, 2002, 2003 Ralf Baechle
 * Copyright (C) 1996 Paul M. Antoine
 * Copyright (C) 1999, 2000 Silicon Graphics, Inc.
 * Copyright (C) 2004  Maciej W. Rozycki
 */
#ifndef __ASM_CPU_INFO_H
#define __ASM_CPU_INFO_H

#include <linux/types.h>

#include <asm/cache.h>

/*
 * Descriptor for a cache
 */
struct cache_desc {
	unsigned int waysize;	/* Bytes per way */
	unsigned short sets;	/* Number of lines per set */
	unsigned char ways;	/* Number of ways */
	unsigned char linesz;	/* Size of line in bytes */
	unsigned char waybit;	/* Bits to select in a cache set */
	unsigned char flags;	/* Flags describing cache properties */
};

/*
 * Flag definitions
 */
#define MIPS_CACHE_NOT_PRESENT	0x00000001
#define MIPS_CACHE_VTAG		0x00000002	/* Virtually tagged cache */
#define MIPS_CACHE_ALIASES	0x00000004	/* Cache could have aliases */
#define MIPS_CACHE_IC_F_DC	0x00000008	/* Ic can refill from D-cache */
#define MIPS_IC_SNOOPS_REMOTE	0x00000010	/* Ic snoops remote stores */
#define MIPS_CACHE_PINDEX	0x00000020	/* Physically indexed cache */

struct cpuinfo_mips {
	unsigned long		udelay_val;
	unsigned long		asid_cache;

	/*
	 * Capability and feature descriptor structure for MIPS CPU
	 */
	unsigned long		options;
	unsigned long		ases;
	unsigned int		processor_id;
	unsigned int		fpu_id;
	unsigned int		cputype;
	int			isa_level;
	int			tlbsize;
	struct cache_desc	icache;	/* Primary I-cache */
	struct cache_desc	dcache;	/* Primary D or combined I/D cache */
	struct cache_desc	scache;	/* Secondary cache */
	struct cache_desc	tcache;	/* Tertiary/split secondary cache */
	int			srsets;	/* Shadow register sets */
	int			core;	/* physical core number */
#if defined(CONFIG_MIPS_MT_SMP) || defined(CONFIG_MIPS_MT_SMTC)
	/*
	 * In the MIPS MT "SMTC" model, each TC is considered
	 * to be a "CPU" for the purposes of scheduling, but
	 * exception resources, ASID spaces, etc, are common
	 * to all TCs within the same VPE.
	 */
	int			vpe_id;  /* Virtual Processor number */
#endif
#ifdef CONFIG_MIPS_MT_SMTC
	int			tc_id;   /* Thread Context number */
#endif
	void 			*data;	/* Additional data */
	unsigned int		watch_reg_count;   /* Number that exist */
	unsigned int		watch_reg_use_cnt; /* Usable by ptrace */
#define NUM_WATCH_REGS 4
	u16			watch_reg_masks[NUM_WATCH_REGS];
} __attribute__((aligned(SMP_CACHE_BYTES)));

extern struct cpuinfo_mips cpu_data[];
#define current_cpu_data cpu_data[smp_processor_id()]
#define raw_current_cpu_data cpu_data[raw_smp_processor_id()]

extern void cpu_probe(void);
extern void cpu_report(void);

extern const char *__cpu_name[];
#define cpu_name_string()	__cpu_name[smp_processor_id()]

#endif /* __ASM_CPU_INFO_H */
