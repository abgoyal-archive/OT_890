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
 * Copyright (C) 1999-2002 Hewlett-Packard Co
 *	Stephane Eranian <eranian@hpl.hp.com>
 *	David Mosberger-Tang <davidm@hpl.hp.com>
 * Copyright (C) 2002 Ken Chen <kenneth.w.chen@intel.com>
 *
 * 1/06/01 davidm	Tuned for Itanium.
 * 2/12/02 kchen	Tuned for both Itanium and McKinley
 * 3/08/02 davidm	Some more tweaking
 */

#include <asm/asmmacro.h>
#include <asm/page.h>

#ifdef CONFIG_ITANIUM
# define L3_LINE_SIZE	64	// Itanium L3 line size
# define PREFETCH_LINES	9	// magic number
#else
# define L3_LINE_SIZE	128	// McKinley L3 line size
# define PREFETCH_LINES	12	// magic number
#endif

#define saved_lc	r2
#define dst_fetch	r3
#define dst1		r8
#define dst2		r9
#define dst3		r10
#define dst4		r11

#define dst_last	r31

GLOBAL_ENTRY(clear_page)
	.prologue
	.regstk 1,0,0,0
	mov r16 = PAGE_SIZE/L3_LINE_SIZE-1	// main loop count, -1=repeat/until
	.save ar.lc, saved_lc
	mov saved_lc = ar.lc

	.body
	mov ar.lc = (PREFETCH_LINES - 1)
	mov dst_fetch = in0
	adds dst1 = 16, in0
	adds dst2 = 32, in0
	;;
.fetch:	stf.spill.nta [dst_fetch] = f0, L3_LINE_SIZE
	adds dst3 = 48, in0		// executing this multiple times is harmless
	br.cloop.sptk.few .fetch
	;;
	addl dst_last = (PAGE_SIZE - PREFETCH_LINES*L3_LINE_SIZE), dst_fetch
	mov ar.lc = r16			// one L3 line per iteration
	adds dst4 = 64, in0
	;;
#ifdef CONFIG_ITANIUM
	// Optimized for Itanium
1:	stf.spill.nta [dst1] = f0, 64
	stf.spill.nta [dst2] = f0, 64
	cmp.lt p8,p0=dst_fetch, dst_last
	;;
#else
	// Optimized for McKinley
1:	stf.spill.nta [dst1] = f0, 64
	stf.spill.nta [dst2] = f0, 64
	stf.spill.nta [dst3] = f0, 64
	stf.spill.nta [dst4] = f0, 128
	cmp.lt p8,p0=dst_fetch, dst_last
	;;
	stf.spill.nta [dst1] = f0, 64
	stf.spill.nta [dst2] = f0, 64
#endif
	stf.spill.nta [dst3] = f0, 64
(p8)	stf.spill.nta [dst_fetch] = f0, L3_LINE_SIZE
	br.cloop.sptk.few 1b
	;;
	mov ar.lc = saved_lc		// restore lc
	br.ret.sptk.many rp
END(clear_page)
