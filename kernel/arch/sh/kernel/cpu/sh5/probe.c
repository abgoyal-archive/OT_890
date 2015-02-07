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
 * arch/sh/kernel/cpu/sh5/probe.c
 *
 * CPU Subtype Probing for SH-5.
 *
 * Copyright (C) 2000, 2001  Paolo Alberelli
 * Copyright (C) 2003 - 2007  Paul Mundt
 *
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 */
#include <linux/init.h>
#include <linux/io.h>
#include <linux/string.h>
#include <asm/processor.h>
#include <asm/cache.h>
#include <asm/tlb.h>

int __init detect_cpu_and_cache_system(void)
{
	unsigned long long cir;

	/*
	 * Do peeks in real mode to avoid having to set up a mapping for
	 * the WPC registers. On SH5-101 cut2, such a mapping would be
	 * exposed to an address translation erratum which would make it
	 * hard to set up correctly.
	 */
	cir = peek_real_address_q(0x0d000008);
	if ((cir & 0xffff) == 0x5103)
		boot_cpu_data.type = CPU_SH5_103;
	else if (((cir >> 32) & 0xffff) == 0x51e2)
		/* CPU.VCR aliased at CIR address on SH5-101 */
		boot_cpu_data.type = CPU_SH5_101;

	/*
	 * First, setup some sane values for the I-cache.
	 */
	boot_cpu_data.icache.ways		= 4;
	boot_cpu_data.icache.sets		= 256;
	boot_cpu_data.icache.linesz		= L1_CACHE_BYTES;
	boot_cpu_data.icache.way_incr		= (1 << 13);
	boot_cpu_data.icache.entry_shift	= 5;
	boot_cpu_data.icache.way_size		= boot_cpu_data.icache.sets *
						  boot_cpu_data.icache.linesz;
	boot_cpu_data.icache.entry_mask		= 0x1fe0;
	boot_cpu_data.icache.flags		= 0;

	/*
	 * Next, setup some sane values for the D-cache.
	 *
	 * On the SH5, these are pretty consistent with the I-cache settings,
	 * so we just copy over the existing definitions.. these can be fixed
	 * up later, especially if we add runtime CPU probing.
	 *
	 * Though in the meantime it saves us from having to duplicate all of
	 * the above definitions..
	 */
	boot_cpu_data.dcache		= boot_cpu_data.icache;

	/*
	 * Setup any cache-related flags here
	 */
#if defined(CONFIG_CACHE_WRITETHROUGH)
	set_bit(SH_CACHE_MODE_WT, &(boot_cpu_data.dcache.flags));
#elif defined(CONFIG_CACHE_WRITEBACK)
	set_bit(SH_CACHE_MODE_WB, &(boot_cpu_data.dcache.flags));
#endif

	/* Setup some I/D TLB defaults */
	sh64_tlb_init();

	return 0;
}
